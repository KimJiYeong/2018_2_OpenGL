#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>
GLvoid Reshape(int w, int h);

//�ػ� ����
#define WideSize 900
#define HighSize 900
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

#define PT 20//���� ���� ����
#define PI 3.141592 //����
typedef struct Shape
{
	Color cl;//����
	Translate_pos pos;
	Translate_pos move;
	Translate_pos scale;
	Translate_pos rot;

	int size;
	int select;
	int height;
	int slice;
	int stacks;

	BOOL any;

};

int change_count;
int select_count;
int st_help;
BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;

Shape camera;
Shape tra;
int rot_count;
int rot_command;

Shape shape[36];
Shape view;
void SetupRC()
{
	//�ʱ�ȭ
	glTranslated(WideSize / 2, HighSize / 2, Z_Size / 2);
}
void main(int argc, char *argv[]) {
	//�ʱ�ȭ

	for (int i = 0; i < 36; i++) {
		shape[i].pos.x = 0;
		shape[i].pos.y = 0;

	}

	camera.pos.x = 0;
	camera.pos.y = 1;
	camera.pos.z = 1;

	shape[0].pos.x = 100;
	shape[1].pos.x = 100;
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
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(100, Timerfunction, 1);
	srand(time(NULL));
	glutMainLoop();
}
void rot_custom(int sel, int degree) {
	//x = 0
	//y = 1
	//z = 2
	GLdouble x_rotate[16] = {
		1, 0, 0, 0,
		0, cos(degree * PI * 10 / 180),  -sin(degree * PI * 10 / 180), 0,
		0, sin(degree * PI * 10 / 180), cos(degree * PI * 10 / 180), 0,
		0, 0, 0, 1
	};
	GLdouble y_rotate[16] = {
		cos(degree * PI * 10 / 180), 0, sin(degree * PI * 10 / 180), 0,
		0, 1,  0, 0,
		-sin(degree * PI * 10 / 180), 0,cos(degree * PI * 10 / 180), 0,
		0, 0, 0, 1
	};
	GLdouble z_rotate[16] = {
		cos(degree * PI * 10 / 180),  -sin(degree * PI * 10 / 180), 0,0,
		sin(degree * PI * 10 / 180), cos(degree * PI * 10 / 180), 0,0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	if (sel == 0) {
		glLoadMatrixd(x_rotate);
		glMultMatrixd(x_rotate);
	}
	else if (sel == 1) {

		glLoadMatrixd(y_rotate);
		glMultMatrixd(y_rotate);
	}
	else if (sel == 2) {

		glLoadMatrixd(z_rotate);
		glMultMatrixd(z_rotate);
	}
	
};
GLvoid drawScene(GLvoid)
{
	//������ ������ ���� ���� �ʹ� �ǰ��ؼ� �����Ͱ�����
	//
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();{

		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		//glRotatef(view.rot.degree, view.rot.x, view.rot.y, view.rot.z);
		rot_custom(select_count, camera.rot.degree);
		gluLookAt(
			0, 0 , 1,  //��5 eye
			0, 0, 0, //���� center
			0, 1, 0); //���ʹ���(�ǵ� ����) up
		//glTranslated(0, 0, 0);
		
		glLineWidth(2);
	
		//��� ���� �׸���
		glColor3f((float)255 / 255, (float)0 / 255, (float)0 / 255);
		glTranslated(0, 0, 0);
			//for (int i = 0; i < 3; i++) {

			//	glPushMatrix();
			//	if (i == 0) {
			//		glColor3f((float)255 / 255, (float)0 / 255, (float)0 / 255);
			//		glRotated(90, 1, 0, 0);
			//	}
			//	else if (i == 1) {
			//		glColor3f((float)0 / 255, (float)255 / 255, (float)0 / 255);
			//		glRotated(90, 0, 1, 0);
			//	}
			//	else if (i == 2) {
			//		glColor3f((float)0 / 255, (float)0 / 255, (float)255 / 255);
			//		glRotated(90, 0, 0, 1);
			//	}
			//	glScalef(1, 0.1, 0.1);
			//	glutSolidCube(40);
			//	glPopMatrix();
			//	glPushMatrix();
			//	glScaled(1, 0.01, 1);
			//	glutSolidCube(20);
			//	glPopMatrix();

			//}//��ǥ�� �׸���

			//�׸���
			glPushMatrix();
			glTranslated(0, 0, 0);
			glutSolidSphere(20, 20, 20);
			glPopMatrix();
			//�׸��� ��

			//�׸���
			glPushMatrix();
			glTranslated(70, 0, 0);

			for (int i = 0; i < 3; i++) {
				glColor3f((float)0 / 255, (float)0 / 255, (float)255 / 255);
				glTranslated(0, 0, 0);
				//glPushMatrix();
				if (i == 0) {
					glRotatef(90, 1, 0, 0);
				}
				else if (i == 1) {
					glRotatef(90, 0, 1, 0);
				}
				else if (i == 2) {
					glRotatef(90, 0, 0, 1);
				}
				//glPopMatrix();
				glutSolidSphere(15, 15, 15);
			}
			glPopMatrix();
			//�׸��� ��

			 //glRotatef(Time_count, 0, 1, 0);
	}
	glPopMatrix();

	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	}
	glutPostRedisplay();

}
void Timerfunction(int value) {

	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.
	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	
	//------------------ī�޶�------------------------
	case 'x':
		camera.rot.degree += 1;
		select_count = 0;
		// z�� �״�� camera.z 
		break;
	case 'y':
		camera.rot.degree += 1;
		select_count = 1;
		// z�� �״�� camera.z 
		break;
	case 'z':
		camera.rot.degree += 1;
		select_count = 2;


		// z�� �״�� camera.z 
		break;

		//-----------ī�޶� �� --------
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
	gluPerspective(45, WideSize / HighSize, 1, Z_Size); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!
	glTranslated(0, 0, -300);
	glMatrixMode(GL_MODELVIEW);

}