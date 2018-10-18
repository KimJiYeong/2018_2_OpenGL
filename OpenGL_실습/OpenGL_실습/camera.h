#pragma once
#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>

typedef struct __Translate_pos {
	float x;
	float y;
	float z;
	float degree;
};

#define PT 20//도형 갯수 설정
#define PI 3.141592 //파이


static __Translate_pos camera;

GLdouble x_rotate[16] = {
	1, 0, 0, 0,
	0, cos(camera.degree * PI * 10 / 180),  -sin(camera.degree * PI * 10 / 180), 0,
	0, sin(camera.degree * PI * 10 / 180), cos(camera.degree * PI * 10 / 180), 0,
	0, 0, 0, 1
};
GLdouble y_rotate[16] = {
	cos(camera.degree * PI * 10 / 180), 0, sin(camera.degree * PI * 10 / 180), 0,
	0, 1,  0, 0,
	-sin(camera.degree * PI * 10 / 180), 0,cos(camera.degree * PI * 10 / 180), 0,
	0, 0, 0, 1
};

GLdouble z_rotate[16] = {
	1, 0, 0, 0,
	0, cos(camera.degree * PI * 10 / 180),  -sin(camera.degree * PI * 10 / 180), 0,
	0, sin(camera.degree * PI * 10 / 180), cos(camera.degree * PI * 10 / 180), 0,
	0, 0, 0, 1
};
