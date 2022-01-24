#include "ProcessedObject.h"

ProcessedObject::ProcessedObject():
	processedArears_{}
{
}

ProcessedObject::ProcessedObject(bool imageIsNull) :
	processedArears_{ QtProcessedArea(QtRotateRect(0,0,0,0,0)) }
{
}

ProcessedObject::ProcessedObject(const ProcessedObject& dep):
	processedArears_{dep.processedArears_ }
{
}

void ProcessedObject::setProcessedArears(std::vector<QtProcessedArea> *newProcessedAreas)
{
	processedArears_.assign(newProcessedAreas->begin(), newProcessedAreas->end());
}

std::vector<QtProcessedArea>* ProcessedObject::getProcesArears()
{
	return &processedArears_;
}

ProcessedObject& ProcessedObject::operator=(const ProcessedObject& drop)
{
	processedArears_.assign(drop.processedArears_.begin(), drop.processedArears_.end());
	return *this;
}

ProcessedObject& ProcessedObject::operator=(ProcessedObject&& drop)
{
	processedArears_.assign(drop.processedArears_.begin(), drop.processedArears_.end());
	drop.processedArears_.clear();
	return *this;
}

