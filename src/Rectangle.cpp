#include "Rectangle.h"
#include <cmath>

Rectangle::Rectangle(const point &upperleft, const point &bottomright) {
	this->upperleft = upperleft;
	this->bottomright = bottomright;
}

Rectangle::Rectangle(const int &ulx, const int &uly, const int &brx, const int &bry) {
	this->setUpperLeft(ulx, uly);
	this->setBottomRight(brx, bry);
}

Rectangle::Rectangle() {}

int Rectangle::getWidth() {
	return this->bottomright.x- this->upperleft.x;
}

int Rectangle::getHeight() {
	return this->upperleft.y - this->bottomright.y;
}

void Rectangle::setUpperLeft(const int &ulx, const int &uly) {
	this->upperleft.x = ulx;
	this->upperleft.y = uly;
}

void Rectangle::setBottomRight(const int &brx, const int &bry) {
	this->bottomright.x = brx;
	this->bottomright.y = bry;
}

point Rectangle::getUpperLeft() {
	return upperleft;
}

point Rectangle::getBottomRight() {
	return bottomright;
}

Rectangle Rectangle::getRectangle() {
	return *this;
}

Rectangle& Rectangle::operator = (const Rectangle &rectangle) {
	upperleft = rectangle.upperleft;
	bottomright = rectangle.bottomright;
	return *this;
}

bool Rectangle::intersects(const Rectangle &r) { //to do !!!
	point ul1 = this->getUpperLeft();
	point br1 = this->getBottomRight();
	point ul2 = r.upperleft;
	point br2 = r.bottomright;

	if (floor(br1.x) >= ul2.x-eps && floor(br1.x) <= ul2.x+eps && ((br1.y >= br2.y-eps && br1.y <= ul2.y+eps) || (ul1.y >= br2.y-eps && ul1.y <= ul2.y+eps))) {return true;}
	else if (ul1.x <= br2.x+eps && ul1.x >= br2.x-eps && ((br1.y >= br2.y-eps && br1.y <= ul2.y+eps) || (ul1.y >= br2.y-eps && ul1.y <= ul2.y+eps))) {return true;}
	else return false;
}
