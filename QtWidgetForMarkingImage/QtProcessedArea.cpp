#include "QtProcessedArea.h"

QtProcessedArea::QtProcessedArea(QObject* parent)
	: QObject(parent),
	processedAreaType(0),
	areaType(0),
	rect(QtRotateRect{}),
	activ(false),
	draw(false),
	systemName(QString{}),
	userName(QString{}),
	doubleTreshF(0),
	doubleTreshS(0),
	singlTresActiv(false)
	//counterProc(nullptr)
{
	
}

QtProcessedArea::QtProcessedArea(int processedType, int areaType_, QtRotateRect newRect, QObject* parent)
	: QObject(parent),
	processedAreaType(processedType),
	areaType(areaType_),
	rect(newRect),
	activ(true),
	draw(true),
	systemName(QString{}),
	userName(QString{}),
	doubleTreshF(0),
	doubleTreshS(0),
	singlTresActiv(false)
	
	//counterProc(nullptr)
{
	if (processedType == 1)
	{
		
	}
}

QtProcessedArea::QtProcessedArea(const QtProcessedArea& drop)
	:processedAreaType(drop.processedAreaType),
	areaType(drop.areaType),
	rect(drop.rect),
	activ(drop.activ),
	draw(drop.draw),
	systemName(drop.systemName),
	userName(drop.userName),
	doubleTreshF(drop.doubleTreshF),
	doubleTreshS(drop.doubleTreshS),
	singlTresActiv(drop.singlTresActiv)
{
	
}

QtProcessedArea::QtProcessedArea(QtProcessedArea&& drop):
	processedAreaType(drop.processedAreaType),
	areaType(drop.areaType),
	rect(drop.rect),
	activ(drop.activ),
	draw(drop.draw),
	systemName(drop.systemName),
	userName(drop.userName),
	doubleTreshF(drop.doubleTreshF),
	doubleTreshS(drop.doubleTreshS),
	singlTresActiv(drop.singlTresActiv)
{
}

QtProcessedArea::~QtProcessedArea()
{

}

void QtProcessedArea::createMaster(cv::Mat const* inputImg)
{
	cv::Rect roi(0, 0, 0, 0);
	if (areaType == 0)
	{
		roi = cv::Rect(rect.getRotateRectSize().x(), rect.getRotateRectSize().y(), rect.getRotateRectSize().width(), rect.getRotateRectSize().height());
	}
}

void QtProcessedArea::setRect(QtRotateRect* newRect)
{
	rect = *newRect;
}

//void QtProcessedArea::setScalRect(QtRotateRect* newRect)
//{
//	scalRect = *newRect;
//}

cv::Mat QtProcessedArea::getDrawImage(cv::Mat const* inputImg)
{
	cv::Rect roi(0,0,0,0);
	cv::Mat mask(inputImg->size(), CV_8UC3,cv::Scalar(255,255,255));
	if (areaType == 0)
	{
		cv::Point vertices[4];
		vertices[0] = cv::Point(rect.getUpLeft_X() + 1, rect.getUpLeft_Y() + 1);
		vertices[1] = cv::Point(rect.getUpRigth_X(), rect.getUpRigth_Y() + 1);
		vertices[2] = cv::Point(rect.getDownRigth_X(), rect.getDownRigth_Y());
		vertices[3] = cv::Point(rect.getDownLeft_X() + 1, rect.getDownLeft_Y());
		cv::fillConvexPoly(mask, vertices, 4, cv::Scalar(0, 0, 0));
		roi=cv::Rect(rect.getRotateRectSize().x(), rect.getRotateRectSize().y(), rect.getRotateRectSize().width() , rect.getRotateRectSize().height() );
	}
	
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
	/*if (inputImg->type() == CV_8U)
		cv::cvtColor(backGround, backGround, cv::COLOR_BGR2GRAY);*/
	cv::bitwise_or(procesingImgPart, backGround, procesingImgPart);
	return procesingImgPart;
}

//void QtProcessedArea::setScalCircle(MyCircle* newCirkle)
//{
//	scalCircle = *newCirkle;
//}

void QtProcessedArea::setAreaType(int newType)
{
	areaType = newType;
}

void QtProcessedArea::setProcessedType(int newType)
{
	processedAreaType = newType;
}

void QtProcessedArea::setActiv(bool isActiv)
{
	activ = isActiv;
}

void QtProcessedArea::setDraw(bool isDraw)
{
	draw = isDraw;
}

void QtProcessedArea::setSystemName(QString newName)
{
	systemName = newName;
}

void QtProcessedArea::setActivTres(bool isSingel)
{
	singlTresActiv = isSingel;
}

void QtProcessedArea::setDoubelTreshF(int min)
{	
	doubleTreshF = min;
}

void QtProcessedArea::setDoubelTreshS(int max)
{

	doubleTreshS = max;
}

bool QtProcessedArea::singelIsActivTresh()
{
	return singlTresActiv;
}

int QtProcessedArea::getDoubelTresF()
{
	return doubleTreshF;
}

int QtProcessedArea::getDoubelTresS()
{
	return doubleTreshS;
}

QString QtProcessedArea::getSystemName()
{
	return systemName;
}

QString QtProcessedArea::getProcesName()
{
	if (processedAreaType == 1)
		return "Outline";
	else if (processedAreaType == 2)
		return "Color";
	else if (processedAreaType == 3)
		return "Edge Pixel";
	else if (processedAreaType == 4)
		return "Position Adjustment";
	else if (processedAreaType == 5)
		return "Width";
	else if (processedAreaType == 6)
		return "Diametr";
	else if (processedAreaType == 7)
		return "Edge";
	else if (processedAreaType == 8)
		return "Pitch";
	else if (processedAreaType == 9)
		return "High-Speed";
	else if (processedAreaType == 10)
		return "OCR";
	else
		return "";
}

void QtProcessedArea::setUserName(QString newName)
{
	userName = newName;
}

QString QtProcessedArea::getUserName()
{
	return userName;
}

bool QtProcessedArea::isActiv()
{
	return activ;
}

bool QtProcessedArea::isDraw()
{
	return draw;
}

int QtProcessedArea::getAreaType()
{
	return areaType;
}

int QtProcessedArea::getProcesseedType()
{
	return processedAreaType;
}

QtRotateRect* QtProcessedArea::getRect()
{
	return &rect;
}

//QtRotateRect* QtProcessedArea::getScalRect()
//{
//	return &scalRect;
//}


void QtProcessedArea::setProcessing(int typeProcessing)
{
	typeProcessing = processedAreaType;
}

QtProcessedArea& QtProcessedArea::operator=(const QtProcessedArea& drop)
{
	processedAreaType = drop.processedAreaType;
	areaType = drop.areaType;
	rect = drop.rect;
	activ = drop.activ;
	draw = drop.draw;
	systemName = drop.systemName;
	userName = drop.userName;
	doubleTreshF = drop.doubleTreshF;
	doubleTreshS = drop.doubleTreshS;
	singlTresActiv = drop.singlTresActiv;
	std::cout << "=" << std::endl;
	return *this;
}

QRect QtProcessedArea::getOriginalLimitRect()
{
	if (areaType == 0)
	{
		return QRect(rect.getMin_X(), rect.getMin_Y(), rect.getMax_X() - rect.getMin_X() + 1, rect.getMax_Y() - rect.getMin_Y() + 1);
	}
	
}

void QtProcessedArea::updateProcessing(cv::Mat newOriginImeg)
{
	cv::Rect roi(0, 0, 0, 0);
	if (areaType == 0)
	{
		roi = cv::Rect(rect.getRotateRectSize().x(), rect.getRotateRectSize().y(), rect.getRotateRectSize().width() , rect.getRotateRectSize().height() );
	}
}

void QtProcessedArea::getProcessedThreshold(std::vector<int>& threshold)
{
	
}

void QtProcessedArea::setProcessedThreshold(std::vector<int>& const threshold)
{
	
}

int QtProcessedArea::computeComparsion(cv::Mat* const masterImage)
{

	return 0;
}

cv::Mat* QtProcessedArea::getMasterImage()
{
	return &cv::Mat();
}

void QtProcessedArea::setCenter(cv::Point const newCenter)
{
	if (areaType == 0)
	{
		rect.setCenter(&QPoint(newCenter.x, newCenter.y));
	}
	
}

void QtProcessedArea::setRotateAngel(float const newRotateAngel)
{
	if (areaType == 0)
	{
		rect.setRotateAngel(newRotateAngel);
	}
}


double QtProcessedArea::getArea(bool scaled)
{
	if (areaType == 0)
		return rect.height() * rect.width();
}