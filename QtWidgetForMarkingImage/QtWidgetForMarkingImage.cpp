#include "QtWidgetForMarkingImage.h"

QtWidgetForMarkingImage::QtWidgetForMarkingImage(QWidget *parent)
    : QWidget(parent),
    setCustomSize{false},
    setCustomName{false}
{
    ui.setupUi(this);
    
    ui.spinBox_classLabel->setMaximum(3 * classInOneColor);
    labelColor = setClassColor();

    connect(ui.PB_chooseImage, SIGNAL(clicked()), this, SLOT(slot_chooseImageName()));
    connect(ui.PB_previousImage, SIGNAL(clicked()), this, SLOT(slot_previousImage()));
    connect(ui.PB_nextImage, SIGNAL(clicked()), this, SLOT(slot_nextImage()));

    connect(ui.spinBox_classLabel, SIGNAL(valueChanged(int)), this, SLOT(slot_setNewClassLabel(int)));
    connect(ui.PB_changeClassLabel, SIGNAL(clicked()), this, SLOT(slot_changeClassLabel()));
    connect(ui.PB_addRectangel, SIGNAL(clicked()), this, SLOT(slot_addRect()));
    connect(ui.PB_deleteRectangel, SIGNAL(clicked()), this, SLOT(slot_delRect()));
    connect(ui.PB_markingImage, SIGNAL(clicked()), this, SLOT(slot_markingAndSaveImage()));

    connect(ui.RB_customSize, SIGNAL(clicked()), this, SLOT(slot_changeSizeRB()));
    connect(ui.RB_originalSize, SIGNAL(clicked()), this, SLOT(slot_changeSizeRB()));
    connect(ui.LE_newHeigth, SIGNAL(textChanged(QString)), this, SLOT(slot_changeImageSize()));
    connect(ui.LE_newWidth, SIGNAL(textChanged(QString)), this, SLOT(slot_changeImageSize()));

    connect(ui.RB_originalName, SIGNAL(clicked()), this, SLOT(slot_changeSaveNameRB()));
    connect(ui.RB_customName, SIGNAL(clicked()), this, SLOT(slot_changeSaveNameRB()));
    connect(ui.LE_saveDirectory, SIGNAL(textChanged(QString)), this, SLOT(slot_setSaveDirectory(QString)));
    connect(ui.LE_sequenceStart, SIGNAL(textChanged(QString)), this, SLOT(slot_changeStartIterator(QString)));
    connect(ui.PB_setSaveDirectory, SIGNAL(clicked()), this, SLOT(slot_chooseSaveDirectory()));
}

QtWidgetForMarkingImage::~QtWidgetForMarkingImage()
{
    delete[] newLabel;
}

void QtWidgetForMarkingImage::slot_chooseImageName()
{
    QFileDialog fd{};
    QStringList imageNames{ fd.getOpenFileNames(this, "Images", "", tr("Images files (*.png *.jpg)")) };
    initScrollArea_withImageName(imageNames);
}

void QtWidgetForMarkingImage::initScrollArea_withImageName(QStringList& const imageNames)
{
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
    if (quantityImage > 0)
    {
        newLabel[activImageId].setStyleSheet("");
        newLabel[newActivImageId].setStyleSheet("border : 1px solid red;");
        activImageId = newActivImageId;
        QString dirictoriName{ newLabel[newActivImageId].getImageName() };
        cv::Mat buferImage{ cv::imread(dirictoriName.toStdString()) };
        if (!buferImage.empty())
        {
            buferImage.copyTo(activImage);
            if (setCustomSize)
                resizeActivImage();
            ui.widgetForImage->setNewImage(activImage);
            ui.widgetForImage->updateImg();
        }
    }
}

void QtWidgetForMarkingImage::resizeActivImage()
{
    if (ui.LE_newHeigth->text() != "" && ui.LE_newWidth->text() != "")
    {
        QString newHeigth{ ui.LE_newHeigth->text() };
        QString newWidth{ ui.LE_newWidth->text() };
        cv::resize(activImage, activImage, cv::Size(newWidth.toInt(), newHeigth.toInt()));
    }
}

QColor QtWidgetForMarkingImage::setClassColor()
{
    int r{ 0 };
    int g{ 0 };
    int b{ 0 };
    int classsLabel{ ui.spinBox_classLabel->value() };
    if (classsLabel / classInOneColor == 0)
    {
        r = (classsLabel ) * stepColor;
    }
    else if (classsLabel / classInOneColor == 1)
    {
        r = 125;
        g = (classsLabel - classInOneColor) * stepColor;
    }
    else if (classsLabel / classInOneColor >= 2)
    {
        r = 125;
        b = (classsLabel - classInOneColor * 2) * stepColor;
    }    
    return QColor(r, g, b);
}

void QtWidgetForMarkingImage::saveMarking()
{
    std::string saveName{ setSaveName() };
    std::ofstream objectCoordinate(saveName + ".txt", std::ios_base::out | std::ios_base::trunc);
    std::vector<QRect> limitRect;
    std::vector<int> classLabel;
    ui.widgetForImage->getClsaaRectangelAndLabel(limitRect, classLabel);
    for (size_t i{ 0 }; i < limitRect.size(); ++i)
    {
        objectCoordinate << classLabel[i] << " " << static_cast<float>(limitRect[i].center().x()) << " " << static_cast<float>(limitRect[i].center().y()) 
            << " " << static_cast<float>(limitRect[i].width()) << " " << static_cast<float>(limitRect[i].height()) << std::endl;
    }
    cv::imwrite(saveName + ".jpg", activImage);
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

std::string QtWidgetForMarkingImage::setSaveName()
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
    if(saveDirektory!="")
        saveName = saveDirektory + QString("/") + saveName;
    return saveName.toStdString();
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

void QtWidgetForMarkingImage::slot_addRect()
{
    ui.widgetForImage->add_rect(ui.spinBox_classLabel->value(), &labelColor);
}

void QtWidgetForMarkingImage::slot_markingAndSaveImage()
{
    saveMarking();
    if (activImageId + 1 != quantityImage)
        slot_nextImage();
}

void QtWidgetForMarkingImage::slot_chooseSaveDirectory()
{
    QFileDialog fd{};
    QString directoryName{ fd.getExistingDirectory(this, "Marked dataset", "") };
    ui.LE_saveDirectory->setText(directoryName);
}

void QtWidgetForMarkingImage::slot_changeStartIterator(QString start)
{
    sequenceIterator = start.toInt();
}

void QtWidgetForMarkingImage::slot_setSaveDirectory(QString directory)
{
    saveDirektory = directory;
}

void QtWidgetForMarkingImage::slot_delRect()
{
    ui.widgetForImage->deletActivRectangel();
}

void QtWidgetForMarkingImage::slot_changeClassLabel()
{
    ui.widgetForImage->setNewClassLabel(ui.spinBox_classLabel->value(), &labelColor);
}

void QtWidgetForMarkingImage::slot_setNewClassLabel(int value)
{
    labelColor = setClassColor();
}

void QtWidgetForMarkingImage::slot_changeImageSize()
{
    resizeActivImage();
}