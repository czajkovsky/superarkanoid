#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "geo.h"

class Rectangle {	
	public:
		Rectangle(point upperleft, point bottomright);
		Rectangle(int ulx, int uly, int brx, int bry);
		int getWidth();
		int getHeigth();
		void setUpperLeft(int ulx, int uly);
		void setBottomRight(int brx, int bry);
	protected:
		point upperleft, bottomright;
};

#endif
