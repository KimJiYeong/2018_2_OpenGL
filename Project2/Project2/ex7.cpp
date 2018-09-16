#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>

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
POINT ms_click; //���콺 ��ǥ
BOOL Mouse_Act_On; //���콺 Ȱ��ȭ ����

//Ű���� ����
void Keyboard(unsigned char key, int x, int y);
BOOL Keyboard_Act_On; //Ű���� Ȱ��ȭ ����

//�÷�
typedef struct Color
{
	int R;
	int B;
	int G;
};

//���� ����
#define cir_amount 15
#define PI 3.14159265358

typedef struct Circle {
	POINT pt[36]; //360��
	Color cl; //����
	int rad;//������
	int Ani_rad; //�ִϸ��̼� ����
	POINT pos;//�߽� ��ǥ
	int Max_rad; //�ƽ� ����
	//-------------------------------------
	BOOL Ani;//������ ������ �ִϸ��̼� ����
};
int Ani_rand;
BOOL draw;
int circle_count =0;
int draw_count = 0;
Circle cir[cir_amount];

void main(int argc, char *argv[]) {
	//�ʱ�ȭ
	for (int i = 0; i < cir_amount; i++) {
	
		cir[i].cl.R = 255;
		cir[i].cl.G = 255;
		cir[i].cl.B = 255;
		cir[i].rad = 0;
	}
	glutInit(&argc, argv);//������ ȭ�� �Ⱥ���

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);//������ ���� ��ǥ
	glutInitWindowSize(WideSize, HighSize); //������ ���� ũ��
	glutCreateWindow("ex7");
	// - �������� ���� ���� �����ϱ�
	//���� �׸���

	glutDisplayFunc(drawScene);
	glutMouseFunc(Mouse);
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
	//�׸��� ���	
	
	for (int i = 0; i < cir_amount; i++) {
		glPointSize(3);
		glColor4f(
			(float)cir[i].cl.R / 255,
			(float)cir[i].cl.G / 255,
			(float)cir[i].cl.B / 255,
			1.0f
		);
		glBegin(GL_POINTS);
		for (int j = 0; j < 36; j++) {
			glVertex2f(
				cir[i].pos.x + cir[i].pt[j].x, 
				cir[i].pos.y + cir[i].pt[j].y
			);
		}
		glEnd();
	}


	glFlush();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		
		if((rand() % 2))
			cir[circle_count].Ani = TRUE;
		else
			cir[circle_count].Ani = FALSE;

		cir[circle_count].pos.x = x;
		cir[circle_count].pos.y = y;

		cir[circle_count].cl.R = 100;
		cir[circle_count].cl.G = 100;
		cir[circle_count].cl.B = 200;
		cir[circle_count].rad = 0;

		//circle ī��Ʈ �ʱ�ȭ
		if (circle_count == 19) {
			circle_count = 0;
			draw = TRUE;
		}
		else {
			if (draw)
				draw_count = 20;
			else
				draw_count++;

			circle_count++;
		}

	}

}

void Timerfunction(int value) {

	//Ÿ�̸� ���� �Է�
	for (int i = 0; i < draw_count; i++) {
		
		if (cir[i].Ani) {//�ִϸ��̼��� Ʈ����
			cir[i].Max_rad = WideSize - cir[i].pos.x + 100;
			if (cir[i].cl.R < 255)
				cir[i].cl.R += 2;
			else
				cir[i].cl.R = 50;
			
			if (cir[i].cl.G < 255)
				cir[i].cl.G += 2;
			else
				cir[i].cl.G = 50;
			
			if (cir[i].cl.G < 255)
				cir[i].cl.G += 2;
			else
				cir[i].cl.G = 50;

		}
		else {
			cir[i].cl.R = 100;
			cir[i].cl.G = 100;
			cir[i].cl.B = 200;

			cir[i].Max_rad = 80; //���� ����
		}
		
		if (cir[i].rad < cir[i].Max_rad) {
			cir[i].rad += 2; //������ ����
		}
		else {
			if (cir[i].Ani)
				;
			else
				cir[i].rad = 0;
		}
		
		for (int j = 0; j < 36; j++) {
				cir[i].pt[j].x = cos(PI * j * 10/ 180) * cir[i].rad;
				cir[i].pt[j].y = sin(PI * j * 10 / 180) * cir[i].rad;
		}
	}

	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.
	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'q'://������

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