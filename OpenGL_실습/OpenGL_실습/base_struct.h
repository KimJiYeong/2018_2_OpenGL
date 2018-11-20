#pragma once
#include <GL/freeglut.h> 
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
	Color cl;//색상
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

};


typedef struct Custom_Light {
	GLfloat ambient[4];//앰비언트
	GLfloat diffuse[4];//디퓨즈
	GLfloat specular[4];//스펙큘러
	GLfloat pos[4];//위치

	bool turn;//조명 키기
};