#include "geo.h"
#include <iostream>
#include <cmath>
using namespace std;

#define INF 1e9
#define eps 0.4


point intersects(circle c, rectangle r) {
	point res;
	point mid = c.center;
	point ul = r.upperleft;
	point br = r.bottomright;
	double tmp;
	if ((tmp = mid.y+c.r) >= br.y-eps && tmp <= br.y+eps && mid.x <= br.x+eps && mid.x >= ul.x-eps) {res.x = mid.x; res.y = br.y;}
	else if ((tmp = mid.x+c.r) >= ul.x-eps && tmp <= ul.x+eps && mid.y <= ul.y+eps && mid.y >= br.y-eps) {res.x = ul.x; res.y = mid.y;}
	else if ((tmp = mid.y-c.r) <= ul.y+eps && tmp >= ul.y-eps && mid.x <= br.x+eps && mid.x >= ul.x-eps) {res.x = mid.x; res.y = ul.y;}
	else if ((tmp = mid.x-c.r) <= br.x+eps && tmp >= br.x-eps && mid.y <= ul.y+eps && mid.y >= br.y-eps) {res.x = br.x; res.y = mid.y;}

	else if ((tmp = sqrt((mid.x-br.x)*(mid.x-br.x) + (mid.y-ul.y)*(mid.y-ul.y))) /*>= c.r-eps && tmp*/ <= c.r+eps) {cout<<"1a\n"; res.x = br.x; res.y = ul.y;}
	else if ((tmp = sqrt((mid.x-br.x)*(mid.x-br.x) + (br.y-mid.y)*(br.y-mid.y))) /*>= c.r-eps && tmp*/ <= c.r+eps) {cout<<"2a\n"; res = br;}
	else if ((tmp = sqrt((ul.x-mid.x)*(ul.x-mid.x) + (mid.y-ul.y)*(mid.y-ul.y))) /*>= c.r-eps && tmp*/ <= c.r+eps) {cout<<"3a\n"; res = ul;}
	else if ((tmp = sqrt((ul.x-mid.x)*(ul.x-mid.x) + (br.y-mid.y)*(br.y-mid.y))) /*>= c.r-eps && tmp*/ <= c.r+eps) {cout<<"4a\n"; res.x = ul.x; res.y = br.y;}
	
	/* następne 4 linijki są źle !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/*	else if (mid.x >= br.x && mid.y >= ul.y && mid.x-c.r <= br.x && mid.y-c.r <= ul.y) {cout<<"1\n"; res.x = br.x; res.y = ul.y;}
	else if (mid.x >= br.x && mid.y <= br.y && mid.x-c.r <= br.x && mid.y+c.r >= br.y) {cout<<"2\n"; res = br;}
	else if (mid.x <= ul.x && mid.y >= ul.y && mid.x+c.r >= ul.x && mid.y-c.r <= ul.y) {cout<<"3\n"; res = ul;}
	else if (mid.x <= ul.x && mid.y <= br.y && mid.x+c.r >= ul.x && mid.y+c.r >= br.y) {cout<<"4\n"; res.x = ul.x; res.y = br.y;}
*/	
	/* ale już można je wyjebać :)))))))))))))))))))))) */

	else res.x = res.y = INF;

	return res;
}

bool intersects(rectangle r1, rectangle r2) { //to do !!!
	point ul1 = r1.upperleft;
	point br1 = r1.bottomright;
	point ul2 = r2.upperleft;
	point br2 = r2.bottomright;
	double tmp;

	if (floor(br1.x) >= ul2.x-eps && floor(br1.x) <= ul2.x+eps && ((br1.y >= br2.y-eps && br1.y <= ul2.y+eps) || (ul1.y >= br2.y-eps && ul1.y <= ul2.y+eps))) {return true;}
	else if (ul1.x <= br2.x+eps && ul1.x >= br2.x-eps && ((br1.y >= br2.y-eps && br1.y <= ul2.y+eps) || (ul1.y >= br2.y-eps && ul1.y <= ul2.y+eps))) {return true;}
	else return false;
}

void move(circle &c, vec v, double speed) {
	point p = c.center;
	p.x += speed*v.x;
	p.y += speed*v.y;
	c.center = p;
}

line make_line(vec v) {
	line res;

	res.A = v.y;
	res.B = -v.x;
	res.C = 0;

	return res;
}

line perpendicular(line l, point p) {
	line res;

	res.A = -l.B;
	res.B = l.A;
	res.C = -res.A*p.x - res.B*p.y;

	return res;
}

line parallel(line l, point p) {
	line res;

	res.A = l.A;
	res.B = l.B;
	res.C = -res.A*p.x - res.B*p.y;

	return res;
}

inline double det(double A1, double B1, double A2, double B2) {
	return A1*B2 - B1*A2;
}

inline double is(point a, point b) {
	return a.x*b.x + a.y*b.y;
}


point intersect(line A, line B) {
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

point rzut(line l, point a) {
	line perp = perpendicular(l, a);
	point res = intersect(l, perp);
	return res;
}

vec reflection(vec v, vec v1) {
	vec res;
	
	point p;
	p.x = 0 - v.x;
	p.y = 0 - v.y;

	line r = make_line(v1);

	cout<<"line r: "<<r.A<<" "<<r.B<<" "<<r.C<<"\n";

	point mid;
	mid.x = mid.y = 0;
	
	
//	line norm = perpendicular(r, mid);
	line par = perpendicular(r, p);
	cout<<"line par: "<<par.A<<" "<<par.B<<" "<<par.C<<"\n";

	point inter = intersect(r, par);
	cout<<"point inter: "<<inter.x<<" "<<inter.y<<"\n";

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

int rec_width(rectangle r) {
	return r.bottomright.x-r.upperleft.x;
}

int rec_height(rectangle r) {
	return r.upperleft.y-r.bottomright.y;
}
