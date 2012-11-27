#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "geo.h"

class Circle;

class Rectangle {	
	public:
		Rectangle(const point &upperleft, const point &bottomright);
		Rectangle(const int &ulx, const int &uly, const int &brx, const int &bry);
		Rectangle();
		int getWidth();
		int getHeight();
		void setUpperLeft(const int &ulx, const int &uly);
		void setBottomRight(const int &brx, const int &bry);
		point getUpperLeft();
		point getBottomRight();
		Rectangle getRectangle();
		Rectangle& operator = (const Rectangle &rectangle);
		bool intersects(const Rectangle &r);
		friend class Circle;
	protected:
		point upperleft, bottomright;
};

#endif
