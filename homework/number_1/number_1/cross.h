#pragma once
#include <iostream>
#include <cmath>
using namespace std;

#define EPSILON 0.0001

struct vector2
{
	double x, y;

	vector2(){}
	vector2(double _x, double _y) {
		x = _x, y = _y;
	}
	//외적
	double cross(const vector2& other) const {
		return x * other.y - y * other.x;
	}

	//연산자 오버로딩을 통해 벡터의 연산 구하기
	vector2 operator * (double r) const {
		return vector2(x * r, y * r);
	}

	vector2 operator + (vector2 other) const {
		return vector2(x + other.x, y + other.y);
	}
	vector2 operator - (vector2 other) const {
		return vector2(x - other.x, y - other.y);
	}


	//벡터 비교
	bool operator == (vector2 other) const {
		return x == other.x && y == other.y;

	}
	bool operator < (vector2 other) const {
		return x < other.x && y < other.y;
	}
};

bool lineIntersection(vector2 a, vector2 b, vector2 c, vector2 d, vector2& x) {
	double det = (b - a).cross(d - c);

	if (fabs(det) < EPSILON) return false;
	x = a + (b - a) * ((c - a).cross(d - c) / det);
	return true;
}
double ccw(vector2 a, vector2 b) {
	return a.cross(b);
}
double ccw(vector2 p, vector2 a, vector2 b) {
	return ccw(a - p, b - p);

}
bool paralleSegments(vector2 a, vector2 b, vector2 c, vector2 d, vector2& p) {
	if (b < a) swap(a, b);
	if (d < c)swap(c, d);

	if (ccw(a, b, c) != 0 || b < c || d < a) return false;

	if (a < c) p = c;
	else p = a;
	return true;
}

bool inBoundingRectangle(vector2 p, vector2 a, vector2 b) {
	if (b < a)swap(a, b);
	return p == a || p == b || (a < p && p < b);

}
bool segmentIntersection(vector2 a, vector2 b, vector2 c, vector2 d, vector2& p) {
	if (!lineIntersection(a, b, c, d, p))
		return paralleSegments(a, b, c, d, p);
	return inBoundingRectangle(p, a, b) && inBoundingRectangle(p, c, d);
}

bool sementIntersects2(vector2 a, vector2 b, vector2 c, vector2 d) {
	double ab = ccw(a, b, c) * ccw(a, b, d);
	double cd = ccw(c, d, a) * ccw(c, d, b);

	if (ab == 0 && cd == 0) {
		if (b < a)swap(a, b);
		if (d < c)swap(c, d);
		return !(b < c || d < a);
	}
	return ab <= 0 && cd <= 0;
}




