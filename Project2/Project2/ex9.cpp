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
int Time_count; //Ÿ�̸� ī��Ʈ

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
};

Shape back[100]; //��� �Լ�
Shape erase;//���찳 �Լ�



void main(int argc, char *argv[]) {
	//�ʱ�ȭ
	for (int i = 0; i < 100; i++) {
		
		//�簢�� �ʱ�ȭ �Լ�
		back[i].cl.R = rand() % 255;
		back[i].cl.G = rand() % 255;
		back[i].cl.B = rand() % 255;
		back[i].size = 10;
		
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
	glutCreateWindow("ex4");
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
	for (int i = 0; i < 100; i++) {
		if (!back[i].collide) {
			glColor4f(
				(float)back[i].cl.R / 255,
				(float)back[i].cl.G / 255,
				(float)back[i].cl.B / 255, 1.0f
			);

			glRectf(
				back[i].pt.x - back[i].size,
				back[i].pt.y - back[i].size,
				back[i].pt.x + back[i].size,
				back[i].pt.y + back[i].size
			);
			glEnd();
		}
	
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

	for (int i = 0; i < 100; i++) {
		if (erase.pt.x + erase.size > back[i].pt.x - back[i].size 
			&& erase.pt.y + erase.size > back[i].pt.y - back[i].size
			&& erase.pt.x - erase.size < back[i].pt.x + back[i].size
			&& erase.pt.y - erase.size < back[i].pt.y + back[i].size
			) {
			back[i].collide = TRUE;
		}
	
	}

}
void Timerfunction(int value) {

	//Ÿ�̸� ���� �Է�

	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.
	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'r'://������
			 //�ʱ�ȭ
		for (int i = 0; i < 100; i++) {

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