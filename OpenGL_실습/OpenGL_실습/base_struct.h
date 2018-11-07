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
	BOOL any;

};
