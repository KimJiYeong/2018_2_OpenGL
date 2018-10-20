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
int next_rot;
int st_help;
BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;

Shape tra;
int rot_count;
int rot_command;

Shape shape[2];
Shape view;


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



void SetupRC()
{
	//�ʱ�ȭ
	glTranslated(WideSize / 2, HighSize / 2, Z_Size/2);
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
	
	glPushMatrix();//-----------------------------------
	//glRotated(-45, 0, 0, 1);
	glTranslated(camera.move.x, camera.move.y, camera.move.z);
	camera_custom(0, 0, 0, camera.rot.degree, camera.rot.x, camera.rot.y, camera.rot.z, camera.move.x, camera.move.y, camera.move.z);

	gluLookAt(
		EYE.x, EYE.y, EYE.z,  //��5 eye
		AT.x, AT.y, AT.z, //���� center
		0, 1, 0 //���ʹ���(�ǵ� ����) up
	);

	glPushMatrix();//---------------------------------------
				  
				  //glTranslated(0, 0, 0);
	glLineWidth(2);
	glColor3f((float)255 / 255, (float)255 / 255, (float)255 / 255);
	glMatrixMode(GL_MODELVIEW);
	//��ǥ�� �׸���
	glPushMatrix();

	glTranslated(0, -15, 0);
	glScalef(1, 0.001, 1);
	glutSolidCube(200);//���� �����
		
	glPopMatrix();//��ǥ�� �׸���
	
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
	
	
	for (int i = 0; i < 2; i++) {
		glPushMatrix();//���� �׸���
		glRotatef(move_count, 0, 1, 0);
		if (i == 0) {//ù��° ����
			glTranslated(shape[i].pos.x, 0, 0);
			glRotatef(shape[i].rot.degree, shape[i].rot.x, shape[i].rot.y, shape[i].rot.z);
			if (!Look) {
				if (shape[i].select == 0) {
					glutSolidSphere(15, 10, 10);
				}
				else if (shape[i].select == 1) {
					glutSolidCube(30);
				}
				else if (shape[i].select == 2) {
					glutSolidCone(15, 10, 3, 3);
				}
				else if (shape[i].select == 3) {
					glutSolidTeapot(30);
				}
			}
			else {
				if (shape[i].select == 0) {
					glutWireSphere(15, 10, 10);
				}
				else if (shape[i].select == 1) {
					glutWireCube(30);
				}
				else if (shape[i].select == 2) {
					glutWireCone(15, 10, 3, 3);
				}
				else if (shape[i].select == 3) {
					glutWireTeapot(30);
				}
			}
		}

		if (i == 1) {//�ι�° ����
			glTranslated(-shape[i].pos.x, 0, 0);
			glRotatef(shape[i].rot.degree, shape[i].rot.x, shape[i].rot.y, shape[i].rot.z);
			if (Look) {
				if (shape[i].select == 0) {
					glutSolidSphere(15, 10, 10);
				}
				else if (shape[i].select == 1) {
					glutSolidCube(30);
				}
				else if (shape[i].select == 2) {
					glutSolidCone(15, 10, 3, 3);
				}
				else if (shape[i].select == 3) {
					glutSolidTeapot(30);
				}
			}
			else {
				if (shape[i].select == 0) {
					glutWireSphere(15, 10, 10);
				}
				else if (shape[i].select == 1) {
					glutWireCube(30);
				}
				else if (shape[i].select == 2) {
					glutWireCone(15, 10, 3, 3);
				}
				else if (shape[i].select == 3) {
					glutWireTeapot(30);
				}
			}
		}
		//glRotatef(Time_count, 0, 1, 0);

		glPopMatrix();//���� �׸���
	}



	glPopMatrix();
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
	if (!ani) {
		move_count += 1;//Ÿ�̸� ī��Ʈ
	}
	for (int i = 0; i < 2; i++) {
		if (shape[i].any) {
			shape[i].rot.degree += 2;
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



	case 'L':
		ani = TRUE;
		for (int i = 0; i < 2; i++) {
			if (i == 0) {
				shape[i].any = TRUE;
				shape[i].rot.x = 0;
				shape[i].rot.y = 1;
				shape[i].rot.z = 0;
			}
			else {
				shape[i].any = FALSE;
			}
		}
		break;
	case 'R':
		ani = TRUE;
		for (int i = 0; i < 2; i++) {
			if (i != 0) {
				shape[i].any = TRUE;
				shape[i].rot.x = 0;
				shape[i].rot.y = 1;
				shape[i].rot.z = 0;
			}
			else {
				shape[i].any = FALSE;
			}
		}
		break;
	case 'O':
		ani = TRUE;
		ttt++;
		if (ttt % 2 == 0) {
			for (int i = 0; i < 2; i++) {
				shape[i].any = TRUE;
				shape[i].rot.x = 0;
				shape[i].rot.y = 1;
				shape[i].rot.z = 0;
			}
		}
		else {
			ani = FALSE;
		}
		
		break;
	
	case 'c'://���� �ٲٳ�
	case 'C':
		change_count++;
		if (change_count % 2 == 1) {
			Look = TRUE;
		}
		else {
			Look = FALSE;
		}
		break;
	case '1':
		next_rot++;
		for (int i = 0; i < 2; i++) {
			shape[i].select = (next_rot%4);
		}
		break;
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