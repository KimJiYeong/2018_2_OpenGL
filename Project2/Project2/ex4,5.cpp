#include <GL/freeglut.h> 
#include <random>
#include <time.h>
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

#define WideSize 800
#define HighSize 600
BOOL Mouse_click;
POINT click; //���콺 ��ǥ
typedef struct Shape {
	POINT pos;
	int color_R;
	int color_G;
	int color_B;
	int size;
	int Speed;
	BOOL LOOK; //���� T = �Ʒ� F = UP
	BOOL SIZE;
	BOOL Sha;//���
	int Go;//����
};

Shape shape[10];//�簢�� ����
int shape_count = 0;//10�� �Ѿ�°� üũ

void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Timerfunction(int value);

void main(int argc, char *argv[]) {
	//�ʱ�ȭ
	for (int i = 0; i < 10; i++) {
		shape[i].color_R = 255;
		shape[i].color_G = 255;
		shape[i].color_B = 255;
		shape[i].size= 0;
		shape[i].pos.x = 0;
		shape[i].pos.y = 0;
		shape[i].Speed = i * 0.2;

	}
	int MainMenu;
	int SubMenu1 = 0;
	int SubMenu2 = 0;
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
	//4���� ���
	for (int i = 0; i < 10; i++) {
		glColor4f(
			(float)shape[i].color_R / 255,
			(float)shape[i].color_G / 255,
			(float)shape[i].color_B / 255, 1.0f
		);
		if (shape[i].Sha == TRUE) {
			glRectf(
				shape[i].pos.x - shape[i].size,
				shape[i].pos.y - shape[i].size,
				shape[i].pos.x + shape[i].size,
				shape[i].pos.y + shape[i].size
			);

		}
		else {
			glBegin(GL_TRIANGLES);
			glVertex2f(shape[i].pos.x - shape[i].size, shape[i].pos.y - shape[i].size);
			glVertex2f(shape[i].pos.x + shape[i].size, shape[i].pos.y - shape[i].size);
			glVertex2f(shape[i].pos.x + shape[i].size, shape[i].pos.y + shape[i].size);
			glEnd();

		}
		
	}

	
	glFlush();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		shape[shape_count].Go = 1;
		shape[shape_count].LOOK = TRUE;
		shape[shape_count].pos.x = x; //���콺 ��ǥ�� �ѱ��.
		shape[shape_count].pos.y = y;
			shape[shape_count].color_R = 100;
			shape[shape_count].color_G = 100;
			shape[shape_count].color_B = 100;
			shape[shape_count].size = 10;
			if (shape_count == 10) {
				shape_count = 0;
			}
			else {
				shape_count++;
			}
		
	}

}
void Timerfunction(int value) {
	for (int i = 0; i < 10; i++) {
		shape[i].color_R += 10;

		if (shape[i].SIZE) {
			if (shape[i].size == 20) {
				shape[i].SIZE = FALSE;
				shape[i].color_R = 0;
			}
			shape[i].color_B -= 10;
			shape[i].color_G += 10;
			shape[i].size += 2;
		}
		else {
			if (shape[i].size == 10) {
				shape[i].SIZE = TRUE;
				shape[i].color_R = 0;

			}
			shape[i].color_G -= 10;
			shape[i].color_B += 10;
			shape[i].size -= 2;
		}
	
	//�簢�� �����̱�
		if (shape[i].pos.x + shape[i].size > WideSize || shape[i].pos.y + shape[i].size > HighSize) {
			if (shape[i].LOOK == TRUE){
				shape[i].Go = 2;
			}
			else if (shape[i].LOOK == FALSE) {
				shape[i].Go = 3;
			}
		}
		else if (shape[i].pos.x - shape[i].size  < 0  || shape[i].pos.y - shape[i].size  < 0) {
			if (shape[i].LOOK == TRUE) {
				shape[i].Go = 1;
			}
			else if (shape[i].LOOK == FALSE) {
				shape[i].Go = 4;
			}
		}
		
		//Ŀ�ǵ忡 ���� �Է��� �޴´�.
		
		if (shape[i].Go == 1) {
			shape[i].pos.x += 10 + shape[i].Speed;
			shape[i].pos.y += 10 + shape[i].Speed;
			shape[i].LOOK = TRUE;

		}
		else if (shape[i].Go == 2) {
			shape[i].pos.x += 10 + shape[i].Speed;
			shape[i].pos.y -= 10 + shape[i].Speed;
			shape[i].LOOK = FALSE;
		}
		else if (shape[i].Go == 3) {
			shape[i].pos.x -= 10 + shape[i].Speed;
			shape[i].pos.y -= 10 + shape[i].Speed;
			shape[i].LOOK = FALSE;
		}
		else if (shape[i].Go == 4) {
			shape[i].pos.x -= 10 + shape[i].Speed;
			shape[i].pos.y += 10 + shape[i].Speed;
			shape[i].LOOK = TRUE;
		}
	}


//	shape[shape_count].color_G = +5;
//	shape[shape_count].color_B = +5;
	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.

	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���
	
}

void Keyboard(unsigned char key, int x, int y){
	switch (key)
	{
	case 'q'://������
		for (int i = 0; i < 10; i++) {
			shape[i].Speed += 2;
		}
		break;
	case 's'://������
		for (int i = 0; i < 10; i++) {
			shape[i].Speed -= 2;
		}
		break;
	case 'r':
		for (int i = 0; i < 10; i++) {
			shape[i].Sha = TRUE;
		}
		break;
	case 't':
		for (int i = 0; i < 10; i++) {
			shape[i].Sha = FALSE;
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
	glOrtho(0, WideSize, HighSize,0, 0, 1.0); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!

}