#include <GL/freeglut.h> 
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
void Motion(int x, int y);
void Passive(int x , int y);
BOOL Mouse_Act_On; //마우스 활성화 여부

				   //키보드 제어
void Keyboard(unsigned char key, int x, int y);
BOOL Keyboard_Act_On; //키보드 활성화 여부

					  //색 선정

#define PT 20//도형 갯수 설정
#define PI 3.141592 //파이

BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;

//카메라
Cam camera;
int view_trans;
#define FRONT 0
#define TOP 1

//은면제거
BOOL depth;
int depth_count;
//컬링
BOOL culling;
int culling_count;
//쉐이딩
BOOL shade;
int shade_count;

struct Look_int
{
	int x;
	int y;
	int z;
	int degree;
};

//피킹 검사
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
	glutInitWindowPosition(100, 100);//윈도우 띄우기 좌표
	glutInitWindowSize(WideSize, HighSize); //윈도우 띄우기 크기
	glutCreateWindow("ex16");
	// - 랜덤으로 시작 도형 설정하기
	//도형 그리기
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
			//마우스 움직임

			if (shade) {
				glShadeModel(GL_SMOOTH);
			}
			else {
				glShadeModel(GL_FLAT);
			}
			//상자 그리기
			

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


	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}
int ttt;
int ani_count;
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
			printf("직각투영 On \n");
		}
		else {
			ani = FALSE;//회전하는거 트루로
			Reshape(WideSize, HighSize);
			printf("직각투영 off \n");
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
	//투영 변환
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

		glTranslated(0.0, 0.0, -500.0);     // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
	}
	else {
		glOrtho(-WideSize/2, WideSize/2, -HighSize/2, HighSize/2, -400, 400); //윈도우를 초기화 하는 함수입니다!
		glTranslated(0.0, 0.0, 0.0);     // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}