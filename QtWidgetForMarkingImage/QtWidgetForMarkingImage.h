#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

#include <QtWidgets/QWidget>
#include "Display/QtGuiDisplay.h"
#include "MarkupObject.h"
#include "ui_QtWidgetForMarkingImage.h"

#include "QtWidgetsImageName.h"
#include "Classifier/Classifier.h"
#include"Loger/simplLoger.h"

class QtWidgetForMarkingImage : public QWidget
{
    Q_OBJECT
    
    QtWidgetsImageName* newLabel{};
    int activImageId{};
    int quantityImage{};
    Frame activImage_{};
    bool setCustomSize{};
    bool setCustomName{};
    float activPenScale{ 1.0 };

    QString saveDirektoryForImage{};
    QString saveDirektoryForLabel{};
    int sequenceIterator{ 0 };
    std::vector<MarkupObject> markupObjects_;
    int activMarkupObject_{ -1 };

    Classifier classLabels{};
    std::map<std::string, Classifier> typeClasses;

public:
    QtWidgetForMarkingImage(QWidget *parent = Q_NULLPTR);
    ~QtWidgetForMarkingImage();

private:
    Ui::QtWidgetForMarkingImageClass ui;
    void initScrollArea_withImageName(QStringList& const imageNames);
    void setActivImage(int const newActivImageId);
    void resizeActivImage();
    void saveMarking();
    std::string setSaveNameForImage();
    std::string setSaveNameForLabel(const std::string& imageName);
    void loadClassifirs();
    void loadMarking(const std::string& fileName);
    void parsLineToOjectParams(const std::string& line, MarkupObject& object);
    void convertBboxToAbsluteCoordinate(cv::Rect2f& inputRect, const cv::Size2i imageSize);

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
    void slot_setSaveDirectoryForImage(QString directory);
    void slot_setSaveDirectoryForLabel(QString directory);
    void slot_chooseSaveDirectoryForImage();
    void slot_chooseSaveDirectoryForLabel();
    void slot_changeStartIterator(QString start);
    void slot_changeImageSize();
    void slot_setActivMarkupObject(int const index);
    void slot_changeTypes(const QString& className);
    void slot_changeTypeLabel();
    void slot_updateScale(float newScale);
};
