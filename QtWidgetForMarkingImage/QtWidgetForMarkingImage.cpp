#include "QtWidgetForMarkingImage.h"

QtWidgetForMarkingImage::QtWidgetForMarkingImage(QWidget *parent)
    : QWidget(parent),
    setCustomSize{false},
    setCustomName{false}
{
    LOG.logMessege("app start", LogLevel::_INFO_);
    ui.setupUi(this);
    loadClassifirs();
   
    connect(ui.widgetForImage, &QtGuiDisplay::newActivScale, this, &QtWidgetForMarkingImage::slot_updateScale);
    connect(ui.PB_chooseImage, SIGNAL(clicked()), this, SLOT(slot_chooseImageName()));
    connect(ui.PB_previousImage, SIGNAL(clicked()), this, SLOT(slot_previousImage()));
    connect(ui.PB_nextImage, SIGNAL(clicked()), this, SLOT(slot_nextImage()));

    connect(ui.cb_classes, &QComboBox::currentTextChanged, this, &QtWidgetForMarkingImage::slot_changeTypes);
    connect(ui.PB_changeClassLabel, SIGNAL(clicked()), this, SLOT(slot_changeClassLabel()));
    connect(ui.pb_changeType, SIGNAL(clicked()), this, SLOT(slot_changeTypeLabel()));
    connect(ui.PB_addRectangel, SIGNAL(clicked()), this, SLOT(slot_addMarkupObject()));
    connect(ui.PB_deleteRectangel, SIGNAL(clicked()), this, SLOT(slot_delRect()));
    connect(ui.PB_markingImage, SIGNAL(clicked()), this, SLOT(slot_markingAndSaveImage()));

    connect(ui.RB_customSize, SIGNAL(clicked()), this, SLOT(slot_changeSizeRB()));
    connect(ui.RB_originalSize, SIGNAL(clicked()), this, SLOT(slot_changeSizeRB()));
    connect(ui.LE_newHeigth, SIGNAL(textChanged(QString)), this, SLOT(slot_changeImageSize()));
    connect(ui.LE_newWidth, SIGNAL(textChanged(QString)), this, SLOT(slot_changeImageSize()));

    connect(ui.RB_originalName, SIGNAL(clicked()), this, SLOT(slot_changeSaveNameRB()));
    connect(ui.RB_customName, SIGNAL(clicked()), this, SLOT(slot_changeSaveNameRB()));
    connect(ui.le_saveDirectoryImage, SIGNAL(textChanged(QString)), this, SLOT(slot_setSaveDirectoryForImage(QString)));
    connect(ui.le_saveDirectoryLabel, SIGNAL(textChanged(QString)), this, SLOT(slot_setSaveDirectoryForLabel(QString)));
    connect(ui.LE_sequenceStart, SIGNAL(textChanged(QString)), this, SLOT(slot_changeStartIterator(QString)));
    connect(ui.pb_setSaveDirectoryForImage, SIGNAL(clicked()), this, SLOT(slot_chooseSaveDirectoryForImage()));
    connect(ui.pb_setSaveDirectoryForLabel, SIGNAL(clicked()), this, SLOT(slot_chooseSaveDirectoryForLabel()));

    connect(ui.widgetForImage, &QtGuiDisplay::newActivFigure, this, &QtWidgetForMarkingImage::slot_setActivMarkupObject);
}

QtWidgetForMarkingImage::~QtWidgetForMarkingImage()
{
    delete[] newLabel;
    newLabel = nullptr;
    LOG.logMessege("app stop", LogLevel::_INFO_);
}

void QtWidgetForMarkingImage::slot_chooseImageName()
{
    QFileDialog fd{};
    QStringList imageNames{ fd.getOpenFileNames(this, "Images", "", tr("Images files (*.png *.jpg)")) };
    initScrollArea_withImageName(imageNames);
}

void QtWidgetForMarkingImage::initScrollArea_withImageName(QStringList& const imageNames)
{
    delete[] newLabel;
    newLabel = nullptr;
    quantityImage = 0;
    activImageId = 0;
    newLabel = new QtWidgetsImageName[imageNames.size()];
    quantityImage = imageNames.size();
    for (size_t i{ 0 }; i < quantityImage; ++i)
    {
        connect(&newLabel[i], SIGNAL(mousePres(int)), this, SLOT(slot_activateImage(int)));
        newLabel[i].setImageName(imageNames[i]);
        newLabel[i].setId(i);
        ui.VL_withImageName->addWidget(&newLabel[i]);
    }
    if (quantityImage > 0)
    {
        ui.PB_nextImage->setEnabled(true);
        ui.PB_previousImage->setEnabled(true);
        setActivImage(0);
    }
}

void QtWidgetForMarkingImage::setActivImage(int const newActivImageId)
{
    for (; markupObjects_.size() > 0;)
    {
        slot_delRect();
    }
    if (quantityImage > 0)
    {
        newLabel[activImageId].setStyleSheet("");
        newLabel[newActivImageId].setStyleSheet("border : 1px solid red;");
        activImageId = newActivImageId;
        QString dirictoriName{ newLabel[newActivImageId].getImageName() };
        cv::Mat buferImage{ cv::imread(dirictoriName.toStdString()) };
        if (!buferImage.empty())
        {
            activImage_.updateFrame(buferImage);
            activImage_.setStatusFrame(false);
            if (setCustomSize)
                resizeActivImage();
            ui.widgetForImage->setActivFrame(activImage_);
            ui.widgetForImage->updateImage();
            
            size_t found{ dirictoriName.toStdString().find_last_of(".") };
            QString fileName{ QString::fromStdString(dirictoriName.toStdString().erase(found, dirictoriName.size() - 1)) + ".txt" };
            if (!saveDirektoryForLabel.isNull())
            {
                found = fileName.toStdString().find_last_of("/\\");
                fileName = QString::fromStdString(fileName.toStdString().substr(found + 1));
                QString fileNameBufer{ fileName };
                fileName = saveDirektoryForLabel + '/' + fileNameBufer;
            }
            
            loadMarking(fileName.toStdString());
        }
    }
}

void QtWidgetForMarkingImage::resizeActivImage()
{
    if (ui.LE_newHeigth->text() != "" && ui.LE_newWidth->text() != "")
    {
        QString newHeigth{ ui.LE_newHeigth->text() };
        QString newWidth{ ui.LE_newWidth->text() };
        cv::Mat buferImage{ activImage_.getMat() };
        cv::resize(buferImage, buferImage, cv::Size(newWidth.toInt(), newHeigth.toInt()));
        activImage_.updateFrame(buferImage);
    }
}

void QtWidgetForMarkingImage::saveMarking()
{
    std::string saveNameForImage{ setSaveNameForImage() };
    std::string saveNameForLabel{ setSaveNameForLabel(saveNameForImage) };
    std::ofstream objectCoordinate(saveNameForLabel, std::ios_base::out | std::ios_base::trunc);
    FigureRectangle* limitRect{};
    
    cv::Size imageSize{ activImage_.getMat().size() };
    for (size_t i{ }; i < markupObjects_.size(); ++i)
    {
        limitRect = markupObjects_[i].position;
        objectCoordinate << markupObjects_[i].getClass() + 1 << " " << static_cast<float>(limitRect->getWidth() / 2 + limitRect->getX()) / imageSize.width << " " << static_cast<float>(limitRect->getHeidth() / 2 + limitRect->getY()) / imageSize.height
            << " " << static_cast<float>(limitRect->getWidth()) / imageSize.width << " " << static_cast<float>(limitRect->getHeidth()) / imageSize.height << " " << markupObjects_[i].getType() << std::endl;
    }
    cv::imwrite(saveNameForImage, activImage_.getMat());
    objectCoordinate.close();
}

void QtWidgetForMarkingImage::slot_previousImage()
{
    if (activImageId == 0)
        setActivImage(quantityImage-1);
    else
        setActivImage(activImageId-1);
}

void QtWidgetForMarkingImage::slot_nextImage()
{
    if (activImageId == quantityImage-1)
        setActivImage(0);
    else
        setActivImage(activImageId+1);
}

void QtWidgetForMarkingImage::slot_changeSizeRB()
{
    if (ui.RB_originalSize->isChecked())
    {
        ui.LE_newHeigth->setEnabled(false);
        ui.LE_newWidth->setEnabled(false);
        ui.label_customHeigth->setEnabled(false);
        ui.label_customWidth->setEnabled(false);
        setCustomSize = false;
    }
    else
    {
        ui.LE_newHeigth->setEnabled(true);
        ui.LE_newWidth->setEnabled(true);
        ui.label_customHeigth->setEnabled(true);
        ui.label_customWidth->setEnabled(true);
        setCustomSize = true;
    }
    setActivImage(activImageId);
}

std::string QtWidgetForMarkingImage::setSaveNameForImage()
{
    QString saveName{};
    if (setCustomName)
    {
        if (ui.CB_customName->isChecked())
        {
            saveName = ui.LE_customName->text();
        }
        if (ui.CB_sequence->isChecked())
        {
            ui.LE_curentValue->setText(QString::number(sequenceIterator));
            saveName += QString::number(sequenceIterator);
            ++sequenceIterator;
        }
    }
    else
    {
        std::size_t found = newLabel[activImageId].getImageName().toStdString().find_last_of("/\\");
        saveName = QString::fromStdString(newLabel[activImageId].getImageName().toStdString().substr(found + 1));
        found = saveName.toStdString().find_last_of(".");
        saveName = QString::fromStdString(saveName.toStdString().erase(found, saveName.size()-1));
    }
    if (!saveDirektoryForImage.isNull())
        saveName = saveDirektoryForImage + QString("/") + saveName;
    saveName += QString(".png");
    return saveName.toStdString();
}

std::string QtWidgetForMarkingImage::setSaveNameForLabel(const std::string& imageName)
{
    std::string saveName{ imageName };
    std::size_t found{ imageName.find_last_of(".") };
    saveName.erase(found, saveName.size() - 1);

    if (!saveDirektoryForLabel.isNull())
    {
        found = saveName.find_last_of("/\\");
        saveName.erase(0, found);
        saveName = saveDirektoryForLabel.toStdString() + saveName;
    }
    saveName += ".txt";
    return saveName;
}

void QtWidgetForMarkingImage::loadClassifirs()
{
    try {
        classLabels.setClassifier("classes/classes.txt");
        for (size_t i{}; i < classLabels.size(); ++i)
        {
            ui.cb_classes->addItem(QString::fromLocal8Bit(classLabels[i].c_str()));
            typeClasses[classLabels[i]].setClassifier("classes/" + classLabels[i] + ".txt");
        }
        ui.cb_types->clear();
        for (size_t i{}; i < typeClasses[classLabels[ui.cb_classes->currentIndex()]].size(); ++i)
        {
            ui.cb_types->addItem(QString::fromLocal8Bit(typeClasses[classLabels[ui.cb_classes->currentIndex()]][i].c_str()));
        }
        LOG.logMessege("classifire load", LogLevel::_INFO_);
    }
    catch (std::string err)
    {
        LOG.logMessege(err, LogLevel::_ERROR_);
    }
}

void QtWidgetForMarkingImage::parsLineToOjectParams(const std::string& line, MarkupObject& object)
{
    std::stringstream ss(line);
    std::string temp;
    int i{ 0 };
    cv::Rect2f bbox{};
    while (std::getline(ss, temp, ' '))
    {
        switch (i)
        {
        case 0:
        {
            int classId{ std::atoi(temp.c_str()) - 1 };
            object.setClass(classId);
            ui.cb_classes->setCurrentIndex(classId);
            break;
        }
        case 1:
        {
            bbox.x = std::atof(temp.c_str());
            break;
        }
        case 2:
        {
            bbox.y = std::atof(temp.c_str());
            break;
        }
        case 3:
        {
            bbox.width = std::atof(temp.c_str());
            break;
        }
        case 4:
        {
            bbox.height = std::atof(temp.c_str());
            break;
        }
        case 5:
        {
            int typeId{ std::atoi(temp.c_str()) };
            object.setType(typeId);
            ui.cb_types->setCurrentIndex(typeId);
            break;
        }
        }
        ++i;
    }
    convertBboxToAbsluteCoordinate(bbox, activImage_.getMat().size());
    object.position->setRectangle(bbox.x, bbox.y, bbox.width, bbox.height);
}

void QtWidgetForMarkingImage::convertBboxToAbsluteCoordinate(cv::Rect2f& inputRect, const cv::Size2i imageSize)
{
    inputRect.width *= imageSize.width;
    inputRect.height *= imageSize.height;
    inputRect.x = inputRect.x * imageSize.width - inputRect.width / 2.0;
    inputRect.y = inputRect.y * imageSize.height - inputRect.height / 2.0;
}

void QtWidgetForMarkingImage::loadMarking(const std::string& fileName)
{
    std::ifstream fileWithMarks{ fileName, std::ios::in };
    if (fileWithMarks.is_open())
    {
        std::string line{};
        while (std::getline(fileWithMarks, line))
        {
            activMarkupObject_ = markupObjects_.size();
            markupObjects_.push_back(MarkupObject());
            markupObjects_[activMarkupObject_].setPenScale(activPenScale);
            ui.widgetForImage->addRectangel(markupObjects_[activMarkupObject_].position);
            parsLineToOjectParams(line, markupObjects_[activMarkupObject_]);
        }
        if (markupObjects_.size() > 0)
        {
            ui.PB_deleteRectangel->setEnabled(true);
            markupObjects_[activMarkupObject_].setActiv(true);
            ui.widgetForImage->setActivFigure(activMarkupObject_);
        }
        fileWithMarks.close();
    }
}

void QtWidgetForMarkingImage::slot_changeSaveNameRB()
{
    if (ui.RB_originalName->isChecked())
    {
        ui.CB_customName->setEnabled(false);
        ui.CB_sequence->setEnabled(false);
        ui.LE_customName->setEnabled(false);
        ui.LE_sequenceStart->setEnabled(false);
        ui.label_startSequence->setEnabled(false);
        ui.label_curentValue->setEnabled(false);
        ui.LE_curentValue->setEnabled(false);
        setCustomName = false;
    }
    else
    {
        ui.CB_customName->setEnabled(true);
        ui.CB_sequence->setEnabled(true);
        ui.LE_customName->setEnabled(true);
        ui.LE_sequenceStart->setEnabled(true);
        ui.label_startSequence->setEnabled(true);
        ui.label_curentValue->setEnabled(true);
        ui.LE_curentValue->setEnabled(true);
        setCustomName = true;
        sequenceIterator = ui.LE_sequenceStart->text().toInt();
    }
}

void QtWidgetForMarkingImage::slot_activateImage(int i)
{
    setActivImage(i);
}

void QtWidgetForMarkingImage::slot_addMarkupObject()
{
    if (activMarkupObject_ != -1)
        markupObjects_[activMarkupObject_].setActiv(false);
    activMarkupObject_ = markupObjects_.size();
    markupObjects_.push_back(MarkupObject());
    markupObjects_[activMarkupObject_].setPenScale(activPenScale);
    
    markupObjects_[activMarkupObject_].setClass(ui.cb_classes->currentIndex());
    markupObjects_[activMarkupObject_].setType(ui.cb_types->currentIndex());
    markupObjects_[activMarkupObject_].setActiv(true);

    ui.widgetForImage->addRectangel(markupObjects_[activMarkupObject_].position);
    ui.widgetForImage->setActivFigure(activMarkupObject_);

    if (activMarkupObject_ == 0)
        ui.PB_deleteRectangel->setEnabled(true);
}

void QtWidgetForMarkingImage::slot_markingAndSaveImage()
{
    saveMarking();
    if (activImageId + 1 != quantityImage)
        slot_nextImage();
}

void QtWidgetForMarkingImage::slot_chooseSaveDirectoryForImage()
{
    QFileDialog fd{};
    QString directoryName{ fd.getExistingDirectory(this, "Marked images", "") };
    ui.le_saveDirectoryImage->setText(directoryName);
    if(ui.le_saveDirectoryLabel->text().size() ==0 )
        ui.le_saveDirectoryLabel->setText(directoryName);
}

void QtWidgetForMarkingImage::slot_chooseSaveDirectoryForLabel()
{
    QFileDialog fd{};
    QString directoryName{ fd.getExistingDirectory(this, "Image's label", "") };
    ui.le_saveDirectoryLabel->setText(directoryName);
}

void QtWidgetForMarkingImage::slot_changeStartIterator(QString start)
{
    sequenceIterator = start.toInt();
}

void QtWidgetForMarkingImage::slot_setSaveDirectoryForImage(QString directory)
{
    saveDirektoryForImage = directory;
}

void QtWidgetForMarkingImage::slot_setSaveDirectoryForLabel(QString directory)
{
    saveDirektoryForLabel = directory;
}

void QtWidgetForMarkingImage::slot_delRect()
{
    markupObjects_[activMarkupObject_].position->Delete();
    markupObjects_.erase(markupObjects_.begin() + activMarkupObject_);
    ui.widgetForImage->deleteFigure(activMarkupObject_);

    if (activMarkupObject_ == markupObjects_.size() && activMarkupObject_ > 0)
    {
        --activMarkupObject_;
        ui.widgetForImage->setActivFigure(activMarkupObject_);
    }
   
    if (markupObjects_.size() == 0)
    {
        ui.PB_deleteRectangel->setDisabled(true);
        activMarkupObject_ = -1;
        ui.cb_classes->setCurrentIndex(0);
    }

    if (activMarkupObject_ >= 0)
    {
        ui.cb_classes->setCurrentIndex(markupObjects_[activMarkupObject_].getClass());
        ui.cb_types->setCurrentIndex(markupObjects_[activMarkupObject_].getType());
    }
    ui.widgetForImage->updateImage();
}

void QtWidgetForMarkingImage::slot_changeClassLabel()
{
    if (activMarkupObject_ != -1)
    {
        markupObjects_[activMarkupObject_].setClass(ui.cb_classes->currentIndex());
        markupObjects_[activMarkupObject_].setType(0);
        ui.widgetForImage->updateImage();
    }
}

void QtWidgetForMarkingImage::slot_changeImageSize()
{
    resizeActivImage();
}

void QtWidgetForMarkingImage::slot_setActivMarkupObject(int const index)
{
    if (index < markupObjects_.size())
    {
        markupObjects_[activMarkupObject_].setActiv(false);
        activMarkupObject_ = index;
        ui.widgetForImage->setActivFigure(index);
        ui.cb_classes->setCurrentIndex(markupObjects_[index].getClass());
        ui.cb_types->setCurrentIndex(markupObjects_[index].getType());
        markupObjects_[activMarkupObject_].setActiv(true);
        ui.widgetForImage->updateImage();
    }
}

void QtWidgetForMarkingImage::slot_changeTypes(const QString& className)
{
    ui.cb_types->clear();
    for (size_t i{}; i < typeClasses[className.toLocal8Bit().toStdString()].size(); ++i)
    {
        ui.cb_types->addItem(QString::fromLocal8Bit(typeClasses[className.toLocal8Bit().toStdString()][i].c_str()));
    }
}

void QtWidgetForMarkingImage::slot_changeTypeLabel()
{
    if (activMarkupObject_ != -1)
    {
        markupObjects_[activMarkupObject_].setType(ui.cb_types->currentIndex());
    }
}

void QtWidgetForMarkingImage::slot_updateScale(float newScale)
{
    activPenScale = newScale;
    for (auto &obj : markupObjects_)
    {
        obj.setPenScale(activPenScale);
    }
}
