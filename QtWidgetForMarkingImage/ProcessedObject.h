#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "qpixmap.h"
#include "QtProcessedArea.h"
#include "qfiledialog.h"

#ifndef  processedObject_h
#define  processedObject_h

class ProcessedObject
{
protected:
	std::vector<QtProcessedArea> processedArears_;

public:
	ProcessedObject();
	ProcessedObject(bool imageIsNull);
	ProcessedObject(const ProcessedObject& dep);
	void setProcessedArears(std::vector<QtProcessedArea> *newProcessedAreas);
	std::vector<QtProcessedArea>* getProcesArears();
	ProcessedObject& operator=(const ProcessedObject& drop);
	ProcessedObject& operator=(ProcessedObject&& drop);

};

#endif