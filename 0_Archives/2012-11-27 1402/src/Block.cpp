#include "Block.h"
#include <iostream>
using namespace std;

Block::Block(const point &upperleft, const point &bottomright, const int &lives, const int &type) : Rectangle(upperleft, bottomright) {
	this->lives = lives;
	this->type = type;
	this->maxlives = lives;
}

bool Block::hit(const int &power) {
	this->lives -= power;
	if (this->lives > 0) return false;
	else return true; //do wyrzucenia z vectora
}

void Block::print() {
	cout<<"Block: "<<upperleft.x<<" "<<upperleft.y<<"\t"<<bottomright.x<<" "<<bottomright.y<<"\n";
}

int Block::get_type() {
	return this->type;
}
int Block::get_lives() {
	return this->lives;
}
int Block::get_maxlives() {
	return this->maxlives;
}

