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
void Passive(int x , int y);
BOOL Mouse_Act_On; //���콺 Ȱ��ȭ ����

				   //Ű���� ����
void Keyboard(unsigned char key, int x, int y);
BOOL Keyboard_Act_On; //Ű���� Ȱ��ȭ ����

					  //�� ����

#define PT 20//���� ���� ����
#define PI 3.141592 //����

BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;

//ī�޶�
Cam camera;
int view_trans;
#define FRONT 0
#define TOP 1

//��������
BOOL depth;
int depth_count;
//�ø�
BOOL culling;
int culling_count;
//���̵�
BOOL shade;
int shade_count;

struct Look_int
{
	int x;
	int y;
	int z;
	int degree;
};

//��ŷ �˻�
Shape picking[3];
Look_int m_pos;
int pick_num_save = 0;
void SetupRC()
{
	m_pos.degree = 50;

	picking[0].move.x = 0;
	picking[0].move.y = 0;
	picking[0].move.z = 0;

	picking[1].move.x = 0;
	picking[1].move.y = 0;
	picking[1].move.z = 100;
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
	glutPassiveMotionFunc(Passive);
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
			

			glPushMatrix();
			glTranslated(picking[0].move.x, picking[0].move.y, picking[0].move.z);
			glColor3f((float) 100/ 255, (float) 100/ 255, (float) 200/ 255);
			glutSolidCube(50);
			glPopMatrix();

			glPushMatrix();
			glTranslated(picking[1].move.x, picking[1].move.y, picking[1].move.z);
			glColor3f((float)50 / 255, (float)50 / 255, (float)200 / 255);
			glutSolidCube(50);
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (view_trans == FRONT) {
		m_pos.x = x - WideSize / 2;
		m_pos.y = (HighSize / 2 - y);
	}
	else if (view_trans == TOP) {
		m_pos.x = x - WideSize / 2;
		m_pos.y = -(HighSize / 2 - y);
	}


	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		printf("---------------------------------------\n x : %d , y : % d \n", m_pos.x , m_pos.y);
		if (view_trans == FRONT) {//front view
			for (int i = 0; i < 2; i++) {
				if ((picking[i].move.y - m_pos.degree < m_pos.y) && (m_pos.y < picking[i].move.y + m_pos.degree)) {
					if ((picking[i].move.x - m_pos.degree <  m_pos.x) && (m_pos.x < picking[i].move.x + m_pos.degree)) {
						Mouse_Act_On = true;
						pick_num_save = i;
						printf("picking-\n");
					}
				}
			}
		}
		else if (view_trans == TOP) {//front view
			for (int i = 0; i < 2; i++) {
				if ((picking[i].move.z - m_pos.degree < m_pos.z) && (m_pos.z < picking[i].move.z + m_pos.degree)) {
					if ((picking[i].move.x - m_pos.degree <  m_pos.x) && (m_pos.x < picking[i].move.x + m_pos.degree)) {
						Mouse_Act_On = true;
						pick_num_save = i;
						printf("---------------------------------------\n");

					}
				}
			}
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (view_trans == FRONT) {//front view
			picking[pick_num_save].move.x = m_pos.y;
			picking[pick_num_save].move.y = m_pos.x;
			pick_num_save = 3;
			Mouse_Act_On = false;
		}
		else if (view_trans == TOP) {//front view
			picking[pick_num_save].move.z = m_pos.y;
			picking[pick_num_save].move.x = m_pos.x;
			pick_num_save = 3; 
			Mouse_Act_On = false;
			
		}
	}
	glutPostRedisplay();

}
void Motion(int x, int y) {
//	m_pos.x = x - WideSize / 2;
//	m_pos.y = (HighSize / 2 - y);

	if (Mouse_Act_On) {
		if (view_trans == FRONT) {//front view

			m_pos.x = x - WideSize / 2;
			m_pos.y = (HighSize / 2 - y);
			
			picking[pick_num_save].move.x = m_pos.y;
			picking[pick_num_save].move.y = m_pos.x;
		
		}
		else if (view_trans == TOP) {//top view
			m_pos.x = x - WideSize / 2;
			m_pos.y = -(HighSize / 2 - y);

			picking[pick_num_save].move.x = m_pos.x;
			picking[pick_num_save].move.z = m_pos.y;
}
		}
}

void Passive(int x , int y) {
	printf("x %d ", x - WideSize / 2);
	printf("y %d \n", (HighSize / 2 - y));
	m_pos.x = x - WideSize / 2;
	m_pos.y = (HighSize / 2 - y);

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
			printf("�������� On \n");
		}
		else {
			ani = FALSE;//ȸ���ϴ°� Ʈ���
			Reshape(WideSize, HighSize);
			printf("�������� off \n");
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
	//���� ��ȯ
	case 'l':
	case 'L':
		ani_count++;
		if (ani_count % 2) {
			camera.Init();
			camera.rotateEye(0, 90, 0);
			view_trans = TOP;
			printf("top \n");
		}
		else {
			camera.Init();
			camera.rotateEye(0, 0, 90);
			view_trans = FRONT;
			printf("front \n");
		}
		
	break;

	case 'k':
		camera.Init();
		camera.rotateEye(10, 45,0);
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (!ani) {
		//glRotatef(30, 0, 1, 0);
		gluPerspective(60.0f, w / h, 1.0, 1000.0);

		glTranslated(0.0, 0.0, -500.0);     // ���� ������ ȭ�� �������� �̵��Ͽ� �þ߸� Ȯ���Ѵ�.
	}
	else {
		glOrtho(-WideSize/2, WideSize/2, -HighSize/2, HighSize/2, -400, 400); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!
		glTranslated(0.0, 0.0, 0.0);     // ���� ������ ȭ�� �������� �̵��Ͽ� �þ߸� Ȯ���Ѵ�.
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}