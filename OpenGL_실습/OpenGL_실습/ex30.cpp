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
;
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
GLfloat ctrlpoints[3][3][3] =
{
	{
		{ -100.0, 0.0, -100.0 }, // 1줄 -> 0 0 0 
{  -100.0, 0.0, 0.0 },//그 줄의 제어점 -> 0 1 0
{ -100.0, 0.0, 100.0 }  // 2줄 -> 0 2 0 
	}, //첫번째 줄
{
	{ 0.0, 0.0, -100.0 } // 1 0 0 
	,{ 0.0, 100.0, 0.0 } // 1 1 0 
	,{ 0.0, 0.0, 100.0 }  // 1 2 0 
},
{
	{ 100.0, 0.0, -100.0 }// 2 0 0 
	,{ 100.0, 0.0, 0.0 } // 2 1 0 
	,{ 100.0, 0.0, 100.0 } // 2 2 0
},

};
int ck_count;
int click_play;
int pt_count;
Shape ball;
Shape line[Size];
int pos[4];
Shape vagier[Size];

float t;

void SetupRC()
{
	ball.size = 10;
	ball.move.x = 0;
	ball.move.y = 200;
	ball.move.z = 0;

	//라인 정리
	for (int i = 0; i < Size; i++) {
		line[i].pos.x = (50) * cos( PI * i / 18 );
		line[i].pos.y =	( 100);
		line[i].pos.z = (50)* sin(PI * i  / 18);
	}
	//라인 정리
	for (int i = 0; i < Size; i++) {
		line[i].move.x = (50 - i) * cos(PI * i / 18);
		line[i].move.y = (100 - 2 * i);
		line[i].move.z = (50 - i)* sin(PI * i / 18);
	}

	for (int i = 0; i < Size; i++) {
		t += 0.02;
		vagier[i].pos.x = 
				(
				((1 - t)*(1 - t)*(line[Size - 1].move.x))//원좌표
				+ (2 * (t) * (1 - t) * (ctrlpoints[0][0][0]))//컨트롤 포인트
				+ ((t) * (t) * ctrlpoints[0][0][0])//떨어져야 하는 좌표
				);
			vagier[i].pos.y =
				(
				((1 - t)*(1 - t)*(line[Size - 1].move.y))//원좌표
					+ (2 * (t) * (1 - t) * (line[Size - 1].move.y))//컨트롤 포인트
					+ ((t) * (t) * ctrlpoints[0][0][1])//떨어져야 하는 좌표
					);
	
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
BOOL fill = false;
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

				glMap2f(GL_MAP2_VERTEX_3,
					0, 1, 3, 3,
					0, 1, 9, 3,
					&ctrlpoints[0][0][0]);
				glEnable(GL_MAP2_VERTEX_3);

				glColor3f(1, 1, 1);
				glMapGrid2f(10, 0, 1, 10, 0, 1);
				if (fill) {
					glEvalMesh2(GL_LINE, 0, 10, 0, 10);
				}
				else {
					glEvalMesh2(GL_FILL, 0, 10, 0, 10);
				}

			glPointSize(2.0);
			glColor3f(1, 0, 1);
			glBegin(GL_POINTS);
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					glVertex3fv(ctrlpoints[i][j]);
				}
			}
			glEnd();

			glPushMatrix();
			{
				glTranslated(ball.move.x + ball.pos.x, ball.move.y + ball.pos.y, ball.move.z + ball.pos.z);
				glutSolidSphere(ball.size, 8, 8);
			}
			glPopMatrix();


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
		
	}
	glutPostRedisplay();

}
void Motion(int x, int y) {
	if (Mouse_Act_On) {


	}
}

int Time_count;
int look = -1;
int sub_count = 0;
int sub2_count = 0;
void Timerfunction(int value) {

	if (!ball.any) {
		ball.move.x = line[Time_count % 36].pos.x;
		ball.move.y = line[Time_count % 36].pos.y;
		ball.move.z = line[Time_count % 36].pos.z;
	}
	else {
		ball.move.x = line[ball.move.degree].move.x;
		ball.move.y = line[ball.move.degree].move.y;
		ball.move.z = line[ball.move.degree].move.z;
		if (ball.move.degree < Size - 1) {
			ball.move.degree++;
			printf("%d y %f \n", ball.move.degree, ball.move.y);

		}
	}

	if (ball.move.degree > Size - 5) {
		
		if (sub_count == 10) {
			look *= -1;
		}
		if (sub_count < 15) {
			ctrlpoints[1][1][1] += 5 * look;
			ball.move.y += 2 * look;
		}

		sub_count+=1;
		printf("time : %d", sub_count);
	}
	//공이 움직인다.
	if ((sub_count > 15)) {
		if (sub2_count < Size - 2) {
			sub2_count++;
		}
		ball.move.x = vagier[sub2_count].pos.x;	
		ball.move.y = vagier[sub2_count].pos.y;
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

	case 'u':
		ball.any = true;
		break;

	case 'r':
		ball.any = false;
		Time_count = 0;
		ball.move.degree = 0;
		look = -1;
		ctrlpoints[1][1][1] = 100;
		sub_count = 0;
		
		ball.pos.x = 0;
		ball.pos.y = 0;
		ball.pos.z = 0;

		sub2_count = 0;
		break;

	case 'f':
		fill = true;
		break;

	case 'F':
		fill = false;
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
		glOrtho(-400, 400, -300, 300, -400, 400); //윈도우를 초기화 하는 함수입니다!
		glTranslated(0.0, 0.0, 0.0);     // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}