#include <GL/freeglut.h> 
#include <random>
#include <time.h>

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

					  //�� ����
typedef struct Color
{
	int R;
	int B;
	int G;
};
//Ư�� ���� �ִ�ġ
int max_x;
int max_y;

//Ư������ ������
int num_x;
int num_y;

Color color[10];
Color change_color[10][10];
void main(int argc, char *argv[]) {
	//�ʱ�ȭ
	max_x = rand() % 10 + 1;
	max_y = rand() % 10 + 1;

	for (int i = 0; i < max_x; i++) {
		color[i].R = rand() % 200 + 50;
		color[i].G = rand() % 200 + 50;
		color[i].B = rand() % 200 + 100;
		change_color[i][0].R = color[i].R;
		change_color[i][0].G = color[i].G;
		change_color[i][0].B = color[i].B;
	}

	for (int i = 1; i < max_x; i++)
	{
		for (int j = 0; j < max_y - 1; j++)
		{
			change_color[i][j].R = change_color[i - 1][j].R - 20;
			change_color[i][j].G = change_color[i - 1][j].G - 20;
			change_color[i][j].B = change_color[i - 1][j].B - 5;

		}

	}

	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);//������ ���� ��ǥ
	glutInitWindowSize(WideSize, HighSize); //������ ���� ũ��
	glutCreateWindow("ex2");
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
	//���
	for (int i = 0; i < max_y; i++) {
		for (int j = 0; j < max_x; j++) {
			//4���� �׸���
			glColor4f(
				(float)change_color[i][j].R / 255,
				(float)change_color[i][j].G / 255,
				(float)change_color[i][j].B / 255, 1.0f
			);
			glRectf((WideSize / max_x) * j, (HighSize/max_y)* i, (WideSize / max_x) * (j + 1), (HighSize / max_y)* (i+1));

		}
	

	}
	glFlush();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

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
		max_x = rand() % 10+2;
		max_y = rand() % 10+2;

		for (int i = 0; i < max_x; i++) {

			color[i].R = rand() % 100 + 50;
			color[i].G = rand() % 150 + 100;
			color[i].B = rand() % 150 + 100;

			if (color[i].R > 200) {
				color[i].R = rand() % 100 + 50;
			}

			change_color[i][0].R = color[i].R;
			change_color[i][0].G = color[i].G;
			change_color[i][0].B = color[i].B;
		}
		
		for (int i = 0; i < max_y; i++)
		{
			for (int j = 1; j < max_x; j++)
			{
				change_color[i][j].R = change_color[i][j-1].R - 90;
				change_color[i][j].G = change_color[i][j-1].G - 40;
				change_color[i][j].B = change_color[i][j-1].B - 35;

			}

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