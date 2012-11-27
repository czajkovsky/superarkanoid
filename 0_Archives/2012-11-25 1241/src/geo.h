#ifndef GEO_H
#define GEO_H

#define INF 1e9
#define eps 0.4

struct point {
	double x,y;
};

struct vec {
	double x,y;
};

struct line {
	double A,B,C;
};

line make_line(const vec &v);
line perpendicular(const line &l, const point &p);
line parallel(const line &l, const point &p);
vec reflection(const vec &v, const vec &v1);

#endif
