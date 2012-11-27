#ifndef GEO_H
#define GEO_H

struct point {
	double x,y;
};

struct vec {
	double x,y;
};

struct line {
	double A,B,C;
};

struct rectangle {
	point upperleft, bottomright;
};

struct circle {
	point center;
	double r;
};

point intersects(const circle &c, const rectangle &r);
bool intersects(const rectangle &r1, const rectangle &r2);
void move(circle &c, const vec &v, const double &speed);
line make_line(const vec &v);
line perpendicular(const line &l, const point &p);
line parallel(const line &l, const point &p);
vec reflection(const vec &v, const vec &v1);
int rec_width(const rectangle &r);
int rec_height(const rectangle &r);


#endif
