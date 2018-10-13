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
int TIme_count;

int move_count; //Ÿ�̸� ī��Ʈ
int move_count2; //Ÿ�̸� ī��Ʈ
int move_count3; //Ÿ�̸� ī��Ʈ
int move_count4; //Ÿ�̸� ī��Ʈ

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
	Translate_pos sa;
	int size;
	BOOL select;
	int height;
	int slice;
	int stacks;

};

int click_count;

BOOL Save = false;
BOOL ani = FALSE;
int ani_count = 0;
BOOL Look = FALSE;
int Look_count;

Shape sh[PT];
Shape sh2[PT];
Shape camera;

//ȸ�� ���� ���� �Լ�
BOOL Sel_Rot;
int old_rot;//�ʱⰪ
int next_rot;//����
			 //--------------

typedef struct Po
{
	float x;
	float y;
};

void rot_custom(int init_sel, int sel, int degree, int rot_x, int rot_y, int rot_z, int move_x, int move_y, int move_z) {
	//initialize �ʱ�ȭ
	//x = 0
	//y = 1
	//z = 2
	int init = (PI * 10 / 180);

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
	GLfloat all_rotate[16] = {
		(cos(rot_y * init) * cos(rot_z * init)) ,
		(((-1) * cos(rot_y * init)) * sin(rot_z * init)) ,
		(sin(rot_y * init)),
		0,
		(sin(rot_x * init) * sin(rot_y * init) * cos(rot_z * init) + cos(rot_x * init) * sin(rot_z * init)) ,
		(((-1) * sin(rot_x * init)) * sin(rot_y * init) * cos(rot_z * init) + cos(rot_x * init) * cos(rot_z * init)) ,
		(cos(rot_z * init) * ((-1)* sin(rot_x * init))),
		0
		(((-1)* cos(rot_x * init)) * sin(rot_y * init) * cos(rot_z * init) + sin(rot_x * init) * sin(rot_z * init)) ,
		(cos(rot_x * init) * sin(rot_y * init) * sin(rot_z * init) + sin(rot_x * init) * cos(rot_z * init)),
		(cos(rot_x * init) * cos(rot_y)),
		0,
		0,0,0,1
	};

	if (sel == 1) {
		glMultMatrixf(all_rotate);

	}
};
void main(int argc, char *argv[]) {

	for (int i = 0; i < PT; i++) {
		sh[i].pos.x = 100 * cos(PI * i * 10 / 180);
		sh[i].pos.z = 100 * sin(PI * i * 10 / 180);
		sh[i].pos.y = 1;
	}
	for (int i = 0; i < PT; i++) {
		sh2[i].pos.x = 100 * cos(PI * i * 10 / 180);
		sh2[i].pos.z = 100 * sin(PI * i * 10 / 180);
		sh2[i].pos.y = 1;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);//������ ���� ��ǥ
	glutInitWindowSize(WideSize, HighSize); //������ ���� ũ��
	glutCreateWindow("ex11");
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
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPointSize(4);
	glLineWidth(1);

	glMatrixMode(GL_MODELVIEW);
	//��� ����
	glColor3f((float)100 / 255, (float)200 / 255, (float)100 / 255);
	//��ǥ�� �׸���

	glPushMatrix(); { //���� ���� ����

		rot_custom(old_rot, next_rot, camera.rot.degree, camera.rot.x, camera.rot.y, camera.rot.z);
		gluLookAt(
			camera.move.x, camera.move.y, camera.move.z,  //��5 eye
			0, 0, 0, //���� center
			0, 1, 0); //���ʹ���(�ǵ� ����) up
					  //glTranslated(0, 0, 0);


		glPushMatrix();
		if (Look) {//�ָ��� �� �ָ���
			glutWireSphere(40, 10, 10);
		}
		else {
			glutSolidSphere(40, 10, 10);
		}
		glPushMatrix(); {
			glScalef(2, 2, 2);
			glColor3f((float)67 / 255, (float)116 / 255, (float)217 / 255);
			for (int i = 0; i < 3; i++) {
				if (i == 0) {
					;
				}
				else if (i == 1) {
					glRotatef(45, 0, 0, 1);
				}
				else if (i == 2) {
					glRotatef(-90, 0, 0, 1);
				}
				for (int i = 0; i < PT - 1; i++) {

					glBegin(GL_LINES);
					glVertex3d(sh[i].pos.x, sh[i].pos.y, sh[i].pos.z);
					glVertex3d(sh[i + 1].pos.x, sh[i + 1].pos.y, sh[i + 1].pos.z);
					glEnd();
				}

				//-------

				glPushMatrix(); {
					if (i == 0) {
						glTranslated(sh[move_count].pos.x, sh[move_count].pos.y, sh[move_count].pos.z);
					}
					else if (i == 1) {
						glTranslated(sh[move_count3].pos.x, sh[move_count3].pos.y, sh[move_count3].pos.z);
					}
					else if (i == 2) {
						glTranslated(sh[move_count4].pos.x, sh[move_count4].pos.y, sh[move_count4].pos.z);

					}
					glPushMatrix();
					if (Look) {//�ָ��� �� �ָ���
						glutWireSphere(10, 10, 10);
					}
					else {
						glutSolidSphere(10, 10, 10);
					}
					glScalef(0.5, 0.5, 0.5);

					glColor3f((float)67 / 255, (float)116 / 255, (float)217 / 255);

					for (int i = 0; i < PT - 1; i++) {

						glBegin(GL_LINES);
						glVertex3d(sh2[i].pos.x, sh2[i].pos.y, sh2[i].pos.z);
						glVertex3d(sh2[i + 1].pos.x, sh2[i + 1].pos.y, sh2[i + 1].pos.z);
						glEnd();

					}
					glPopMatrix();

					//------
					glPushMatrix(); {
						glScalef(0.5, 0.5, 0.5);
						glTranslated(sh2[move_count2].pos.x, sh2[move_count2].pos.y, sh2[move_count2].pos.z);

						glColor3f((float)267 / 255, (float)116 / 255, (float)100 / 255);
						glScalef(0.5, 0.5, 0.5);
						if (Look) {//�ָ��� �� �ָ���
							glutWireSphere(10, 10, 10);
						}
						else {
							glutSolidSphere(10, 10, 10);
						}


					}glPopMatrix(); //���� ���� �ݱ�

									//-----


				}glPopMatrix(); //���� ���� �ݱ�

								//------
				glColor3f((float)67 / 255, (float)116 / 255, (float)217 / 255);

			}


		}glPopMatrix();
		glPopMatrix(); //���� ���� �ݱ�

	}glPopMatrix(); //���� ���� �ݱ�



	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {

	if (Mouse_Act_On) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
		}

	}

}
void Timerfunction(int value) {

	//Ÿ�̸� ���� �Է�
	TIme_count++;

	move_count = TIme_count % PT;
	move_count2 = (TIme_count * 2) % PT;
	move_count3 = (TIme_count * 3) % PT;
	move_count4 = (TIme_count * 2) % PT;

	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.
	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case '1'://�ָ��� ����
		Look_count++;
		if (Look_count % 2 == 1) {
			Look = TRUE;//ȸ���ϴ°� Ʈ���
		}
		else {
			Look = FALSE;//ȸ���ϴ°� Ʈ���
		}
		break;
	case '2'://�������� ����
		ani_count++;
		if (ani_count % 2 == 1) {
			ani = TRUE;//ȸ���ϴ°� Ʈ���
		}
		else {
			ani = FALSE;//ȸ���ϴ°� Ʈ���
		}
		break;
		//---------ī�޶�
	case 'x':
		camera.rot.degree -= 1;
		old_rot = next_rot;
		next_rot = 0;
		Sel_Rot = TRUE;
		break;
	case 'X':
		camera.rot.degree -= 1;
		old_rot = next_rot;
		next_rot = 0;
		Sel_Rot = TRUE;
		break;
	case 'y':
		camera.rot.degree += 1;
		old_rot = next_rot;
		next_rot = 1;
		break;
	case 'Y':
		camera.rot.degree += 1;
		old_rot = next_rot;
		next_rot = 1;
		break;
	case 'z':
		camera.rot.degree += 1;
		old_rot = next_rot;
		next_rot = 2;
		break;
	case 'Z':
		camera.rot.degree += 1;
		camera.rot.degree += 1;
		old_rot = next_rot;
		next_rot = 2;
		break;
	case 'w':
		camera.move.y += 2;
		break;
	case 'a':
		camera.move.x -= 2;
		break;
	case 's':
		camera.move.y -= 2;
		break;
	case 'd':
		camera.move.x += 2;
		break;
	case '+':
		camera.move.z += 2;
		break;
	case '-':
		camera.move.z -= 2;
		break;
	case 'i':
		camera.rot.degree = 0;
		camera.move.x = 0;
		camera.move.y = 0;
		camera.move.z = 0;
		break;

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
	glViewport(-1, -1, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (ani) {
		gluPerspective(60.0f, w / h, 1.0, 1000.0);
		glTranslatef(0.0, 0.0, -300.0);     // ���� ������ ȭ�� �������� �̵��Ͽ� �þ߸� Ȯ���Ѵ�.
	}
	else {
		glOrtho(0, WideSize, HighSize, 0, -Z_Size / 2, Z_Size / 2); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!
		glTranslated(WideSize / 2, HighSize / 2, 0);
		glRotatef(30, 1, 0, 0);

	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}