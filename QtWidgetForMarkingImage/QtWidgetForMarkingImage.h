#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtWidgetForMarkingImage.h"

#include "QtWidgetsImageName.h"

class QtWidgetForMarkingImage : public QWidget
{
    Q_OBJECT
    
    QtWidgetsImageName* newLabel{};
    int activImageId{};
    int quantityImage{};
    cv::Mat activImage{};
    bool setCustomSize{};
    bool setCustomName{};
    
public:
    QtWidgetForMarkingImage(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtWidgetForMarkingImageClass ui;
    void initScrollArea_withImageName(QStringList& const imageNames);
    void setActivImage(int const newActivImageId);
    void resizeActivImage();
public slots:
    void slot_chooseImageName();
    void slot_previousImage();
    void slot_nextImage();
    void slot_changeSizeRB();
    void slot_changeSaveNameRB();
    void slot_activateImage(int i);
    void slot_addRect();
    void slot_changeClassLabel(int value);
};
