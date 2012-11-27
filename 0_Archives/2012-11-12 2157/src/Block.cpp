#include "Block.h"
#include <iostream>
using namespace std;

Block::Block(rectangle rec, int lives, int type) {
	this->rec = rec;
	this->lives = lives;
	this->type = type;
	this->maxlives = lives;
}

bool Block::hit(int power) {
	this->lives -= power;
	if (this->lives > 0) return false;
	else return true;//do wyrzucenia z vectora
}

void Block::print() {
	cout<<"Block: "<<(this->rec).upperleft.x<<" "<<(this->rec).upperleft.y<<"\t"<<(this->rec).bottomright.x<<" "<<(this->rec).bottomright.y<<"\n";
}

rectangle Block::return_rectangle() {
	rectangle res = this->rec;
	return res;
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

