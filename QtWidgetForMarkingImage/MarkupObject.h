#pragma once
#include "Figure/FigureRectangle.h"

class MarkupObject
{
	int objectClass_{};
	QPen pen_{ QColor(0,0,0), 1 };
public:
	
	FigureRectangle* position{};
	
	MarkupObject();
	~MarkupObject();

	void setClass(int const classIndex);
	int getClass() const;
	void setActiv(bool const isActiv);
};

