#pragma once
#include <fstream>
#include <iostream>

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
    int stepColor{ 55 };
    int classInOneColor{ 255 / stepColor };
    QColor labelColor;
    QString saveDirektory{"dataSet"};
    int sequenceIterator{ 0 };
public:
    QtWidgetForMarkingImage(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtWidgetForMarkingImageClass ui;
    void initScrollArea_withImageName(QStringList& const imageNames);
    void setActivImage(int const newActivImageId);
    void resizeActivImage();
    QColor setClassColor();
    void saveMarking();
    std::string setSaveName();
public slots:
    void slot_chooseImageName();
    void slot_previousImage();
    void slot_nextImage();
    void slot_changeSizeRB();
    void slot_changeSaveNameRB();
    void slot_activateImage(int i);
    void slot_addRect();
    void slot_delRect();
    void slot_setNewClassLabel(int value);
    void slot_changeClassLabel();
    void slot_markingAndSaveImage();
    void slot_setSaveDirectory(QString directory);
    void slot_chooseSaveDirectory();
    void slot_changeStartIterator(QString start);
    void slot_changeImageSize();
};
