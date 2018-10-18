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



//그리기 제어
GLvoid drawScene(GLvoid);
BOOL draw_Act_On;

//타이머 제어
void Timerfunction(int value);
BOOL Time_Act_On; //타이머 활성화 여부
int TIme_count;

int move_count; //타이머 카운트
int move_count2; //타이머 카운트
int move_count3; //타이머 카운트
int move_count4; //타이머 카운트

				 //마우스 제어
void Mouse(int button, int state, int x, int y);
POINT ms_click; //마우스 좌표
BOOL Mouse_Act_On; //마우스 활성화 여부

				   //키보드 제어
void Keyboard(unsigned char key, int x, int y);
BOOL Keyboard_Act_On; //키보드 활성화 여부

					  //색 선정
typedef struct Color
{
	int R;
	int G;
	int B;
};
typedef struct Translate_pos {
	double x = 0;
	double y = 0;
	double z = 0;
	float degree = 0;
};

#define PT 36//도형 갯수 설정
#define PI 3.141592 //파이
typedef struct Shape
{
	Color cl;//색상
	Translate_pos pos;
	Translate_pos move;
	Translate_pos rot;
	Translate_pos sa;
	int size;
	BOOL select;
	int height;
	int slice;
	int stacks;

};

typedef struct Po
{
	float x;
	float y;
};


//GLdouble x_rotate[16] = {
//	1, 0, 0, 0,
//	0, cos(camera.degree * PI * 10 / 180),  -sin(camera.degree * PI * 10 / 180), 0,
//	0, sin(camera.degree * PI * 10 / 180), cos(camera.degree * PI * 10 / 180), 0,
//	0, 0, 0, 1
//};
//GLdouble y_rotate[16] = {
//	cos(camera.degree * PI * 10 / 180), 0, sin(camera.degree * PI * 10 / 180), 0,
//	0, 1,  0, 0,
//	-sin(camera.degree * PI * 10 / 180), 0,cos(camera.degree * PI * 10 / 180), 0,
//	0, 0, 0, 1
//};
//
//GLdouble z_rotate[16] = {
//	1, 0, 0, 0,
//	0, cos(camera.degree * PI * 10 / 180),  -sin(camera.degree * PI * 10 / 180), 0,
//	0, sin(camera.degree * PI * 10 / 180), cos(camera.degree * PI * 10 / 180), 0,
//	0, 0, 0, 1
//};




//-----행렬관련
int Matrix_O(int sel, int num_0_0, int num_0_1, int num_0_2, int num_1_0, int num_1_1, int num_1_2, int num_2_0, int num_2_1, int num_2_2) {//3X3 행렬 계산
	int m0[3];//i , j , k행
	int m1[3];//첫번째 원소 행
	int m2[3];//2행
	int result;
	//0행 대입
	m0[0] = num_0_0;
	m0[1] = num_0_1;
	m0[2] = num_0_2;
	//1행 대입
	m1[0] = num_1_0;
	m1[1] = num_1_1;
	m1[2] = num_1_2;
	//2행 대입
	m2[0] = num_2_0;
	m2[1] = num_2_1;
	m2[2] = num_2_2;

	//값 구하기
	if (sel == 0) {
		result = (m0[0] * ((m1[1] * m2[2]) - (m1[2] * m2[1])));
	}
	else if (sel == 1) {
		result = (m0[1] * ((m1[0] * m2[2]) - (m1[2] * m2[0])));
	}
	else if (sel == 2) {
		result = (m0[2] * ((m1[0] * m2[1]) - (m1[1] * m2[0])));
	}
	if (result < 0) {
		result = -1;
	}
	else if (result > 0) {
		result = 1;
	}

	return result;
}

