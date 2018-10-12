#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>
GLvoid Reshape(int w, int h);

//�ػ� ����
#define WideSize 800
#define HighSize 600
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

Translate_pos camera;
Shape tra;
int rot_count;
int rot_command;

Shape shape[2];
Shape view;
void SetupRC()
{
	//�ʱ�ȭ
	glTranslated(WideSize / 2, HighSize / 2, Z_Size / 2);
}
void main(int argc, char *argv[]) {
	//�ʱ�ȭ

	camera.x = 0;
	camera.y = 1;
	camera.z = 1;

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

GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();{
		glRotatef(view.rot.degree, view.rot.x, view.rot.y, view.rot.z);
		glTranslated(0, 0, 0);
		gluLookAt(
			camera.x, camera.y, camera.z,  //��5 eye
			0, 0, 0, //���� center
			0, camera.y, 0); //���ʹ���(�ǵ� ����) up
					  //glTranslated(0, 0, 0);
		glLineWidth(2);
		glColor3f((float)255 / 255, (float)255 / 255, (float)255 / 255);
		glMatrixMode(GL_MODELVIEW);
		//��ǥ�� �׸���
		glPushMatrix();{

		glTranslated(0, -15, 0);
		glScalef(1, 0.001, 1);
		glutSolidCube(200);//���� �����
		glPopMatrix(); 
		}
		//��� ���� �׸���
		glColor3f((float)255 / 255, (float)0 / 255, (float)0 / 255);

			for (int i = 0; i < 3; i++) {
				glPushMatrix();
				if (i == 0) {
					glColor3f((float)255 / 255, (float)0 / 255, (float)0 / 255);
					glRotated(90, 1, 0, 0);
				}
				else if (i == 1) {
					glColor3f((float)0 / 255, (float)255 / 255, (float)0 / 255);
					glRotated(90, 0, 1, 0);
				}
				else if (i == 2) {
					glColor3f((float)0 / 255, (float)0 / 255, (float)255 / 255);
					glRotated(90, 0, 0, 1);
				}
				glScalef(1, 0.1, 0.1);
				glutSolidCube(40);
				glPopMatrix();
			}//��ǥ�� �׸���

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
		camera.degree -= 1;
		camera.z = cos(PI * camera.degree * 10 / 90);
		camera.y = sin(PI * camera.degree * 10 / 90);
		break;

	case 'X':
		camera.degree += 1;
		camera.z = cos(PI * camera.degree * 10 / 90);
		camera.y = sin(PI * camera.degree * 10 / 90);
		break;

	case 'y':
		camera.degree -= 1;
		camera.x = cos(PI * camera.degree * 10 / 90);
		camera.z = sin(PI * camera.degree * 10 / 90);
		break;
	
	case 'Y':
		camera.degree += 1;
		camera.x = cos(PI *camera.degree * 10 / 90);
		camera.z = sin(PI * camera.degree * 10 / 90);
		break;

	case 'z':
		camera.degree -= 1;
		camera.x = cos(PI *camera.degree * 10 / 90);
		camera.y = sin(PI * camera.degree * 10 / 90);
		break;
	
	case 'Z':
		camera.degree += 1;
		camera.x = cos(PI *camera.degree * 10 / 90);
		camera.y = sin(PI * camera.degree * 10 / 90);
		break;

	case 'w':
		camera.x += 1;
		break;

	case 's':
		camera.x -= 1;
		break;

	case 'd':
		camera.y += 1;
		break;

	case 'a':
		camera.y -= 1;
		break;

	case '+':
		camera.z += 1;
		break;

	case '-':
		camera.z -= 1;
		break;

	case 'i':
		camera.x = 1;
		camera.y = 1;
		camera.z = 1;
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
	glTranslatef(0, 0, -300);
	glMatrixMode(GL_MODELVIEW);

}