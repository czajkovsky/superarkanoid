#include "Border.h"

Border::Border(rectangle rec, bool murderous) {
	this->rec = rec;
	this->murderous = murderous;
}

rectangle Border::return_rectangle() {
	rectangle res = this->rec;
	return res;
}

bool Border::isMurderous() {
	bool res = this->murderous;
	return res;
}
