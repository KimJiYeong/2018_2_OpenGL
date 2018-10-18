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

#define PT 20//���� ���� ����
#define PI 3.141592 //����



//�׸��� ����
GLvoid drawScene(GLvoid);
BOOL draw_Act_On;

//Ÿ�̸� ����
void Timerfunction(int value);
BOOL Time_Act_On; //Ÿ�̸� Ȱ��ȭ ����
int TIme_count;

int move_count; //Ÿ�̸� ī��Ʈ
int move_count2; //Ÿ�̸� ī��Ʈ
int move_count3; //Ÿ�̸� ī��Ʈ
int move_count4; //Ÿ�̸� ī��Ʈ

				 //���콺 ����
void Mouse(int button, int state, int x, int y);
POINT ms_click; //���콺 ��ǥ
BOOL Mouse_Act_On; //���콺 Ȱ��ȭ ����

				   //Ű���� ����
void Keyboard(unsigned char key, int x, int y);
BOOL Keyboard_Act_On; //Ű���� Ȱ��ȭ ����

					  //�� ����
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

#define PT 36//���� ���� ����
#define PI 3.141592 //����
typedef struct Shape
{
	Color cl;//����
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




//-----��İ���
int Matrix_O(int sel, int num_0_0, int num_0_1, int num_0_2, int num_1_0, int num_1_1, int num_1_2, int num_2_0, int num_2_1, int num_2_2) {//3X3 ��� ���
	int m0[3];//i , j , k��
	int m1[3];//ù��° ���� ��
	int m2[3];//2��
	int result;
	//0�� ����
	m0[0] = num_0_0;
	m0[1] = num_0_1;
	m0[2] = num_0_2;
	//1�� ����
	m1[0] = num_1_0;
	m1[1] = num_1_1;
	m1[2] = num_1_2;
	//2�� ����
	m2[0] = num_2_0;
	m2[1] = num_2_1;
	m2[2] = num_2_2;

	//�� ���ϱ�
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

