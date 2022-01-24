#include "QtWidgetForMarkingImage.h"

QtWidgetForMarkingImage::QtWidgetForMarkingImage(QWidget *parent)
    : QWidget(parent),
    setCustomSize{false},
    setCustomName{false}
{
    ui.setupUi(this);
    
    connect(ui.PB_chooseImage, SIGNAL(clicked()), this, SLOT(slot_chooseImageName()));
    connect(ui.PB_previousImage, SIGNAL(clicked()), this, SLOT(slot_previousImage()));
    connect(ui.PB_nextImage, SIGNAL(clicked()), this, SLOT(slot_nextImage()));

    connect(ui.PB_addRectangel, SIGNAL(clicked()), this, SLOT(slot_addRect()));
    connect(ui.spinBox_classLabel, SIGNAL(valueChanged(int)), this, SLOT(slot_changeClassLabel(int)));

    connect(ui.RB_customSize, SIGNAL(clicked()), this, SLOT(slot_changeSizeRB()));
    connect(ui.RB_originalSize, SIGNAL(clicked()), this, SLOT(slot_changeSizeRB()));

    connect(ui.RB_originalName, SIGNAL(clicked()), this, SLOT(slot_changeSaveNameRB()));
    connect(ui.RB_customName, SIGNAL(clicked()), this, SLOT(slot_changeSaveNameRB()));
   
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
    QString newHeigth{ ui.LE_newHeigth->text() };
    QString newWidth{ ui.LE_newWidth->text() };
    cv::resize(activImage, activImage, cv::Size(newWidth.toInt(), newHeigth.toInt()));
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

void QtWidgetForMarkingImage::slot_changeSaveNameRB()
{
    if (ui.RB_originalName->isChecked())
    {
        ui.CB_customName->setEnabled(false);
        ui.CB_sequence->setEnabled(false);
        ui.LE_customName->setEnabled(false);
        ui.LE_sequenceStart->setEnabled(false);
        ui.label_startSequence->setEnabled(false);
        setCustomName = false;
    }
    else
    {
        ui.CB_customName->setEnabled(true);
        ui.CB_sequence->setEnabled(true);
        ui.LE_customName->setEnabled(true);
        ui.LE_sequenceStart->setEnabled(true);
        ui.label_startSequence->setEnabled(true);
        setCustomName = true;
    }
}

void QtWidgetForMarkingImage::slot_activateImage(int i)
{
    setActivImage(i);
}

void QtWidgetForMarkingImage::slot_addRect()
{
    ui.widgetForImage->add_rect();//,ui.spinBox_classLabel->value()+2);
}

void QtWidgetForMarkingImage::slot_changeClassLabel(int value)
{
    //ui.widgetForImage->change_rectColor(value + 2);
}
