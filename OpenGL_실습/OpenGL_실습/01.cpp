#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>
GLvoid Reshape(int w, int h);

//�ػ� ����
#define WideSize 800
#define HighSize 600
#define Z_Size 400
#define PI 3.141592
#define Point_num 100
//�׸��� ����
GLvoid drawScene(GLvoid);
BOOL draw_Act_On;

//Ÿ�̸� ����
void Timerfunction(int value);
BOOL Time_Act_On; //Ÿ�̸� Ȱ��ȭ ����
int move_count; //Ÿ�̸� ī��Ʈ

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

//�������
struct Shape
{
	int x =0;//x��ǥ
	int y=0;//y��ǥ
	int z=0;//z��ǥ

	int choose_shape;//1 - ���� 2- ������ 3 - ���� 4 - �簢��

};
Shape shape[Point_num];
Color cl;
void main(int argc, char *argv[]) {
	//�ʱ�ȭ
	for (int i = 0; i < Point_num; i++) {
			ball[i].x = 0;
			ball[i].y = 0;
			ball[i].z = 0;

			ball[i].choose_shape = 1;

	}
	cl.R = 100;
	cl.G = 100;
	cl.B = 100;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);//������ ���� ��ǥ
	glutInitWindowSize(WideSize, HighSize); //������ ���� ũ��
	glutCreateWindow("ex4");
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
	glPointSize(3);
	glLineWidth(3);
	
	glColor3f((float)100 / 255, (float)200 / 255, (float)100 / 255);
	
	glBegin(GL_LINES);
	glVertex3i(0, HighSize / 2, -(Z_Size / 2));
	glVertex3i(WideSize, HighSize / 2, (Z_Size / 2));
	glEnd();

	glColor3f((float) cl.R/ 255, (float) cl.G/255, (float) cl.B/ 255);

	glBegin(GL_LINE_STRIP);
	
	for (int i = 0; i < Point_num; i++) {
		glVertex3i(ball[i].x , ball[i].y, ball[i].z);
	}
	glEnd();

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
	case '1'://���ΰ
		for (int i = 0; i < Point_num; i++) {
				ball[i].x = i * WideSize / 50;
				ball[i].y = 100 *  sin(PI * i * 10/90) + HighSize/2;
		/*
					cir[i].pt2[j].x = -cos(PI * j * 24 / 180) * cir[i].rad[j] + cir[i].pt[60].x * 2;
							cir[i].pt2[j].y = sin(PI * j * 24 / 180) * cir[i].rad[j];
*/
				
				ball[i].z = 100;
				//����� ����: z�� cos���� �ؾ��Ѵ�.
				ball[i].choose_shape = 1;

		}
		break;
	case '2'://������
		for (int i = 0; i < Point_num; i++) {
			ball[i].x = 100 * cos(PI * i * 10 / 90) + WideSize / 50 + i * 10 ;
			ball[i].y = 100 * sin(PI * i * 10 / 90) + HighSize / 2;
			ball[i].z = cos(PI * i * 10 / 90);
			//����� ����: z�� cos���� �ؾ��Ѵ�.
			ball[i].choose_shape = 1;

		}
		break;
	case '3'://����

		break;
	case '4'://�簢��

		break;
	default:
		;
		break;
	}
}

GLvoid Reshape(int w, int h)
{
	glViewport(-1, -1, w, h);
	glOrtho(0, WideSize, 0, HighSize, -(Z_Size/2), (Z_Size/2)); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!

}