#include "geo.h"
#include <iostream>
#include <cmath>
using namespace std;

line make_line(const vec &v) {
	line res;

	res.A = v.y;
	res.B = -v.x;
	res.C = 0;

	return res;
}

line perpendicular(const line &l, const point &p) {
	line res;

	res.A = -l.B;
	res.B = l.A;
	res.C = -res.A*p.x - res.B*p.y;

	return res;
}

line parallel(const line &l, const point &p) {
	line res;

	res.A = l.A;
	res.B = l.B;
	res.C = -res.A*p.x - res.B*p.y;

	return res;
}

inline double det(const double &A1, const double &B1, const double &A2, const double &B2) {
	return A1*B2 - B1*A2;
}

inline double is(const point &a, const point &b) {
	return a.x*b.x + a.y*b.y;
}

point intersect(const line &A, const line &B) {
	point res;

	double W = det(A.A, A.B, B.A, B.B);
	double Wx = det(-A.C, A.B, -B.C, B.B);
	double Wy = det(A.A, -A.C, B.A, -B.C);

	if (W != 0) {
		res.x = Wx/W;
		res.y = Wy/W;
	}
	else {
		res.x = res.y = INF;
	}
	
	return res;
}

point rzut(const line &l, const point &a) {
	line perp = perpendicular(l, a);
	point res = intersect(l, perp);
	return res;
}

vec reflection(const vec &v, const vec &v1) {
	vec res;
	
	point p;
	p.x = 0 - v.x;
	p.y = 0 - v.y;

	line r = make_line(v1);

	//cout<<"line r: "<<r.A<<" "<<r.B<<" "<<r.C<<"\n";

	point mid;
	mid.x = mid.y = 0;
	
	
	//line norm = perpendicular(r, mid);
	line par = perpendicular(r, p);
	//cout<<"line par: "<<par.A<<" "<<par.B<<" "<<par.C<<"\n";

	point inter = intersect(r, par);
	//cout<<"point inter: "<<inter.x<<" "<<inter.y<<"\n";

	if (inter.x != INF) {

		point dest;
		dest.x = inter.x + inter.x - p.x;
		dest.y = inter.y + inter.y - p.y;

		res.x = dest.x;
		res.y = dest.y;
		
	}

	else {
		res.x = -v.x;
		res.y = -v.y;
	}

	return res;
}
