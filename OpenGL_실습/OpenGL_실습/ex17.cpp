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
int move_count; //Ÿ�̸� ī��Ʈ
int Time_count2;
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

#define PT 36//���� ���� ����
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
//ȸ�� ���� ���� �Լ�
BOOL Sel_Rot;
int old_rot;//�ʱⰪ
int next_rot;//����
//--------------

int st_help;
BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;

Shape camera;
Shape sh[36];
Shape sh_2[36];
Shape view;


GLdouble identity[16] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

void SetupRC()
{
	//�ʱ�ȭ
	glTranslated(WideSize / 2, HighSize / 2, Z_Size / 2);
}
void main(int argc, char *argv[]) {
	//�ʱ�ȭ
	for (int i = 0; i < PT; i++) {
		sh[i].pos.z = 50 * cos(PI * i / 15);
		sh[i].pos.x = 50 * sin(PI * i/ 15);
		sh[i].pos.y = 0;
	}
	for (int i = 0; i < PT; i++) {
		sh_2[i].pos.z = 30 * cos(PI * i / 15);
		sh_2[i].pos.x = 30 * sin(PI * i / 15);
		sh_2[i].pos.y = 0;
	}
	old_rot = 3;
	next_rot = 3;

	camera.pos.x = 0;
	camera.pos.y = 1;
	camera.pos.z = 1;

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


void rot_custom(int init_sel ,int sel, int degree) {
	//initialize �ʱ�ȭ
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
	//glLoadMatrixd(x_rotate);
	//glLoadMatrixd(y_rotate);
	//glLoadMatrixd(z_rotate);
	
	if (sel == 0) {
		if (init_sel == 1) {
			glMultMatrixd(y_rotate);
		}
		else if (init_sel == 2) {
			glMultMatrixd(z_rotate);
		}
		glMultMatrixd(x_rotate);
		
	}
	else if (sel == 1) {
		if (init_sel == 0) {
			glMultMatrixd(x_rotate);
		}
		else if (init_sel == 2) {
			glMultMatrixd(z_rotate);
		}
		glMultMatrixd(y_rotate);
	}
	else if (sel == 2) {
		if (init_sel == 0) {
			glMultMatrixd(x_rotate);
		}
		else if (init_sel == 1) {
			glMultMatrixd(y_rotate);
		}
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
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	glPushMatrix();
	//glRotated(90, 1, 0, 0);
	rot_custom(old_rot, next_rot, camera.rot.degree);
	gluLookAt(
		1, 0, 1,  //��5 eye
		0, 0, -200, //���� center
		0, 1, 0); //���ʹ���(�ǵ� ����) up
				  //glTranslated(0, 0, 0);
	glPushMatrix(); {
		
		glLineWidth(2);
		glPushMatrix();//ī�޶� ����
		{

				glTranslated(0, 0, 0);
				glColor3f((float)61 / 255, (float)183 / 255, (float)204 / 255);
				glutWireSphere(20, 15, 15);
			
			glPushMatrix();//��� �˵� �׸���
			{
			
				for (int i = 0; i < 3; i++) {
					if (i == 0) {
						glColor3f((float)0 / 255, (float)130 / 255, (float)153 / 255);
					}
					else if (i == 1) {
						glColor3f((float)0 / 255, (float)130 / 255, (float)0/ 255);
						glRotatef(45, 0, 1, 0);
					}
					else if (i == 2) {
						glRotatef(90, 0, 1, 0);
					}


					
					for (int i = 0; i < PT - 1; i++) {
						glBegin(GL_LINES);
						glVertex3d(sh[i].pos.x, sh[i].pos.y, sh[i].pos.z);
						glVertex3d(sh[i + 1].pos.x, sh[i + 1].pos.y, sh[i + 1].pos.z);
						glEnd();
					}

				}

			}
			glPopMatrix();//��� �˵� �׸��� ��

			glPushMatrix();//���� �׸���
			{
					
					glTranslated(sh[move_count].pos.x, sh[move_count].pos.y, sh[move_count].pos.z);
					glColor3f((float)67 / 255, (float)116 / 255, (float)217 / 255);
					glutWireSphere(10, 15, 15);
					glColor3f((float)0 / 255, (float)130 / 255, (float)153 / 255);
					for (int i = 0; i < PT - 1; i++) {//�˵�
						glBegin(GL_LINES);
						glVertex3d(sh_2[i].pos.x, sh_2[i].pos.y, sh_2[i].pos.z);
						glVertex3d(sh_2[i + 1].pos.x, sh_2[i + 1].pos.y, sh_2[i + 1].pos.z);
						glEnd();
						
					}
					glPushMatrix();

					glTranslated(sh_2[move_count].pos.x, sh_2[move_count].pos.y, sh_2[move_count].pos.z);
					glColor3f((float)67 / 255, (float)116 / 255, (float)217 / 255);
					glutWireSphere(5, 10, 10);
					
					glPopMatrix();

			}
			glPopMatrix();//���� �׸��� ��

			//�׸���
			for (int i = 1; i < 4; i++) {
				glPushMatrix();
				glColor3f((float)0 / 255, (float)0 / 255, (float)255 / 255);

				glPopMatrix();
			}

			//glRotatef(Time_count, 0, 1, 0);

		}glPopMatrix(); //ī�޶� ���� ��

	}glPopMatrix();//�׸��� ��
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

	if (Time_count2 == PT) {
		Time_count2 = 0;
	}
	Time_count2++;

	if (move_count == PT) {
		move_count = 0;
	}
	move_count++;


	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.
	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	
	//------------------ī�޶�------------------------
	case 'x':
		camera.rot.degree -= 1;
		if (!Sel_Rot) {//�ʱ� ���ýÿ�
			old_rot = 0;
			next_rot = old_rot;
			Sel_Rot = TRUE;
		}
		else {
			next_rot = 0;
		}
		break;
	case 'X':
		camera.rot.degree += 1;
		if (!Sel_Rot) {//�ʱ� ���ýÿ�
			old_rot = 0;
			next_rot = old_rot;
			Sel_Rot = TRUE;
		}
		else {
			next_rot = 0;
		}
		break;
	
	case 'y':
		camera.rot.degree += 1;
		if (!Sel_Rot) {//�ʱ� ���ýÿ�
			old_rot = 1;
			next_rot = old_rot;
			Sel_Rot = TRUE;
		}
		else {
			next_rot = 1;
		}
		break;
	case 'Y':
		camera.rot.degree += 1;
		if (!Sel_Rot) {//�ʱ� ���ýÿ�
			old_rot = 1;
			next_rot = old_rot;
			Sel_Rot = TRUE;
		}
		else {
			next_rot = 1;
		}
		break;
	case 'z':
		camera.rot.degree += 1;
		if (!Sel_Rot) {//�ʱ� ���ýÿ�
			old_rot = 2;
			next_rot = old_rot;
			Sel_Rot = TRUE;
		}
		else {
			next_rot = 2;
		}
		break;
	case 'Z':
		camera.rot.degree += 1;
		if (!Sel_Rot) {//�ʱ� ���ýÿ�
			old_rot = 2;
			next_rot = old_rot;
			Sel_Rot = TRUE;
		}
		else {
			next_rot = 2;
		}break;


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
	gluPerspective(45, WideSize / HighSize, Z_Size/2, Z_Size ); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!
	glTranslated(0, 0, -300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}