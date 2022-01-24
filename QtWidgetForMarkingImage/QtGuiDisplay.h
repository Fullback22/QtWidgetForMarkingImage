#pragma once

#include <QWidget>
#include "ui_QtGuiDisplay.h"
#include "qfiledialog.h"
#include "myLabel.h"
#include "ProcessedObject.h"
//#include "QtAreaToProcessed.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>

class QtGuiDisplay : public QWidget
{
	Q_OBJECT
	cv::Mat tempImg;
	QPixmap TempPixmap;
	QCursor myCursor;
	ProcessedObject* activProcessedObj;

	int activ_roi;
	int activProcesArea;
	bool change_roi;
	bool event_img;
	bool activatedRoi;
	bool activ;

	bool changeActivArea;
	void resizeEvent(QResizeEvent* event);

	double activ_scaled;
	double hor_scaled;
	double vert_scaled;

	bool isProcessingActiv;
	bool isZoomNow;
	void setCursor_àorChangesTopBotton(double rotateAngel);
	void setCursor_àorChangesRightLeft(double rotateAngel);
	void setCursor_àorChangesTopRight_BottonLeft(double rotateAngel);
	void setCursor_àorChangesTopLeft_BottonRight(double rotateAngel);
public:
	QtGuiDisplay(QWidget *parent = Q_NULLPTR);
	~QtGuiDisplay();
	void setSizeScrollBar();

	void setEanbleActivededRoi(bool activ);
	void setActiv(bool activ);
	void setChangeActivArea(bool isChange);
	void setChangesProcessedArears(bool isChang);

	void draw_proceseArears();
	void add_rect(int classLabel, QColor const *color);
	void deletActivRectangel();
	QRect getLabelRect();


	bool isActiv();
	bool getChageActivArea();
	bool ProcessedIsActiv();



	void updateProcessObj(ProcessedObject& activObj);
	void setActivProcesArea(int activArea);

	void setNewImage(cv::Mat const inputImg);
	void setNewClassLabel(int const classLabel, QColor const* color);
	void getClsaaRectangelAndLabel(std::vector<QRect>& rectangel, std::vector<int>& label);
private:
	Ui::QtGuiDisplay ui;
public slots:
	void slot_mouvePixmap();
	void slot_mouseCurrentPos();
	void slot_mousePressed();
	void slot_mouseLeft();
	void slot_mouseLeft(int direct);
	void slot_mouseRelease();
	void slot_ZoomImg_In();
	void slot_ZoomImg_Out();
	void slot_ZoomImg_AllLabl();

	void updateImg();
	//void slot_changeProcssActiv(int isActiv); //0-disenable 1-enable
	
signals:
	//void clic_pb();
	void changeActivProcesArea(int newActiv);
	void getActivProcessArea();
};
