#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>
#include "cross.h"


double _ccw(vector2 a, vector2 b) {
	return a.cross(b);
}

double _ccw(vector2 p, vector2 a, vector2 b) {
	return _ccw(a - p, b - p);
}


GLvoid Reshape(int w, int h);

//�ػ� ����
#define WideSize 1000
#define HighSize 800
#define Z_Size 400

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

void Motion(int x, int y);
BOOL Motion_Act_On; //���콺 Ȱ��ȭ ����

int Gradient;//True�� ��� False�� ����

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
	float x;
	float y;
	float z;
	float degree;
};

typedef struct Tran {
	double x;
	double y;
};
typedef struct BoOL
{
	int b_x;
	int b_y;
	int b_z;
};

#define PT 20//���� ���� ����
#define PI 3.141592 //����
typedef struct Shape
{
	Color cl;//����
	vector2 pos;
	Translate_pos move;
	Translate_pos scale;
	Translate_pos rot;

	int size;
	int select;
	BoOL b;
	BOOL any;

};

typedef struct Main_Shape
{
	Color cl;//����
	vector2 pos[4];
	Translate_pos move;
	Translate_pos scale;
	Translate_pos rot;

	int size;
	int select;
	int height;
	int slice;
	int stacks;
	BoOL b;
	BOOL any;

};


BOOL Activation;//Activation Ʈ�� : �浹 / �� :�浹 x 
#define Sub_t_num 10
#define Sub_t_size 50

//�ﰢ��
Shape sub_t[Sub_t_num];//���ʿ� �ִ� �ﰢ��
Main_Shape main_R[1];//����
Main_Shape main_t[Sub_t_num];
//�巡�� ��
Translate_pos Drag[3];

void main(int argc, char *argv[]) {
	//�ʱ�ȭy
	for (int i = 0; i < Sub_t_num; i++) {
		sub_t[i].size = Sub_t_size;
		sub_t[i].pos.x = -Sub_t_size;
		sub_t[i].pos.y = HighSize - Sub_t_size;
	}

	//�ӽ� �ʱ�ȭ
	main_R[0].move.x = WideSize/2 - Sub_t_size / 4;
	main_R[0].move.y = 0;

	main_R[0].pos[0].x = +Sub_t_size/ 2, main_R[0].pos[0].y =  +Sub_t_size / 2; // 
	main_R[0].pos[1].x = +Sub_t_size / 2, main_R[0].pos[1].y =  -Sub_t_size / 2;
	main_R[0].pos[2].x = -Sub_t_size / 2, main_R[0].pos[2].y =  -Sub_t_size / 2;
	main_R[0].pos[3].x = -Sub_t_size / 2, main_R[0].pos[3].y =  +Sub_t_size / 2;


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);//������ ���� ��ǥ
	glutInitWindowSize(WideSize, HighSize); //������ ���� ũ��
	glutCreateWindow("ex16");
	// - �������� ���� ���� �����ϱ�
	//���� �׸���
	//SetupRC();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(100, Timerfunction, 1);
	srand(time(NULL));
	glutMainLoop();
}

//�� ������ ���� �����ϴ��� ���θ� ��ȯ�Ѵ�.
bool sementIntersects(vector2 a, vector2 b, vector2 c, vector2 d) {
	double ab = ccw(a, b, c)*ccw(a, b, d);
	double cd = ccw(c, d, a)*ccw(c, d, b);
	//�� ������ �� ������ ���� �ְų� ������ ��ġ�� ���
	if (ab == 0 && cd == 0) {
		if (b < a) swap(a, b);
		if (d < c) swap(c, d);
		return !(b< c || d <a);
	}
	return ab <= 0 && cd <= 0;
}
int Grad_i;//���ο� i
GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();//-----------------------------------
	{
		//���� ��
		glBegin(GL_LINES);
		glVertex2i(0, HighSize - Sub_t_size + Sub_t_size / 2 );
		glVertex2i(WideSize, HighSize - Sub_t_size + Sub_t_size / 2);
		glEnd();

		//�Ʒ� ��
		for (int i = 0; i < 3; i++) {
			glBegin(GL_LINES);//���� ���μ�
			glVertex2i(0,Sub_t_size * (i+1));
			glVertex2i(Sub_t_size * (5), Sub_t_size * (i+ 1));
			glEnd();

			glBegin(GL_LINES);//������ ���μ�
			glVertex2i(WideSize,Sub_t_size * (i + 1));
			glVertex2i(WideSize - Sub_t_size * (5), Sub_t_size * (i + 1));
			glEnd();

		}
		for (int i = 0; i < 10; i++) {

			if (i < 5) {
				glBegin(GL_LINES);//���μ�
				glVertex2i(Sub_t_size * (i + 1), Sub_t_size * (3));
				glVertex2i(Sub_t_size * (i + 1), 0);
				glEnd();
			}
			else {
				glBegin(GL_LINES);//���μ�
				glVertex2i(WideSize - Sub_t_size * (5 - i + 5) ,Sub_t_size * (3));
				glVertex2i(WideSize - Sub_t_size * (5 - i + 5), 0);
				glEnd();

			}
	
		}


		for (int i = 0; i < Sub_t_num; i++) {
			//���� ���� ���ƴٴϴ� �ﰢ��
			glPushMatrix();
			glTranslated(sub_t[i].pos.x, sub_t[i].pos.y + Sub_t_size / 2, 0);
		//��� ��ġ ���ϱ�
			glPushMatrix();
		//ȸ���ϱ�
			glRotated(sub_t[i].rot.degree, 0, 0, 1);
			glBegin(GL_TRIANGLES);
			glVertex2i(0, 0);
			glVertex2i(sub_t[i].size, 0);
			glVertex2i(0, sub_t[i].size);
			glEnd();
			glPopMatrix();
			glPopMatrix();
			//----------
		}
		
		glBegin(GL_TRIANGLES);
		glVertex2i(0, 0);
		glVertex2i(10, 0);
		glVertex2i(0, 10);
		glEnd();	

		//���� �簢��

		glPushMatrix();
		glTranslated(main_R[0].move.x, main_R[0].move.y, 0);
		glBegin(GL_POLYGON); // ����
		glVertex2i(main_R[0].pos[3].x, main_R[0].pos[3].y);
		glVertex2i(main_R[0].pos[2].x, main_R[0].pos[2].y);
		glVertex2i(main_R[0].pos[1].x, main_R[0].pos[1].y);
		glVertex2i(main_R[0].pos[0].x, main_R[0].pos[0].y); // 
		glEnd();
		glPopMatrix();

		//������
		for (int i = 0; i < Grad_i; i++) {
			glPushMatrix();
			if (i % 2 == 0) {
				glTranslated(main_t[i].move.x, main_t[i].move.y, 0);
				glBegin(GL_POLYGON); // ����
				glVertex2i(main_t[i].pos[2].x, main_t[i].pos[2].y);
				glVertex2i(main_t[i].pos[0].x, main_t[i].pos[0].y);
				glVertex2i(main_t[i].pos[1].x, main_t[i].pos[1].y);

				glEnd();
			}
			else {
				glBegin(GL_POLYGON); // ������
				glVertex2i(main_t[i].pos[2].x, main_t[i].pos[2].y);
				glVertex2i(main_t[i].pos[1].x, main_t[i].pos[1].y);
				glVertex2i(main_t[i].pos[0].x, main_t[i].pos[0].y);
				glEnd();
			}
			glPopMatrix();
		}


		if (Mouse_Act_On && Motion_Act_On) {
			//�巡���ϴ� �� �׸���
			glBegin(GL_LINES);
			glVertex2i(Drag[0].x, Drag[0].y);
			glVertex2i(Drag[1].x, Drag[1].y);
			glEnd();
		}

	}	glPopMatrix();


	

	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		Drag[0].x = x;
		Drag[0].y = HighSize - y;
		Mouse_Act_On = true;
		Motion_Act_On = true;
		Gradient = 0;

	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
	
		if (Drag[0].x < Drag[1].x) {
			if (Drag[0].y < Drag[1].y) { // ���� +
				Gradient = 1;

				printf("��� %d \n", Gradient);
				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[3].x;
				main_t[Grad_i].move.x = 100;

				Grad_i++;

				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[1].x;

				Grad_i++;

			}
			else {// ���Ⱑ ���̳ʽ� p0 y < p1 y
				Gradient = 2;
				printf("��� %d \n", Gradient);
				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[3].x;

				Grad_i++;

				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[1].x;

				Grad_i++;
			}
		}
		else {//P0 x  > P1 x �� ������
			if (Drag[0].y > Drag[1].y) { // ���� +
				Gradient = 3;
				printf("��� %d \n", Gradient);
				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[3].x;

				Grad_i++;

				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[1].x;

				Grad_i++;
			}
			else {// ���Ⱑ ���̳ʽ� P0 y > P1 y
				Gradient = 4;
				printf("��� %d \n", Gradient);
				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[3].x;

				Grad_i++;

				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[1].x;

				Grad_i++;
			}
		}
		Drag[2].y = Drag[1].y - Drag[0].y;
		Drag[2].x = Drag[1].x - Drag[0].x;
		Drag[2].degree = Drag[2].y / Drag[2].x;//����

		Mouse_Act_On = false;
	}


	glutPostRedisplay();

}
void Motion(int x, int y) {
	if (Motion_Act_On == true)
	{
		Drag[1].x = x;
		Drag[1].y = HighSize - y;
	}
	
	glutPostRedisplay();

}
float LenghtPts(int x1,int y1, int x2, int y2) {
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

BOOL InCircle(int x, int y, int mx, int my , int size) {
	if (LenghtPts(x, y, mx, my) < size) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}
int temp_x, temp_y;
int move_start_count;//���ʿ��� ��Ÿ�� ������

void Timerfunction(int value) {
	//Ÿ�̸� ����
	Time_count++;
	if (Time_count % 20 == 0) {
		if (move_start_count < Sub_t_num)
			move_start_count++;
	}
	for (int i = 0; i < move_start_count; i++) {
		if (sub_t[i].pos.x > WideSize) {//ȭ�� ������ ������ �ʱ� ���·� �����ش�.
			sub_t[i].pos.x = -Sub_t_size * 20;//20�� ���� ������ Ÿ�̹��� ���߱� ���ؼ�.
		}
		
		sub_t[i].pos.x += 10;
		sub_t[i].rot.degree += 10;
	}

	for (int i = 0; i < 4; i++) {
		main_R[0].pos[i].y += 10;
	}


	//m = (y2 - y1)/(x2 - x1)
	//y = mx
	//x = y/m

	if (!Mouse_Act_On) {//�浹üũ ����
	//���⿡ ���� �浹üũ�� �޶�����
	//���� ���
		if (Gradient == 1 || Gradient == 3) {
			temp_y = Drag[2].degree *WideSize/2 ;
			if ((main_R[0].pos[3].x < temp_y) && (temp_y < main_R[0].pos[0].x)) {
				main_R[0].move.y -= 10;
				printf("1");
			}
	

		}
		else if (Gradient == 2 || Gradient == 4) {
			temp_y = (WideSize / 2) *((Drag[1].y - Drag[0].y) / (Drag[1].x - Drag[1].x));
			if ((main_R[0].pos[1].y < temp_y) && (temp_y < main_R[0].pos[0].y)) {
				main_R[0].move.y -= 10;
				printf("5");
			}
			printf("���� %d \n", Gradient);
		}
		printf("3");
	}
	

	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.
	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���

}
int ttt;
int ani_count;
void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{

	case '2'://�������� ����

		break;
	default:
		;
		break;
	}
	glutPostRedisplay();

}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45, WideSize / HighSize, 1, Z_Size); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!
	//glTranslatef(0, 0, -300);
	glOrtho(0, WideSize, 0, HighSize, -Z_Size / 2, Z_Size / 2); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!
	//glRotatef(-60, 1, 0, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}