#include "MarkupObject.h"

MarkupObject::MarkupObject()
{
	position = Rectangel(0, 0, 1, 1);
}

MarkupObject::~MarkupObject()
{

}

void MarkupObject::setClass(int const classIndex)
{
    objectClass_ = classIndex;
    int r{ 0 };
    int g{ 0 };
    int b{ 0 };
    int stepColor = 55;
    int classInOneColor = 255 / stepColor;
    if (objectClass_ / classInOneColor == 0)
    {
        r = (objectClass_)*stepColor;
    }
    else if (objectClass_ / classInOneColor == 1)
    {
        r = 125;
        g = (objectClass_ - classInOneColor) * stepColor;
    }
    else if (objectClass_ / classInOneColor >= 2)
    {
        r = 125;
        b = (objectClass_ - classInOneColor * 2) * stepColor;
    }
    pen_.setColor(QColor{ r,g,b });
    position->setPen(pen_);
}

int MarkupObject::getClass() const
{
	return objectClass_;
}

void MarkupObject::setType(int const typeIndex)
{
    objectType_ = typeIndex;
}

int MarkupObject::getType() const
{
    return objectType_;
}

void MarkupObject::setActiv(bool const isActiv)
{
    if (isActiv)
        pen_.setWidth(3 * penScale_);
    else
        pen_.setWidth(1 * penScale_);
    position->setPen(pen_);
}

void MarkupObject::setPenScale(const double scale)
{
    if (scale <= 1)
        penScale_ = 1.0 / scale;
    else
        penScale_ = 1;
    pen_.setWidth(pen_.width() * penScale_);
    return;
}
