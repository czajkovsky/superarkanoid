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

point intersects(circle c, rectangle r);
bool intersects(rectangle r1, rectangle r2);
void move(circle &c, vec v, double speed);
line make_line(vec v);
line perpendicular(line l, point p);
line parallel(line l, point p);
vec reflection(vec v, vec v1);
int rec_width(rectangle r);
int rec_height(rectangle r);


#endif
