#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>
#include "base_struct.h"
#include "camera.h"
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
int move_count; //Ÿ�̸� ī��Ʈ

				//���콺 ����
void Mouse(int button, int state, int x, int y);
POINT ms_click; //���콺 ��ǥ
void Motion(int x, int y);
BOOL Mouse_Act_On; //���콺 Ȱ��ȭ ����

				   //Ű���� ����
void Keyboard(unsigned char key, int x, int y);
BOOL Keyboard_Act_On; //Ű���� Ȱ��ȭ ����

					  //�� ����

#define PT 20//���� ���� ����
#define PI 3.141592 //����



int change_count;
int next_rot;
int st_help;
BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;


//ī�޶�-----------------

Translate_pos EYE;
Translate_pos AT;
Translate_pos UP;

static int __x;
static int __y;
static int __z;

//ī�޶�
Cam camera;

//��������
BOOL depth;
int depth_count;
//�ø�
BOOL culling;
int culling_count;
//���̵�
BOOL shade;
int shade_count;

int sub;
int ctrl_pt_click  = 0;//Ŭ�� Ƚ��
int ctrl_pt_play = 0; //������ ���� ����
GLfloat ctrlpoints[19][3] = { { -40.0, -40.0, 0.0 },{ -20.0, 40.0, 0.0 },{ 20.0, 20.0, 0.0 } ,{ 40.0, 0.0, 0.0 } };

void SetupRC()
{
	//�� �ʱ�ȭ
	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 3; j++) {
			ctrlpoints[i][j] = 0;
		}
	}
	
}
void main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);//������ ���� ��ǥ
	glutInitWindowSize(WideSize, HighSize); //������ ���� ũ��
	glutCreateWindow("ex16");
	// - �������� ���� ���� �����ϱ�
	//���� �׸���
	SetupRC();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(100, Timerfunction, 1);
	srand(time(NULL));
	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{

	glFrontFace(GL_CW);

	//���� ����
	if (depth) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}

	//�ø� ����
	if (culling) {
		//glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
	}
	else {
		//glFrontFace(GL_CCW);
		glDisable(GL_CULL_FACE);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();//-----------------------------------
	{
		camera.drawCamera();


		glPushMatrix();//-------------�׸��� �Է�--------------------------
		{
			//���콺 ������

				if (shade) {
					glShadeModel(GL_SMOOTH);
				}
				else {
					glShadeModel(GL_FLAT);
				}
				//���� �׸���
				
				for (int i = 0; i < ctrl_pt_play; i++) {
					glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[i * 3][0]);
					glEnable(GL_MAP1_VERTEX_3);//� �׸��� ���� Ȱ��ȭ

					glBegin(GL_LINE_STRIP);
					for (int i = 0; i <= 10; i++) {
						glEvalCoord1f((GLfloat)i / 10.0);
					}
					glEnd();

				}
				
				glDisable(GL_MAP1_VERTEX_3);

				glPointSize(5.0);
				glColor3f(0, 0, 1);
				glBegin(GL_POINTS);
				for (int i = 0; i < ctrl_pt_click; i++) {
					glVertex3fv(&ctrlpoints[i][0]);
				}
				glEnd();
		}
		glPopMatrix();
	}
	glPopMatrix();
	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		ctrlpoints[ctrl_pt_click][0] = x;
		ctrlpoints[ctrl_pt_click][1] = y;
		ctrlpoints[ctrl_pt_click][2] = 0;
		if (ctrl_pt_click < 19) {
			ctrl_pt_click++;
		}
		if (ctrl_pt_click % 3 == 0) {
			ctrl_pt_play++;
		}

	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		for (int i = 0; i < ctrl_pt_click; i++) {
			if ((ctrlpoints[i][1] - 5 < y) && (y < ctrlpoints[i][1] + 5)) {
				if ((ctrlpoints[i][0] - 5 < x) && (x < ctrlpoints[i][0] + 5)) {
					Mouse_Act_On = TRUE;
					sub = i;
				}
			}
		}

	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
					ctrlpoints[sub][0] = x;
					ctrlpoints[sub][1] = y;
					Mouse_Act_On = FALSE;
			
	}


	glutPostRedisplay();

}
void Motion(int x, int y) {
	if (Mouse_Act_On) {

		ctrlpoints[sub][0] = x;
		ctrlpoints[sub][1] = y;
	
	}
}

void Timerfunction(int value) {


	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.
	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���

}
int ttt;
int ani_count;
void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
		//---------ī�޶�
		//rotate
	case 'x':
		camera.rotateEye(1, 0, 0);
		break;
	case 'X':
		camera.rotateEye(-1, 0, 0);
		break;

	case 'y':
		camera.rotateEye(0, 1, 0);
		break;
	case 'Y':
		camera.rotateEye(0, -1, 0);
		break;

	case 'z':
		camera.rotateEye(0, 0, 1);
		break;
	case 'Z':
		camera.rotateEye(0, 0, -1);
		break;

		//move
	case 'w':
		camera.moveEye(0, 1, 0);
		break;
	case 'a':
		camera.moveEye(-1, 0, 0);
		break;

	case 's':
		camera.moveEye(0, -1, 0);
		break;

	case 'd':
		camera.moveEye(1, 0, 0);
		break;
	case '+':
		camera.moveEye(0, 0, 1);

		break;
	case '-':
		camera.moveEye(0, 0, -1);
		break;
	case 'i':
		camera.Init();
		break;

		// z�� �״�� camera.z 

		//-----------ī�޶� �� --------

	case '2'://�������� ����
		ani_count++;
		if (ani_count % 2 == 1) {
			ani = TRUE;//ȸ���ϴ°� Ʈ���
			Reshape(WideSize, HighSize);
		}
		else {
			ani = FALSE;//ȸ���ϴ°� Ʈ���
			Reshape(WideSize, HighSize);
		}
		break;

		//------------���� ��

	case '3'://��������
		depth_count++;
		if (depth_count % 2 == 1) {
			depth = true;
			printf("���� On \n");
		}
		else {
			printf("���� Off \n");
			depth = false;
		}

		break;
	case '4'://�ø�
		culling_count++;
		if (culling_count % 2 == 1) {
			culling = true;
			printf("�ø� On \n");
		}
		else {
			printf("�ø� Off \n");
			culling = false;
		}

		break;
	case '5'://���̵�
		shade_count++;
		if (shade_count % 2 == 1) {
			shade = true;
			printf("���̵� On \n");
		}
		else {
			printf("���̵� Off \n");
			shade = false;
		}

		break;

	case 'r':
	case 'R':
		for (int i = 0; i < 19; i++) {
			for (int j = 0; j < 3; j++) {
				ctrlpoints[i][j] = 0;
			}
		}
		ctrl_pt_click = 0;
		ctrl_pt_play = 0;

		break;

	default:
		;
		break;
	}
	glutPostRedisplay();

}

GLvoid Reshape(int w, int h)
{
	glViewport(-1, -1, w, h);
	glOrtho(0, WideSize, HighSize, 0, -Z_Size / 2, Z_Size / 2); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!

	
}