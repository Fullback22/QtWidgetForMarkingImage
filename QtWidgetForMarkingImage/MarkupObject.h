#pragma once
#include "Figure/FigureRectangle.h"

class MarkupObject
{
	int objectClass_{};
	int objectType_{};
	QPen pen_{ QColor(0,0,0), 1 };
	double penScale_{ 1.0 };
public:
	
	FigureRectangle* position{};
	
	MarkupObject();
	~MarkupObject();

	void setClass(int const classIndex);
	int getClass() const;
	void setType(int const typeIndex);
	int getType() const;
	void setActiv(bool const isActiv);
	void setPenScale(const double scale);
};

