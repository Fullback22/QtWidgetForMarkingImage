#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <QObject>
#include <qrect.h>
#include "QtRotateRect.h"
#include "qimage.h"

class QtProcessedArea : public QObject
{
	Q_OBJECT
	QtRotateRect rect;
	bool activ;
	bool draw;
public:
	QtProcessedArea(QObject *parent=Q_NULLPTR );
	QtProcessedArea(QtRotateRect newRect, QObject* parent = Q_NULLPTR);

	QtProcessedArea(const QtProcessedArea& drop);
	QtProcessedArea(QtProcessedArea&& drop);
	~QtProcessedArea();
	void createMaster(cv::Mat const *inputImg);
	void setRect(QtRotateRect* newRect);
	cv::Mat getDrawImage(cv::Mat const* inputImg);


	void setActiv(bool isActiv);
	void setDraw(bool isDraw);
	bool isActiv();
	bool isDraw();

	double getArea(bool scaled=false);
	QtRotateRect* getRect();

	QtProcessedArea& operator=(const QtProcessedArea& drop);
	QRect getOriginalLimitRect();
	void updateProcessing(cv::Mat newOriginImeg);
	void setCenter(cv::Point const newCenter);
};
