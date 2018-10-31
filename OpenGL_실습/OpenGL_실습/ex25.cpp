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
	int G;
	int B;
};
typedef struct Translate_pos {
	float x;
	float y;
	float z;
	float degree;
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
	Translate_pos sp_pos[36];
	Translate_pos pos;
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

typedef struct QUAD
{
	Color cl[8];//����
	Translate_pos pos[8];
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

int change_count;
int next_rot;
int st_help;
BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;


//ī�޶�-----------------

Shape camera;
Translate_pos EYE;
Translate_pos AT;
Translate_pos UP;

static int __x;
static int __y;
static int __z;


const void camera_custom
(double pos_x, double pos_y, double pos_z, //��ġ
	double degree, const double rot_x, const double rot_y, const double rot_z, //ȸ��
	const double move_x, const double move_y, const double move_z //������
) {


	EYE.x =
		((cos(rot_y) * cos(rot_z)) +
		(sin(rot_x) * sin(rot_y) * cos(rot_z) + cos(rot_x) * sin(rot_z)) +
			((((-1) * cos(rot_x)) * sin(rot_y) * cos(rot_z)) + (sin(rot_x) * sin(rot_z))));

	EYE.y =
		(((-1) * cos(rot_y) * sin(rot_z)) +
		(((-1) * sin(rot_x) * sin(rot_y) * sin(rot_z)) + (cos(rot_x) * cos(rot_z))) +
			((cos(rot_x) * sin(rot_y) * sin(rot_z)) + (sin(rot_x) * sin(rot_z))));

	EYE.z =
		(sin(rot_y) +
		(((-1) * sin(rot_x)) * cos(rot_y)) +
			(cos(rot_x) * cos(rot_y)));//stay

	AT.x = pos_x;
	AT.y = pos_y;
	AT.z = pos_z;

};

QUAD index;

//��������
BOOL depth;
int depth_count;
//�ø�
BOOL culling;
int culling_count;
//���̵�
BOOL shade;
int shade_count;
//��
Shape ball;

//������
Shape spring;

void SetupRC()
{
	//�ʱ�ȭ
	glTranslated(WideSize / 2, HighSize / 2, Z_Size / 2);
}
void main(int argc, char *argv[]) {
	//�ʱ�ȭ
	camera.rot.x = 0;
	camera.rot.y = 0;
	camera.rot.z = 0;

	camera.move.x = 0;
	camera.move.y = 0;
	camera.move.z = 0;

	EYE.x = 0, EYE.y = 0, EYE.z = 300;//EYE���� �ʱ�ȭ
	AT.x = 0, AT.y = 0, AT.z = 0;//EYE���� �ʱ�ȭ
	UP.x = 0, UP.y = 1, UP.z = 0;//EYE���� �ʱ�ȭ

	//�ʱ�ȭ
	ball.size = 25;
	ball.rot.degree = 1;
	spring.size = 40;

	for (int i = 0; i < 36; i++) {
		spring.sp_pos[i].x =spring.size * cos(i * PI * 10 / 36);
		spring.sp_pos[i].z =spring.size * sin(i * PI * 10 / 36);
		spring.sp_pos[i].y = i;

	}

	//-------------
	//�ε��� ���� ����
	index.pos[0].x = -1;
	index.pos[0].y = -1;
	index.pos[0].z = 1;
	
	index.cl[0].R = 1;
	index.cl[0].G = 0;
	index.cl[0].B = 0;

	index.pos[1].x = 1;
	index.pos[1].y = -1;
	index.pos[1].z = 1;

	index.cl[1].R = 1;
	index.cl[1].G = 0;
	index.cl[1].B = 1;

	index.pos[2].x = 1;
	index.pos[2].y = -1;
	index.pos[2].z = -1;

	index.cl[2].R = 0;
	index.cl[2].G = 0;
	index.cl[2].B = 1;

	index.pos[3].x = -1;
	index.pos[3].y = -1;
	index.pos[3].z = -1;

	index.cl[3].R = 0;
	index.cl[3].G = 0;
	index.cl[3].B = 0;

	index.pos[4].x = -1;
	index.pos[4].y = 1;
	index.pos[4].z = 1;

	index.cl[4].R = 1;
	index.cl[4].G = 1;
	index.cl[4].B = 0;

	index.pos[5].x = 1;
	index.pos[5].y = 1;
	index.pos[5].z = 1;

	index.cl[5].R = 1;
	index.cl[5].G = 1;
	index.cl[5].B = 1;

	index.pos[6].x = 1;
	index.pos[6].y = 1;
	index.pos[6].z = -1;

	index.cl[6].R = 0;
	index.cl[6].G = 1;
	index.cl[6].B = 1;

	index.pos[7].x = -1;
	index.pos[7].y = 1;
	index.pos[7].z = -1;

	index.cl[7].R = 0;
	index.cl[7].G = 1;
	index.cl[7].B = 0;

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

		//glRotated(-45, 0, 0, 1);
		glTranslated(camera.move.x, camera.move.y, camera.move.z);
		camera_custom(0, 0, 0, camera.rot.degree, camera.rot.x, camera.rot.y, camera.rot.z, camera.move.x, camera.move.y, camera.move.z);

		gluLookAt(
			EYE.x, EYE.y, EYE.z,  //��5 eye
			AT.x, AT.y, AT.z, //���� center
			0, 1, 0 //���ʹ���(�ǵ� ����) up
		);

		glPushMatrix();//-------------�׸��� �Է�--------------------------
		{
			glTranslated(0, 0, 0);
			glPushMatrix(); {
				glScaled(50, 50, 50);

				if (shade) {
					glShadeModel(GL_SMOOTH);
				}
				else {
					glShadeModel(GL_FLAT);
				}


				glBegin(GL_POLYGON); {//�Ʒ�

					glColor3f(index.cl[1].R, index.cl[1].G, index.cl[1].B);
					glVertex3f(index.pos[1].x, index.pos[1].y, index.pos[1].z);//Yellow

					glColor3f(index.cl[0].R, index.cl[0].G, index.cl[0].B);
					glVertex3f(index.pos[0].x, index.pos[0].y, index.pos[0].z);//Yellow

					glColor3f(index.cl[3].R, index.cl[3].G, index.cl[3].B);
					glVertex3f(index.pos[3].x, index.pos[3].y, index.pos[3].z);//Yellow

					glColor3f(index.cl[2].R, index.cl[2].G, index.cl[2].B);
					glVertex3f(index.pos[2].x, index.pos[2].y, index.pos[2].z);//Yellow

				}
				glEnd();

				glBegin(GL_POLYGON); {//��
					glColor3f(index.cl[0].R, index.cl[0].G, index.cl[0].B);
					glVertex3f(index.pos[0].x, index.pos[0].y, index.pos[0].z);//Yellow

					glColor3f(index.cl[4].R, index.cl[4].G, index.cl[4].B);
					glVertex3f(index.pos[4].x, index.pos[4].y, index.pos[4].z);//Yellow

					glColor3f(index.cl[7].R, index.cl[7].G, index.cl[7].B);
					glVertex3f(index.pos[7].x, index.pos[7].y, index.pos[7].z);//Yellow

					glColor3f(index.cl[3].R, index.cl[3].G, index.cl[3].B);
					glVertex3f(index.pos[3].x, index.pos[3].y, index.pos[3].z);//Yellow

				}
				glEnd();

				glBegin(GL_POLYGON); {//�޸�

					glColor3f(index.cl[2].R, index.cl[2].G, index.cl[2].B);
					glVertex3f(index.pos[2].x, index.pos[2].y, index.pos[2].z);//Yellow

					glColor3f(index.cl[3].R, index.cl[3].G, index.cl[3].B);
					glVertex3f(index.pos[3].x, index.pos[3].y, index.pos[3].z);//Yellow

					glColor3f(index.cl[7].R, index.cl[7].G, index.cl[7].B);
					glVertex3f(index.pos[7].x, index.pos[7].y, index.pos[7].z);//Yellow

					glColor3f(index.cl[6].R, index.cl[6].G, index.cl[6].B);
					glVertex3f(index.pos[6].x, index.pos[6].y, index.pos[6].z);//Yellow


				}
				glEnd();

				glBegin(GL_POLYGON); {//�����ʸ�
					glColor3f(index.cl[1].R, index.cl[1].G, index.cl[1].B);
					glVertex3f(index.pos[1].x, index.pos[1].y, index.pos[1].z);//Yellow

					glColor3f(index.cl[2].R, index.cl[2].G, index.cl[2].B);
					glVertex3f(index.pos[2].x, index.pos[2].y, index.pos[2].z);//Yellow

					glColor3f(index.cl[6].R, index.cl[6].G, index.cl[6].B);
					glVertex3f(index.pos[6].x, index.pos[6].y, index.pos[6].z);//Yellow					

					glColor3f(index.cl[5].R, index.cl[5].G, index.cl[5].B);
					glVertex3f(index.pos[5].x, index.pos[5].y, index.pos[5].z);//Yellow

				}
				glEnd();
				glPushMatrix();
				{
					glTranslated(0, index.pos[7].y, index.pos[7].z);
					glRotated(-index.move.x * 10, 1, 0, 0);
					glTranslated(0, -index.pos[7].y, -index.pos[7].z);
					
					glBegin(GL_POLYGON); {//�Ѳ�
					
					glColor3f(index.cl[4].R, index.cl[4].G, index.cl[4].B);
					glVertex3f(index.pos[4].x, index.pos[4].y, index.pos[4].z);//Yellow

					glColor3f(index.cl[5].R, index.cl[5].G, index.cl[5].B);
					glVertex3f(index.pos[5].x, index.pos[5].y, index.pos[5].z);//Yellow

					glColor3f(index.cl[6].R, index.cl[6].G, index.cl[6].B);
					glVertex3f(index.pos[6].x, index.pos[6].y, index.pos[6].z);//Yellow				

					glColor3f(index.cl[7].R, index.cl[7].G, index.cl[7].B);
					glVertex3f(index.pos[7].x, index.pos[7].y, index.pos[7].z);//Yellow

					}
					glEnd();
				}
				glPopMatrix();
				glPushMatrix();
				{
					glTranslated(0, index.pos[0].y, index.pos[0].z);
					glRotated(index.move.y * 10, 1, 0, 0);
					glTranslated(0, -index.pos[0].y, -index.pos[0].z);

					glBegin(GL_POLYGON); {//�ո�
				
						glColor3f(index.cl[0].R, index.cl[0].G, index.cl[0].B);
						glVertex3f(index.pos[0].x, index.pos[0].y, index.pos[0].z);//Yellow

						glColor3f(index.cl[1].R, index.cl[1].G, index.cl[1].B);
						glVertex3f(index.pos[1].x, index.pos[1].y, index.pos[1].z);//Yellow

						glColor3f(index.cl[5].R, index.cl[5].G, index.cl[5].B);
						glVertex3f(index.pos[5].x, index.pos[5].y, index.pos[5].z);//Yellow


						glColor3f(index.cl[4].R, index.cl[4].G, index.cl[4].B);
						glVertex3f(index.pos[4].x, index.pos[4].y, index.pos[4].z);//Yellow
				}
				glEnd();
				}
				glPopMatrix();
			}
			glPopMatrix();//���� �׸��� ��

			//���׸���
			glPushMatrix();
			{
				glTranslated(0 ,0 ,0);
				
				glPushMatrix();
				glTranslated(0, ball.move.y, 0);
				glRotated(ball.rot.degree *  ball.move.y * PI,1,0, 0);
				glColor3f(0.5, 0, 0.5);
				glutSolidSphere(ball.size, 8, 8);
				glPopMatrix();
				
				glPushMatrix();
				glTranslated(0, 0, ball.move.z);
				glRotated(ball.rot.degree *  ball.move.z * PI, 1, 0, 0);
				glColor3f(0, 0, 0);
				glutSolidSphere(ball.size, 8, 8);
				glPopMatrix();

			}
			glPopMatrix();//���׸��� ��

			glPushMatrix();//������ �׸���
			{
				glTranslated(0, 0, 0);
				glLineWidth(2);

				glColor3f(1, 1, 1);

				glBegin(GL_LINES);
				for (int i = 0; i < 35; i++) {
				glVertex3f(spring.sp_pos[i].x, spring.sp_pos[i].y, spring.sp_pos[i].z);
				glVertex3f(spring.sp_pos[i+ 1].x, spring.sp_pos[i + 1].y, spring.sp_pos[i + 1].z);
				}
				glEnd();

				//glutWireSphere(100, 8, 8);
			}
			glPopMatrix();
		}
		glPopMatrix();
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

	if (index.b.b_x) {
		if (index.move.x < 9) {
			if (index.move.x == 8) {
				ball.b.b_x = true;
			}
			index.move.x += 1;
		}
	}
	else {
		if (index.move.x > 0) {
			
			index.move.x -= 1;
		}
	}
	
	//���� ������
	if (ball.b.b_x) {
		if (ball.size * 2 * PI > ball.move.y) {
			ball.rot.degree = -1;
			ball.move.y += 2;
		}
		if (spring.sp_pos[35].y <= ball.size * 2 * PI) {
			for (int i = 0; i < 36; i++) {
				spring.sp_pos[i].y += 0.1 + i;
			}
		}
		
	}
	else {
		if (0 < ball.move.y) {
			if (ball.move.y == 2) {
				index.b.b_x = false;
			}
			ball.rot.degree = -1;
			ball.move.y -= 2;
		}
		if (spring.sp_pos[35].y > 36) {
			for (int i = 0; i < 36; i++) {
				spring.sp_pos[i].y -= 0.1 + i;
			}
		}

	}

	if (index.b.b_y) {
		if (index.move.y < 9) {
			if (index.move.y == 8) {
				ball.b.b_y = true;
			}
			index.move.y += 1;
		}
	}
	else {
		if (index.move.y > 0) {

			index.move.y -= 1;
		}


	}

	//���� ������
	if (ball.b.b_y) {
		if (ball.size * 2 * PI > ball.move.z) {
			ball.rot.degree = 1;
			ball.move.z += 2;
		}
	}
	else {
		if (0 < ball.move.z) {
			if (ball.move.z == 2) {
				index.b.b_y = false;
			}
			ball.rot.degree = 1;
			ball.move.z -= 2;
		}
	}
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
		__x = 1;
		camera.rot.x -= 0.1;
		break;
	case 'X':
		camera.rot.x += 0.1;
		break;

	case 'y':
		__y = 1;
		camera.rot.y -= 0.1;
		break;
	case 'Y':
		camera.rot.y += 0.1;
		break;

	case 'z':
		__z = 1;
		camera.rot.z -= 0.1;
		break;
	case 'Z':
		camera.rot.z += 0.1;
		break;
		
		//move
	case 'w':
		camera.move.y += 1;
		break;
	case 'a':
		camera.move.x -= 1;
		break;

	case 's':
		camera.move.y -= 1;
		break;

	case 'd':
		camera.move.x += 1;
		break;
	case '+':
		camera.move.z += 1;
		break;
	case '-':
		camera.move.z -= 1;
		break;
	case 'i':
		camera.rot.degree = 0;
		camera.move.x = 0;
		camera.move.y = 0;
		camera.move.z = 0;
		camera.rot.x = 0;
		camera.rot.y = 0;
		camera.rot.z = 0;

		__x = 0;
		__y = 0;
		__z = 0;

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

	//�Ѳ� �ݱ�
	case 'k':
		index.b.b_x = true;
		break;
	case 'K':
		ball.b.b_x = false;
		break;

		//�ո� �ݱ�
	case 'j':
		index.b.b_y = true;
		break;
	case 'J':
		ball.b.b_y = false;
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


	if (!ani) {
		//glRotatef(30, 0, 1, 0);
		gluPerspective(60.0f, w / h, 1.0, 1000.0);

		glTranslated(0.0, 0.0, -300.0);     // ���� ������ ȭ�� �������� �̵��Ͽ� �þ߸� Ȯ���Ѵ�.
	}
	else {
		glOrtho(0, WideSize, HighSize, 0, -Z_Size / 2, Z_Size / 2); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!
		glTranslated(WideSize / 2, HighSize / 2, 0);
		//glRotatef(-60, 1, 0, 0);

	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}