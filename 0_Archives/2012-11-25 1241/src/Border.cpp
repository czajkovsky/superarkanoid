#include "Border.h"

Border::Border(const point &upperleft, const point &bottomright, const bool &murderous) : Rectangle(upperleft, bottomright) {
	this->murderous = murderous;
}

bool Border::isMurderous() {
	bool res = this->murderous;
	return res;
}
