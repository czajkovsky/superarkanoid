#ifndef BORDER_H
#define BORDER_H

#include "geo.h"

class Border {
	public:
		Border(const rectangle &rec, const bool &murderous);
		rectangle return_rectangle();
		bool isMurderous();
	private:
		rectangle rec;
		bool murderous;
};

#endif
