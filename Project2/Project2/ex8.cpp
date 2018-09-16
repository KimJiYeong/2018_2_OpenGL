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
	POINT pt[60]; //360��
	POINT pt2[60];
	Color cl; //����
	int rad[60];//������
	int Ani_rad; //�ִϸ��̼� ����
	POINT pos;//�߽� ��ǥ
	int Max_rad; //�ƽ� ����
				 //-------------------------------------
	BOOL Ani;//������ ������ �ִϸ��̼� ����
	int Time_count; //Ÿ�̸� ī��Ʈ
	
	int Ani1_count;//������ ������
	int Ani2_count;//������ ������

	//�ݴ�� ������
	int dir; //����

};
int Ani_rand;
BOOL draw;
int circle_count = 0;
int draw_count = 0;
Circle cir[cir_amount];

void main(int argc, char *argv[]) {
	//�ʱ�ȭ
	for (int i = 0; i < cir_amount; i++) {

	//	cir[i].cl.R = 255;
	//	cir[i].cl.G = 255;
	//	cir[i].cl.B = 255;
		for (int k = 0; k < 60; k++) {
			cir[i].rad[k] = 0;
		}
		cir[i].Ani = FALSE;
		cir[i].Ani2_count = 60;

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
		for (int j = 0; j < 60; j++) {
				glVertex2f(
					cir[i].pos.x + cir[i].pt[j].x,
					cir[i].pos.y + cir[i].pt[j].y
				);
				glVertex2f(
					cir[i].pos.x + cir[i].pt2[j].x ,
					cir[i].pos.y + cir[i].pt2[j].y
				);
		}
		
		glEnd();
	}


	glFlush();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	
		cir[circle_count].pos.x = x;
		cir[circle_count].pos.y = y;

		cir[circle_count].cl.R = 100;
		cir[circle_count].cl.G = 100;
		cir[circle_count].cl.B = 200;

		cir[circle_count].dir = rand() % 2;


		for (int k = 0; k < 60; k++) {
			cir[circle_count].rad[k] = 0;
		}
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
		

		cir[i].Time_count++;

		if (cir[i].Time_count %1 == 0) {

			if (cir[i].dir ==1) {//�ִϸ��̼��� Ʈ����
				if (cir[i].Ani1_count <= 60)
				{
					cir[i].Ani = FALSE;
					cir[i].Ani1_count++;

				}
				else {
					cir[i].Ani = TRUE;
				}
				if (cir[i].Ani2_count > 0) {
					if (cir[i].Ani == TRUE) {
						cir[i].Ani2_count--;

						for (int j = 60; j > cir[i].Ani2_count; j--) {
							cir[i].rad[j] = j * 1.5; //������ ����
							cir[i].pt2[j].x = -cos(PI * j * 24 / 180) * cir[i].rad[j] + cir[i].pt[60].x * 2;
							cir[i].pt2[j].y = sin(PI * j * 24 / 180) * cir[i].rad[j];

						}
					}

				}

				for (int j = 0; j < cir[i].Ani1_count; j++) {
					cir[i].rad[j] = j * 1.5; //������ ����

					cir[i].pt[j].x = cos(PI * j * 24 / 180) * cir[i].rad[j];
					cir[i].pt[j].y = -sin(PI * j * 24 / 180) * cir[i].rad[j];



				}
			}
			else {

			
					if (cir[i].Ani1_count <= 60)
					{
						cir[i].Ani = FALSE;
						cir[i].Ani1_count++;

					}
					else {
						cir[i].Ani = TRUE;
					}
					if (cir[i].Ani2_count > 0) {
						if (cir[i].Ani == TRUE) {
							cir[i].Ani2_count--;

							for (int j = 60; j > cir[i].Ani2_count; j--) {
								cir[i].rad[j] = j * 1.5; //������ ����
								cir[i].pt2[j].x = cos(PI * j * 24 / 180) * cir[i].rad[j] + cir[i].pt[60].x * 2;
								cir[i].pt2[j].y = -sin(PI * j * 24 / 180) * cir[i].rad[j];

							}
						}

					}

					for (int j = 0; j < cir[i].Ani1_count; j++) {
						cir[i].rad[j] = j * 1.5; //������ ����

						cir[i].pt[j].x = -cos(PI * j * 24 / 180) * cir[i].rad[j];
						cir[i].pt[j].y = sin(PI * j * 24 / 180) * cir[i].rad[j];


					}
			}


			
			}//�ִϸ��̼�
		
		
	}//Ÿ�̸� ī��Ʈ

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