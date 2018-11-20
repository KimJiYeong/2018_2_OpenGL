#include <random>
#include <time.h>
#include <math.h>
#include "base_struct.h"
#include "camera.h"
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
BOOL Mouse_Act_On; //마우스 활성화 여부

				   //키보드 제어
void Keyboard(unsigned char key, int x, int y);
BOOL Keyboard_Act_On; //키보드 활성화 여부

#define PT 20//도형 갯수 설정
#define PI 3.141592 //파이

int change_count;
int next_rot;
int st_help;
BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;

//카메라-----------------


Translate_pos EYE;
Translate_pos AT;
Translate_pos UP;

static int __x;
static int __y;
static int __z;

//카메라
Cam camera;

//은면제거
BOOL depth;
int depth_count;
//컬링
BOOL culling;
int culling_count;
//쉐이딩
BOOL shade;
int shade_count;

Custom_Light lamp1;
Custom_Light lamp2;

GLfloat ambientLight[] = { 1.f, 1.f, 1.f, 1.0f };
GLfloat DiffuseLight[] = { 1.0, 0,0, 1 };
GLfloat SpecularLight[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightPos[] = { 100 ,5, 20, 1 };

GLfloat DiffuseLight2[] = { 0.0, 0.0 ,1.0, 1 };
GLfloat SpecularLight2[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightPos2[] = { -100 ,5, 20, 1 };

// 음 뭐지 lamp1.turn = true;


void SetupRC()
{
	for (int i = 0; i < 4; i++) {
		lamp1.ambient[i] = ambientLight[i];
		lamp1.diffuse[i] = DiffuseLight[i];
		lamp1.specular[i] = SpecularLight[i];
		lamp1.pos[i] = lightPos[i];

		lamp2.ambient[i] = ambientLight[i];
		lamp2.diffuse[i] = DiffuseLight2[i];
		lamp2.specular[i] = SpecularLight2[i];
		lamp2.pos[i] = lightPos2[i];


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
	glutTimerFunc(100, Timerfunction, 1);
	srand(time(NULL));
	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{

	glFrontFace(GL_CW);

	//은면 여부
	if (depth) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}

	//컬링 여부
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


		glPushMatrix();//-------------그리기 입력--------------------------
		{
			glPushMatrix();
			{
				if (shade) {
					glShadeModel(GL_SMOOTH);
				}
				else {
					glShadeModel(GL_FLAT);
				}
			}
			glPopMatrix();

			glEnable(GL_LIGHTING);
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
			glLightfv(GL_LIGHT0, GL_AMBIENT, lamp1.ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, lamp1.diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, lamp1.specular);
			glLightfv(GL_LIGHT0, GL_POSITION, lamp1.pos);

			glLightfv(GL_LIGHT1, GL_AMBIENT, lamp2.ambient);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, lamp2.diffuse);
			glLightfv(GL_LIGHT1, GL_SPECULAR, lamp2.specular);
			glLightfv(GL_LIGHT1, GL_POSITION, lamp2.pos);


			if (lamp1.turn) {
				glEnable(GL_LIGHT0);
			}
			if (lamp2.turn) {
				glEnable(GL_LIGHT1);
			}
			glutSolidSphere(50, 8, 8);//원을 만든다.

			if (lamp1.turn) {
				glDisable(GL_LIGHT0);
			}
			if (lamp2.turn) {
				glDisable(GL_LIGHT1);
			}


			glDisable(GL_LIGHTING);
			glPushMatrix();
			{
				//
				glColor3fv(DiffuseLight);
				glTranslated(lamp1.pos[0], lamp1.pos[1], lamp1.pos[2]);
				glutSolidCone(5, 10, 8, 8);
			}
			glPopMatrix();

			glPushMatrix();
			{
				//
				glColor3fv(DiffuseLight2);
				glTranslated(lamp2.pos[0], lamp2.pos[1], lamp2.pos[2]);
				glutSolidCone(5, 10, 8, 8);
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
void Motion(int x, int y, BOOL state) {

}

void Timerfunction(int value) {


	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}
int ttt;
int ani_count;
float count = 0.1;
void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
		//---------카메라
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

		// z는 그대로 camera.z 

		//-----------카메라 끝 --------

	case '2'://직각투영 유무
		ani_count++;
		if (ani_count % 2 == 1) {
			ani = TRUE;//회전하는거 트루로
			Reshape(WideSize, HighSize);
		}
		else {
			ani = FALSE;//회전하는거 트루로
			Reshape(WideSize, HighSize);
		}
		break;

		//------------투영 끝

	case '3'://은면제거
		depth_count++;
		if (depth_count % 2 == 1) {
			depth = true;
			printf("은면 On \n");
		}
		else {
			printf("은면 Off \n");
			depth = false;
		}

		break;
	case '4'://컬링
		culling_count++;
		if (culling_count % 2 == 1) {
			culling = true;
			printf("컬링 On \n");
		}
		else {
			printf("컬링 Off \n");
			culling = false;
		}

		break;
	case '5'://쉐이딩
		shade_count++;
		if (shade_count % 2 == 1) {
			shade = true;
			printf("쉐이딩 On \n");
		}
		else {
			printf("쉐이딩 Off \n");
			shade = false;
		}

		break;

	case 'j':
	case 'J':
		if (!lamp1.turn) {
			lamp1.turn = true;
		}
		else {
			lamp1.turn = false;
		}
		break;

	case 'k':
	case 'K':
		if (!lamp2.turn) {
			lamp2.turn = true;
		}
		else {
			lamp2.turn = false;
		}
		break;

	case 'l':
	case 'L':
		count += 0.1;
		lamp1.pos[0] = 100 * cos(count * PI);
		lamp1.pos[2] = 100 * sin(count * PI);
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

	if (!ani) {
		//glRotatef(30, 0, 1, 0);
		gluPerspective(60.0f, w / h, 1.0, 1000.0);

		glTranslated(0.0, 0.0, -300.0);     // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
	}
	else {
		glOrtho(0, WideSize, HighSize, 0, -Z_Size / 2, Z_Size / 2); //윈도우를 초기화 하는 함수입니다!
		glTranslated(WideSize / 2, HighSize / 2, 0);
		//glRotatef(-60, 1, 0, 0);

	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}