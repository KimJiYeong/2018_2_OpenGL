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
	Color cl;//����
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
	GLfloat ambient[4];//�ں��Ʈ
	GLfloat diffuse[4];//��ǻ��
	GLfloat specular[4];//����ŧ��
	GLfloat pos[4];//��ġ

	bool turn;//���� Ű��
};