#ifndef BORDER_H
#define BORDER_H

#include "geo.h"
#include "Rectangle.h"

class Border : public Rectangle {
	public:
		Border(const point &upperleft, const point &bottomright, const bool &murderous);
		bool isMurderous();
	private:
		bool murderous;
};

#endif
