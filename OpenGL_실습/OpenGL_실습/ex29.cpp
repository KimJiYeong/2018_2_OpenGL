#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>
//#include "other_draw.h"
#include "base_struct.h"
GLvoid Reshape(int w, int h);

//해상도 설정
#define WideSize 800
#define HighSize 600
#define Z_Size 400

//그리기 제어
GLvoid drawScene(GLvoid);
BOOL draw_Act_On;

//타이머 제어
void Timerfunction(int value);
BOOL Time_Act_On; //타이머 활성화 여부
int move_count; //타이머 카운트

				//마우스 제어
void Mouse(int button, int state, int x, int y);
POINT ms_click; //마우스 좌표
void Motion(int x, int y);
BOOL Mouse_Act_On; //마우스 활성화 여부

				   //키보드 제어
void Keyboard(unsigned char key, int x, int y);
BOOL Keyboard_Act_On; //키보드 활성화 여부
void SpecialKeys(int key, int x, int y);

#define PT 20//도형 갯수 설정
#define PI 3.141592 //파이

int change_count;
int next_rot;
int st_help;
BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;

Shape tra;
int rot_count;
int rot_command;

Shape sub[2];

//카메라-----------------

Shape camera;
Translate_pos EYE;
Translate_pos AT;
Translate_pos UP;

static int __x;
static int __y;
static int __z;


#define Size 36

Shape tree;//나무
		   //Shape tree;
Shape chin_man;//철봉사람
Shape run_man;//철봉사람
Shape run;//철봉사람
Shape plan;
Shape press_man;
//Shape tree;
Shape ball;
Shape clain;
const void camera_custom
(double pos_x, double pos_y, double pos_z, //위치
	double degree, const double rot_x, const double rot_y, const double rot_z, //회전
	const double move_x, const double move_y, const double move_z //움직임
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

GLfloat trans_y[10];
//-------------

//---------------
struct Curve_shape
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};
GLfloat cr_shape[8][3][3]; //앞에있는 8하고 3하고만 있다고 생각하자 마지막은 x y z ()
GLfloat ctrlpoints[8][3][3] =
{
	{
		{ -40.0, 0.0, 40.0 }, // 1줄 -> 0 0 0 
{ -20.0, -100.0, 40.0 },//그 줄의 제어점 -> 0 1 0
{ 40.0, 0.0, 40.0 }  // 2줄 -> 0 2 0 

	}, //첫번째 줄
{
	{ -40.0, 0.0, 0.0 } // 1 0 0 
	,{ -50.0, -100.0, 0.0 } // 1 1 0 
	,{ 40.0, 0.0, 0.0 }  // 1 2 0 
},
{
	{ -40.0, 0.0, -40.0 }// 2 0 0 
	,{ -20.0, 40.0, -40.0 } // 2 1 0 
	,{ 40.0, 0.0, -40.0 } // 2 2 0
},
{
	{ -40.0, 0.0, -40.0 }// 3 0 0~2(x y z) 
	,{ -20.0, 40.0, -40.0 }// 3 1 0 
	,{ 40.0, 0.0, -40.0 } // 3 2 0 
},
{
	{ -40.0, 0.0, -40.0 }//4 0 0 
	,{ -20.0, 40.0, -40.0 }// 4 1 0 
	,{ 40.0, 0.0, -40.0 }//4 2 0
},
{
	{ -80.0, 0.0, -40.0 }//5 0 0 
	,{ -80.0, 40.0, -40.0 }//5 1 0 
	,{ 60.0, 0.0, -50.0 }//5 2 0 
},

};
int ck_count;
int click_play;
int pt_count;

int pos[4];
void SetupRC()
{
	for (int i = 0; i < 4; i++) {
		pos[i] = -1;
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				ctrlpoints[i][j][k] = 0.1;
			}
		}
	}
}
void main(int argc, char *argv[]) {


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);//윈도우 띄우기 좌표
	glutInitWindowSize(WideSize, HighSize); //윈도우 띄우기 크기
	glutCreateWindow("ex16");
	// - 랜덤으로 시작 도형 설정하기
	//도형 그리기
	SetupRC();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeys);
	glutTimerFunc(100, Timerfunction, 1);
	srand(time(NULL));
	glutMainLoop();
}

#define BOX_SIZE 200
//2파이 r

GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();//-----------------------------------
	{

		//glRotated(-45, 0, 0, 1);
		glTranslated(camera.move.x, camera.move.y, camera.move.z);
		camera_custom(0, 0, 0, camera.rot.degree, camera.rot.x, camera.rot.y, camera.rot.z, camera.move.x, camera.move.y, camera.move.z);
			gluLookAt(
				EYE.x, EYE.y, EYE.z,  //위5 eye
				0, 0, 0, //방향 center
				0, 1, 0 //위쪽방향(건들 ㄴㄴ) up
			);
		
		//들어갈 내용
		glPushMatrix();//---------------------------------------
		{
			for (int i = 0; i < ck_count; i++) {
			
					glMap2f(GL_MAP2_VERTEX_3,
						0, 1, 3, 3,
						0, 1, 9, 2,
						&ctrlpoints[i* 1][0][0]);
					glEnable(GL_MAP2_VERTEX_3);

					glColor3f(0, 0, 1);
					glMapGrid2f(10, 0, 1, 10, 0, 1);
					glEvalMesh2(GL_LINE, 0, 10, 0, 10);

			}
			
			glPointSize(2.0);
			glColor3f(1, 0, 1);
			glBegin(GL_POINTS);
			for (int i = 0; i < pt_count; i++) {
				for (int j = 0; j < 3; j++) {
						glVertex3fv(ctrlpoints[i][j]);
				}
			}
			glEnd();

		}
		glPopMatrix();
	}
	glPopMatrix();
	glutSwapBuffers();
}
int a;
int a_count;
GLfloat tmp;
//첫번째가 행이고 두번째가 열
void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (ani) {
			if (a_count == 0) {
				ctrlpoints[a][0][0] = x - 300;
				ctrlpoints[a][0][1] = -(300 - 1 - y);
				ctrlpoints[a][0][2] = 0.1f;
				
				ctrlpoints[a][0][2] = ctrlpoints[a][0][1];
				ctrlpoints[a][0][1] = 0.f;
				pt_count++;
				a_count++;
						printf("x %f y %f z %f \n", ctrlpoints[a][0][0], ctrlpoints[a][0][1], ctrlpoints[a][0][2]);
				
	}
			else if (a_count == 1) {
				ctrlpoints[a][2][0] =  x - 300;
				ctrlpoints[a][2][1] = -(300 - 1 - y);
				ctrlpoints[a][2][2] = 0.1f;

				ctrlpoints[a][2][2] = ctrlpoints[a][2][1];
				ctrlpoints[a][2][1] = 0.f;

				//제어점 자동 보정
				ctrlpoints[a][1][0] = ctrlpoints[a][0][0] + (ctrlpoints[a][2][0] - ctrlpoints[a][0][0]) / 2;
				ctrlpoints[a][1][1] = ctrlpoints[a][0][1] + (ctrlpoints[a][2][1] - ctrlpoints[a][0][1]) / 2;
				ctrlpoints[a][1][2] = ctrlpoints[a][0][2] + (ctrlpoints[a][2][2] - ctrlpoints[a][0][2]) / 2;
				a_count++;
				printf("제어점 : x %f y %f z %f \n", ctrlpoints[a][1][0], ctrlpoints[a][1][1], ctrlpoints[a][1][2]);
				printf("2줄 : x %f y %f z %f \n", ctrlpoints[a][2][0], ctrlpoints[a][2][1], ctrlpoints[a][2][2]);
				pt_count++;
				pt_count++;
			}
			else if (a_count == 2) {
				if (a < 8) {
					a+= 1;
					ck_count++;
					a_count = 0;
				}
			}
		
		}
	
		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		{
		
		}
		if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		{
		
			Mouse_Act_On = FALSE;

		}
	}
	glutPostRedisplay();

}
void Motion(int x, int y) {
	if (Mouse_Act_On) {


	}
}

int Time_count;

void Timerfunction(int value) {

	if (Time_Act_On) {
		for (int i = 0; i < 4; i++) {
				if (((int)-trans_y[i] >(int)ctrlpoints[i][1][1]) || ((int)trans_y[i] < (int)ctrlpoints[i][1][1])) {
					pos[i] *= -1;
				}
				ctrlpoints[i][1][1] += 5 * pos[i] ;
		}
	}

	Time_count++;
	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}
int ttt;
int ani_count;

void SpecialKeys(int key, int x, int y) {
	if (key == GLUT_KEY_DOWN) {

	}

	if (key == GLUT_KEY_UP) {

	}
	if (key == GLUT_KEY_LEFT) {

	}

	if (key == GLUT_KEY_RIGHT) {

	}
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
		//---------카메라
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

		// z는 그대로 camera.z 

		//-----------카메라 끝 --------

	case '0'://직각투영 유무
		ani_count++;
		if (ani_count % 2 == 1) {
			ani = TRUE;//회전하는거 트루로
			Reshape(WideSize, HighSize);
			printf("직각투영 on \n");
		}
		else {
			ani = FALSE;//회전하는거 트루로
			EYE.x = 10;
			EYE.y = 10;
			EYE.z = 10;
			printf("직각투영 off \n");
			Reshape(WideSize, HighSize);
		}
		break;

	case '1':
		ctrlpoints[0][1][1]+= 1;
		trans_y[0] = ctrlpoints[0][1][1];
		break;
	case '2':
		ctrlpoints[1][1][1] += 1;
		trans_y[1] = ctrlpoints[1][1][1];
		break;
	case '3':
		ctrlpoints[2][1][1] += 1;
		trans_y[2] = ctrlpoints[2][1][1];
		break;
	case '4':
		ctrlpoints[3][1][1] += 1;
		trans_y[3] = ctrlpoints[3][1][1];
		break;

	case 'p'://플레이
		Time_Act_On = true;
		break;
	case 'P'://플레이
		Time_Act_On = false;
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

		glTranslated(0.0, 0.0, -500.0);     // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
	}
	else {
		glOrtho(-400 , 400 , -300 , 300 , -400 ,400); //윈도우를 초기화 하는 함수입니다!
		glTranslated(0.0, 0.0, 0.0);     // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}