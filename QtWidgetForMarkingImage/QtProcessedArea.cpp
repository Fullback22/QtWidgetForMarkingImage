#include "QtProcessedArea.h"

QtProcessedArea::QtProcessedArea(QObject* parent)
	: QObject(parent),
	rect(QtRotateRect{}),
	classLabel{0},
	lineColor{nullptr},
	activ(false),
	draw(false)
{
	
}

QtProcessedArea::QtProcessedArea(QtRotateRect newRect, int const classLabel_, QColor const *color, QObject* parent)
	: QObject(parent),
	rect(newRect),
	classLabel{ classLabel_ },
	lineColor{ new QColor(*color) },
	activ(true),
	draw(true)
{

}

QtProcessedArea::QtProcessedArea(const QtProcessedArea& drop)
	:rect(drop.rect),
	activ(drop.activ),
	classLabel(drop.classLabel),
	lineColor(drop.lineColor),
	draw(drop.draw)
{
	
}

QtProcessedArea::QtProcessedArea(QtProcessedArea&& drop):
	rect(drop.rect),
	activ(drop.activ),
	classLabel(drop.classLabel),
	lineColor(drop.lineColor),
	draw(drop.draw)
{
}

QtProcessedArea::~QtProcessedArea()
{

}

void QtProcessedArea::createMaster(cv::Mat const* inputImg)
{
	/*cv::Rect roi(0, 0, 0, 0);
	if (areaType == 0)
	{
		roi = cv::Rect(rect.getRotateRectSize().x(), rect.getRotateRectSize().y(), rect.getRotateRectSize().width(), rect.getRotateRectSize().height());
	}*/
}

void QtProcessedArea::setRect(QtRotateRect* newRect)
{
	rect = *newRect;
}

cv::Mat QtProcessedArea::getDrawImage(cv::Mat const* inputImg)
{
	cv::Rect roi(0,0,0,0);
	cv::Mat mask(inputImg->size(), CV_8UC3,cv::Scalar(255,255,255));

	cv::Point vertices[4];
	vertices[0] = cv::Point(rect.getUpLeft_X() + 1, rect.getUpLeft_Y() + 1);
	vertices[1] = cv::Point(rect.getUpRigth_X(), rect.getUpRigth_Y() + 1);
	vertices[2] = cv::Point(rect.getDownRigth_X(), rect.getDownRigth_Y());
	vertices[3] = cv::Point(rect.getDownLeft_X() + 1, rect.getDownLeft_Y());
	cv::fillConvexPoly(mask, vertices, 4, cv::Scalar(0, 0, 0));
	roi=cv::Rect(rect.getRotateRectSize().x(), rect.getRotateRectSize().y(), rect.getRotateRectSize().width() , rect.getRotateRectSize().height() );

	
	mask = mask(roi);
	cv::Mat procesingImgPart(*inputImg);
	procesingImgPart = procesingImgPart(roi);
	
	cv::Mat backGround;
	if(inputImg->type()==CV_8U)
		cv::cvtColor(mask, mask, cv::COLOR_RGB2GRAY);
	cv::bitwise_and(cv::Mat(*inputImg,roi), mask, backGround);
	cv::bitwise_not(mask, mask);
	if (inputImg->type() == CV_8U)
		cv::cvtColor(mask, mask, cv::COLOR_GRAY2BGR);
	cv::bitwise_and(procesingImgPart, mask, procesingImgPart);

	cv::bitwise_or(procesingImgPart, backGround, procesingImgPart);
	return procesingImgPart;
}

void QtProcessedArea::setActiv(bool isActiv)
{
	activ = isActiv;
}

void QtProcessedArea::setDraw(bool isDraw)
{
	draw = isDraw;
}

bool QtProcessedArea::isActiv()
{
	return activ;
}

bool QtProcessedArea::isDraw()
{
	return draw;
}

int QtProcessedArea::getClassLabel() const
{
	return classLabel;
}

QColor QtProcessedArea::getColor() const
{
	return *lineColor;
}

void QtProcessedArea::setColor(QColor const* color) 
{
	delete lineColor;
	lineColor = new QColor(*color);
}

QtRotateRect* QtProcessedArea::getRect()
{
	return &rect;
}

QtProcessedArea& QtProcessedArea::operator=(const QtProcessedArea& drop)
{
	rect = drop.rect;
	activ = drop.activ;
	classLabel = drop.classLabel;
	lineColor = drop.lineColor;
	draw = drop.draw;
	return *this;
}

QRect QtProcessedArea::getOriginalLimitRect()
{
	return QRect(rect.getMin_X(), rect.getMin_Y(), rect.getMax_X() - rect.getMin_X() + 1, rect.getMax_Y() - rect.getMin_Y() + 1);
}

void QtProcessedArea::updateProcessing(cv::Mat newOriginImeg)
{
	cv::Rect roi(0, 0, 0, 0);
	roi = cv::Rect(rect.getRotateRectSize().x(), rect.getRotateRectSize().y(), rect.getRotateRectSize().width() , rect.getRotateRectSize().height() );
}

void QtProcessedArea::setCenter(cv::Point const newCenter)
{
	rect.setCenter(&QPoint(newCenter.x, newCenter.y));
}

double QtProcessedArea::getArea(bool scaled)
{
	return rect.height() * rect.width();
}

void QtProcessedArea::setClassLabel(int newClass)
{
	classLabel = newClass;
}
