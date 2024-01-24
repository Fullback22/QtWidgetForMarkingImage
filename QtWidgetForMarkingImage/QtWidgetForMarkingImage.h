#pragma once
#include <fstream>
#include <iostream>

#include <QtWidgets/QWidget>
#include "Display/QtGuiDisplay.h"
#include "MarkupObject.h"
#include "ui_QtWidgetForMarkingImage.h"

#include "QtWidgetsImageName.h"

class QtWidgetForMarkingImage : public QWidget
{
    Q_OBJECT
    
    QtWidgetsImageName* newLabel{};
    int activImageId{};
    int quantityImage{};
    Frame activImage_{};
    bool setCustomSize{};
    bool setCustomName{};

    QString saveDirektory{"dataSet"};
    int sequenceIterator{ 0 };
    std::vector<MarkupObject> markupObjects_;
    int activMarkupObject_{ -1 };

public:
    QtWidgetForMarkingImage(QWidget *parent = Q_NULLPTR);
    ~QtWidgetForMarkingImage();

private:
    Ui::QtWidgetForMarkingImageClass ui;
    void initScrollArea_withImageName(QStringList& const imageNames);
    void setActivImage(int const newActivImageId);
    void resizeActivImage();
    void saveMarking();
    std::string setSaveName();
public slots:
    void slot_chooseImageName();
    void slot_previousImage();
    void slot_nextImage();
    void slot_changeSizeRB();
    void slot_changeSaveNameRB();
    void slot_activateImage(int i);
    void slot_addMarkupObject();
    void slot_delRect();
    void slot_changeClassLabel();
    void slot_markingAndSaveImage();
    void slot_setSaveDirectory(QString directory);
    void slot_chooseSaveDirectory();
    void slot_changeStartIterator(QString start);
    void slot_changeImageSize();
    void slot_setActivMarkupObject(int const index);
};
