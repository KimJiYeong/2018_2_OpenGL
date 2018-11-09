#pragma once
#pragma once

typedef struct Color
{
	int R;
	int G;
	int B;
};
typedef struct Translate_pos {
	float x;
	float y;
	float z;
	int degree;
};

typedef struct BoOL
{
	int b_x;
	int b_y;
	int b_z;
};

typedef struct Cardinal_spline {
	float x;
	float y;
	float z;

};

typedef struct Shape
{
	Color cl;//»ö»ó
	Translate_pos pos;
	Translate_pos move;
	Translate_pos scale;
	Translate_pos rot;

	int size;
	int select;
	int height;
	int slice;
	int stacks;
	BoOL b;
	bool any;
	float t;
	Cardinal_spline spline[10];
};

struct Look_int
{
	int x;
	int y;
	int z;
	int degree;
};

