#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include "ex9.h"

GLvoid Reshape(int w, int h);

//�ػ� ����
#define WideSize 800
#define HighSize 600

//�׸��� ����
GLvoid drawScene(GLvoid);
BOOL draw_Act_On;

//Ÿ�̸� ����
void Timerfunction(int value);
BOOL Time_Act_On; //Ÿ�̸� Ȱ��ȭ ����

				//���콺 ����
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
POINT ms_click; //���콺 ��ǥ
BOOL Mouse_Act_On; //���콺 Ȱ��ȭ ����
bool left_button;


//Ű���� ����
void Keyboard(unsigned char key, int x, int y);
BOOL Keyboard_Act_On; //Ű���� Ȱ��ȭ ����
					  //���� ����
typedef struct Color
{
	int R;
	int B;
	int G;
};

typedef struct Shape {
	POINT pt;//��ǥ
	int size;//������
	BOOL collide;//�浹
	Color cl;//�÷�
	
	//�ﰢ�� �ִϸ��̼� ����
	BOOL coll_Move;//���찳�� ó������ ������
	BOOL Shape_Tri;//����� �ﰢ��
	//��ٸ��� �ִϸ��̼� ����
	int tri2_pt;//��ٸ���
	int Time_count; //Ÿ�̸� ī��Ʈ

};

Shape back[30]; //��� �Լ�
Shape erase;//���찳 �Լ�



void main(int argc, char *argv[]) {
	//�ʱ�ȭ
	for (int i = 0; i < 30; i++) {
		//�簢�� �ʱ�ȭ �Լ�
		back[i].cl.R = rand() % 255;
		back[i].cl.G = rand() % 255;
		back[i].cl.B = rand() % 255;
		back[i].size = 15;
		back[i].tri2_pt = back[i].size; //��ٸ��� �����

		back[i].collide = FALSE;

		back[i].pt.x = rand() % WideSize - back[i].size;
		back[i].pt.y = rand() % HighSize - back[i].size;

	}
	erase.size = back[0].size * 2;
	erase.cl.R = 10;
	erase.cl.G = 20;
	erase.cl.B = 10;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);//������ ���� ��ǥ
	glutInitWindowSize(WideSize, HighSize); //������ ���� ũ��
	glutCreateWindow("ex10");
	// - �������� ���� ���� �����ϱ�
	//���� �׸���

	glutDisplayFunc(drawScene);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(100, Timerfunction, 1);
	glutReshapeFunc(Reshape);

	srand(time(NULL));
	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//���
	for (int i = 0; i < 30; i++) {
			glColor4f(
				(float)back[i].cl.R / 255,
				(float)back[i].cl.G / 255,
				(float)back[i].cl.B / 255, 1.0f
			);
			glBegin(GL_POLYGON);
			glVertex2i(back[i].pt.x - back[i].size, back[i].pt.y - back[i].size);
			glVertex2i(back[i].pt.x - back[i].tri2_pt, back[i].pt.y + back[i].size);
			glVertex2i(back[i].pt.x + back[i].tri2_pt, back[i].pt.y + back[i].size);
			glVertex2i(back[i].pt.x + back[i].size, back[i].pt.y - back[i].size);


			glEnd();

	}

	

	if (left_button) {
		glColor4f(
			(float)erase.cl.R / 255,
			(float)erase.cl.G / 255,
			(float)erase.cl.B / 255, 1.0f
		);
		glRectf(
			erase.pt.x - erase.size,
			erase.pt.y - erase.size,
			erase.pt.x + erase.size,
			erase.pt.y + erase.size
		);
		glEnd();
	}

	glFlush();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		left_button = true;

	}
	else
	{
		left_button = false;
	}

}
void Motion(int x, int y) {
	if (left_button == true) {
		erase.pt.x = x;
		erase.pt.y = y;
	}

	for (int i = 0; i < 30; i++) {
		
		if (!back[i].Shape_Tri) {
			//�ﰢ���� �� ���찳 �簢���� �浹üũ�� ���� �ʴ´�.
			if (erase.pt.x + erase.size > back[i].pt.x - back[i].size
				&& erase.pt.y + erase.size > back[i].pt.y - back[i].size
				&& erase.pt.x - erase.size < back[i].pt.x + back[i].size
				&& erase.pt.y - erase.size < back[i].pt.y + back[i].size
				) {
				back[i].collide = TRUE;
				back[i].Time_count = 0;
			}
		}

	}

}
void Timerfunction(int value) {
	//Ÿ�̸� ���� �Է�
	for (int i = 0; i < 30; i++)
	{
		back[i].Time_count++;

		if (back[i].collide) {//�浹�ϸ�
			if (back[i].tri2_pt > 0) {
				back[i].tri2_pt--;
				back[i].Shape_Tri = TRUE;
			}
			
		}
		else {//�浹 ��������
			back[i].Shape_Tri = FALSE;
			if (back[i].Time_count % 1 == 0) {
				back[i].tri2_pt++;
			}
		}
	}
	

	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.
	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'r'://������
			 //�ʱ�ȭ
		for (int i = 0; i < 30; i++) {

			//�簢�� �ʱ�ȭ �Լ�
			back[i].cl.R = rand() % 255;
			back[i].cl.G = rand() % 255;
			back[i].cl.B = rand() % 255;
			back[i].size = 10;

			back[i].collide = FALSE;

			back[i].pt.x = rand() % WideSize - back[i].size;
			back[i].pt.y = rand() % HighSize - back[i].size;

		}
		break;
	default:
		;
		break;
	}
}

GLvoid Reshape(int w, int h)
{
	glViewport(-1, -1, w, h);
	glOrtho(0, WideSize, HighSize, 0, 0, 1.0); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!

}