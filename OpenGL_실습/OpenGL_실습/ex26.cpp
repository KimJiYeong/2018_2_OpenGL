#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>
#include "other_draw.h"
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
BOOL Mouse_Act_On; //마우스 활성화 여부

				   //키보드 제어
void Keyboard(unsigned char key, int x, int y);
BOOL Keyboard_Act_On; //키보드 활성화 여부

					  //색 선정

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
//볼
Shape ball[10];
//스프링

//마우스
Shape mouse;
//박스
Shape box;

int index_box_size;
#define BALL_NUM 5

void SetupRC()
{
	index_box_size = 50;
	box.size = 10;
	box.rot.degree = 0;
	mouse.rot.degree = 0;
								 //초기화
	for (int i = 0; i < BALL_NUM; i++) {
		ball[i].size = 5;

		ball[i].move.x = 0;
		ball[i].move.y = 0;
		ball[i].move.z = 0;
		
		ball[i].rot.degree = 180;
	
		if (rand() % 2 == 0) {
			ball[i].rot.x = 1;
		}
		else {
			ball[i].rot.x = -1;
		}

		if (rand() % 2 == 0) {
			ball[i].rot.y = 1;
		}
		else {
			ball[i].rot.y = -1;
		}

		if (rand() % 2 == 0) {
			ball[i].rot.z = 1;
		}
		else {
			ball[i].rot.z = -1;
		}

	}
	//-------------
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
			glRotated(mouse.rot.degree, 0, 0, 1);
			//마우스 움직임

			glPushMatrix();
			{
				if (shade) {
					glShadeModel(GL_SMOOTH);
				}
				else {
					glShadeModel(GL_FLAT);
				}
				//상자 그리기
				Inde_Box(0, 0, 0, index_box_size);
			}
			glPopMatrix();
		
						  //공그리기
			glPushMatrix();
			{
				glTranslated(0, 0, 0);//축 초기화
				glColor3f(1, 1, 1);

				for (int i = 0; i < BALL_NUM; i++) {
					glPushMatrix();
					glTranslated(ball[i].move.x, ball[i].move.y, ball[i].move.z);
					glutSolidSphere(ball[i].size, 8, 8);
					glPopMatrix();
				
				}
			}
			glPopMatrix();//공그리기 끝

			glPushMatrix();//상자 그리기
			{
				glTranslated(box.move.x, box.move.y, box.move.z);
				glutSolidCube(box.size);
			}
			glPopMatrix();

			glPushMatrix();//상자 그리기
			{
				glTranslated(box.move.x, box.move.y, box.move.z - 10);
				glutSolidCube(box.size - 5);
			}
			glPopMatrix();

		}
		glPopMatrix();
	}
	glPopMatrix();
	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON)
	{
		mouse.any = TRUE;
		mouse.move.x = x;
		mouse.move.z = y;
	}
	glutPostRedisplay();

}
void Motion(int x, int y) {
	if (mouse.any) {
		if (mouse.move.x > x || mouse.move.y > y) {
			if (mouse.rot.degree > -180) {
				mouse.rot.degree -= 1;
			}
		}
		else {
			if (mouse.rot.degree < 180) {
				mouse.rot.degree += 1;
			}
		}
		printf("%d \n", mouse.rot.degree);
	}
}

void Timerfunction(int value) {

	for (int i = 0; i < BALL_NUM; i++) {
		//충돌체크
		ball[i].move.x += 5 * ball[i].rot.x;
		ball[i].move.y += 5 * ball[i].rot.y;
		ball[i].move.z += 5 * ball[i].rot.z;


		if ((-index_box_size / 2 >= ball[i].move.x))
		{
			ball[i].rot.x *= -1;
		}
		else if ((index_box_size / 2<= ball[i].move.x))
		{
			ball[i].rot.x *= -1;
		}

		if ((-index_box_size + 10 >= ball[i].move.y))
		{
			ball[i].rot.y *= -1;
		}
		else if ((index_box_size - 10 <= ball[i].move.y))
		{
			ball[i].rot.y *= -1;
		}

		if ((-index_box_size - 10 >=  ball[i].move.z))
		{
			ball[i].rot.z *= -1;
		}
		else if ((index_box_size  <= ball[i].move.z))
		{
			ball[i].rot.z *= -1;
		}
	}

	//충돌체크
	if ((-90 < abs(mouse.rot.degree)) && (abs(mouse.rot.degree) < 90)) {
		box.pos.y = 1;
	}
	if ((90 < abs(mouse.rot.degree)) && (abs(mouse.rot.degree) < 180)) {
		box.pos.y = 2;
	}
	if (abs(mouse.rot.degree) == 0) {
		box.pos.y =1;
		box.pos.x = 3;
	}
	if (abs(mouse.rot.degree) == 180) {
		box.pos.y = 2;
		box.pos.x = 3;
	}

	
	if ((-90 < (mouse.rot.degree)) && ((mouse.rot.degree) < 90)) {
		box.pos.x = 1;
	}
	if ((-90 < (mouse.rot.degree)) && ((mouse.rot.degree) < -1)) {
		box.pos.x = 2;
	}

	if (box.pos.x == 1) {
		if (box.move.x > -index_box_size) {
			box.move.x -= 1;
		}
	}
	else if(box.pos.x == 2){
		if (box.move.x < index_box_size) {
			box.move.x += 1;
		}
	}
	if (box.pos.y == 1 ) {
		if (box.move.y > -index_box_size) {
			box.move.y -= 1;
		}
	}
	else if (box.pos.y == 2) {
		if (box.move.y < index_box_size) {
			box.move.y += 1;
		}
	}

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

	case 'B':
	case 'b':
		index_box_size = 50;
		//초기화
		for (int i = 0; i < BALL_NUM; i++) {
			ball[i].size = 5;

			ball[i].move.x = 0;
			ball[i].move.y = 0;
			ball[i].move.z = 0;

			ball[i].rot.degree = 1;

			if (rand() % 2 == 0) {
				ball[i].rot.x = 1;
			}
			else {
				ball[i].rot.x = -1;
			}

			if (rand() % 2 == 0) {
				ball[i].rot.y = 1;
			}
			else {
				ball[i].rot.y = -1;
			}

			if (rand() % 2 == 0) {
				ball[i].rot.z = 1;
			}
			else {
				ball[i].rot.z = -1;
			}

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