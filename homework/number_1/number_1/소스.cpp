#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>
#include "cross.h"


double _ccw(vector2 a, vector2 b) {
	return a.cross(b);
}

double _ccw(vector2 p, vector2 a, vector2 b) {
	return _ccw(a - p, b - p);
}


GLvoid Reshape(int w, int h);

//해상도 설정
#define WideSize 1000
#define HighSize 800
#define Z_Size 400

//그리기 제어
GLvoid drawScene(GLvoid);
BOOL draw_Act_On;

//타이머 제어
void Timerfunction(int value);
BOOL Time_Act_On; //타이머 활성화 여부
int Time_count; //타이머 카운트

				//마우스 제어
void Mouse(int button, int state, int x, int y);
POINT ms_click; //마우스 좌표
BOOL Mouse_Act_On; //마우스 활성화 여부

void Motion(int x, int y);
BOOL Motion_Act_On; //마우스 활성화 여부

int Gradient;//True면 양수 False면 음수

				   //키보드 제어
void Keyboard(unsigned char key, int x, int y);
BOOL Keyboard_Act_On; //키보드 활성화 여부

					  //색 선정
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

typedef struct Tran {
	double x;
	double y;
};
typedef struct BoOL
{
	int b_x;
	int b_y;
	int b_z;
};

#define PT 20//도형 갯수 설정
#define PI 3.141592 //파이
typedef struct Shape
{
	Color cl;//색상
	vector2 pos;
	Translate_pos move;
	Translate_pos scale;
	Translate_pos rot;

	int size;
	int select;
	BoOL b;
	BOOL any;

};

typedef struct Main_Shape
{
	Color cl;//색상
	vector2 pos[4];
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


BOOL Activation;//Activation 트루 : 충돌 / 폴 :충돌 x 
#define Sub_t_num 10
#define Sub_t_size 50

//삼각형
Shape sub_t[Sub_t_num];//위쪽에 있는 삼각형
Main_Shape main_R[1];//왼쪽
Main_Shape main_t[Sub_t_num];
//드래그 선
Translate_pos Drag[3];

void main(int argc, char *argv[]) {
	//초기화y
	for (int i = 0; i < Sub_t_num; i++) {
		sub_t[i].size = Sub_t_size;
		sub_t[i].pos.x = -Sub_t_size;
		sub_t[i].pos.y = HighSize - Sub_t_size;
	}

	//임시 초기화
	main_R[0].move.x = WideSize/2 - Sub_t_size / 4;
	main_R[0].move.y = 0;

	main_R[0].pos[0].x = +Sub_t_size/ 2, main_R[0].pos[0].y =  +Sub_t_size / 2; // 
	main_R[0].pos[1].x = +Sub_t_size / 2, main_R[0].pos[1].y =  -Sub_t_size / 2;
	main_R[0].pos[2].x = -Sub_t_size / 2, main_R[0].pos[2].y =  -Sub_t_size / 2;
	main_R[0].pos[3].x = -Sub_t_size / 2, main_R[0].pos[3].y =  +Sub_t_size / 2;


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);//윈도우 띄우기 좌표
	glutInitWindowSize(WideSize, HighSize); //윈도우 띄우기 크기
	glutCreateWindow("ex16");
	// - 랜덤으로 시작 도형 설정하기
	//도형 그리기
	//SetupRC();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(100, Timerfunction, 1);
	srand(time(NULL));
	glutMainLoop();
}

//두 선분이 서로 접촉하는지 여부를 반환한다.
bool sementIntersects(vector2 a, vector2 b, vector2 c, vector2 d) {
	double ab = ccw(a, b, c)*ccw(a, b, d);
	double cd = ccw(c, d, a)*ccw(c, d, b);
	//두 선분이 한 직선에 위에 있거나 끝점이 겹치는 경우
	if (ab == 0 && cd == 0) {
		if (b < a) swap(a, b);
		if (d < c) swap(c, d);
		return !(b< c || d <a);
	}
	return ab <= 0 && cd <= 0;
}
int Grad_i;//새로운 i
GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();//-----------------------------------
	{
		//위에 선
		glBegin(GL_LINES);
		glVertex2i(0, HighSize - Sub_t_size + Sub_t_size / 2 );
		glVertex2i(WideSize, HighSize - Sub_t_size + Sub_t_size / 2);
		glEnd();

		//아래 선
		for (int i = 0; i < 3; i++) {
			glBegin(GL_LINES);//왼쪽 가로선
			glVertex2i(0,Sub_t_size * (i+1));
			glVertex2i(Sub_t_size * (5), Sub_t_size * (i+ 1));
			glEnd();

			glBegin(GL_LINES);//오른쪽 가로선
			glVertex2i(WideSize,Sub_t_size * (i + 1));
			glVertex2i(WideSize - Sub_t_size * (5), Sub_t_size * (i + 1));
			glEnd();

		}
		for (int i = 0; i < 10; i++) {

			if (i < 5) {
				glBegin(GL_LINES);//세로선
				glVertex2i(Sub_t_size * (i + 1), Sub_t_size * (3));
				glVertex2i(Sub_t_size * (i + 1), 0);
				glEnd();
			}
			else {
				glBegin(GL_LINES);//세로선
				glVertex2i(WideSize - Sub_t_size * (5 - i + 5) ,Sub_t_size * (3));
				glVertex2i(WideSize - Sub_t_size * (5 - i + 5), 0);
				glEnd();

			}
	
		}


		for (int i = 0; i < Sub_t_num; i++) {
			//위에 선과 돌아다니는 삼각형
			glPushMatrix();
			glTranslated(sub_t[i].pos.x, sub_t[i].pos.y + Sub_t_size / 2, 0);
		//출력 위치 정하기
			glPushMatrix();
		//회전하기
			glRotated(sub_t[i].rot.degree, 0, 0, 1);
			glBegin(GL_TRIANGLES);
			glVertex2i(0, 0);
			glVertex2i(sub_t[i].size, 0);
			glVertex2i(0, sub_t[i].size);
			glEnd();
			glPopMatrix();
			glPopMatrix();
			//----------
		}
		
		glBegin(GL_TRIANGLES);
		glVertex2i(0, 0);
		glVertex2i(10, 0);
		glVertex2i(0, 10);
		glEnd();	

		//메인 사각형

		glPushMatrix();
		glTranslated(main_R[0].move.x, main_R[0].move.y, 0);
		glBegin(GL_POLYGON); // 왼쪽
		glVertex2i(main_R[0].pos[3].x, main_R[0].pos[3].y);
		glVertex2i(main_R[0].pos[2].x, main_R[0].pos[2].y);
		glVertex2i(main_R[0].pos[1].x, main_R[0].pos[1].y);
		glVertex2i(main_R[0].pos[0].x, main_R[0].pos[0].y); // 
		glEnd();
		glPopMatrix();

		//오른쪽
		for (int i = 0; i < Grad_i; i++) {
			glPushMatrix();
			if (i % 2 == 0) {
				glTranslated(main_t[i].move.x, main_t[i].move.y, 0);
				glBegin(GL_POLYGON); // 왼쪽
				glVertex2i(main_t[i].pos[2].x, main_t[i].pos[2].y);
				glVertex2i(main_t[i].pos[0].x, main_t[i].pos[0].y);
				glVertex2i(main_t[i].pos[1].x, main_t[i].pos[1].y);

				glEnd();
			}
			else {
				glBegin(GL_POLYGON); // 오른쪽
				glVertex2i(main_t[i].pos[2].x, main_t[i].pos[2].y);
				glVertex2i(main_t[i].pos[1].x, main_t[i].pos[1].y);
				glVertex2i(main_t[i].pos[0].x, main_t[i].pos[0].y);
				glEnd();
			}
			glPopMatrix();
		}


		if (Mouse_Act_On && Motion_Act_On) {
			//드래그하는 선 그리기
			glBegin(GL_LINES);
			glVertex2i(Drag[0].x, Drag[0].y);
			glVertex2i(Drag[1].x, Drag[1].y);
			glEnd();
		}

	}	glPopMatrix();


	

	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		Drag[0].x = x;
		Drag[0].y = HighSize - y;
		Mouse_Act_On = true;
		Motion_Act_On = true;
		Gradient = 0;

	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
	
		if (Drag[0].x < Drag[1].x) {
			if (Drag[0].y < Drag[1].y) { // 기울기 +
				Gradient = 1;

				printf("양수 %d \n", Gradient);
				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[3].x;
				main_t[Grad_i].move.x = 100;

				Grad_i++;

				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[1].x;

				Grad_i++;

			}
			else {// 기울기가 마이너스 p0 y < p1 y
				Gradient = 2;
				printf("양수 %d \n", Gradient);
				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[3].x;

				Grad_i++;

				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[1].x;

				Grad_i++;
			}
		}
		else {//P0 x  > P1 x 이 작을때
			if (Drag[0].y > Drag[1].y) { // 기울기 +
				Gradient = 3;
				printf("양수 %d \n", Gradient);
				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[3].x;

				Grad_i++;

				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[1].x;

				Grad_i++;
			}
			else {// 기울기가 마이너스 P0 y > P1 y
				Gradient = 4;
				printf("양수 %d \n", Gradient);
				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[3].x;

				Grad_i++;

				main_t[Grad_i].pos[0].x = main_R[0].pos[0].x;
				main_t[Grad_i].pos[1].x = main_R[0].pos[2].x;
				main_t[Grad_i].pos[2].x = main_R[0].pos[1].x;

				Grad_i++;
			}
		}
		Drag[2].y = Drag[1].y - Drag[0].y;
		Drag[2].x = Drag[1].x - Drag[0].x;
		Drag[2].degree = Drag[2].y / Drag[2].x;//기울기

		Mouse_Act_On = false;
	}


	glutPostRedisplay();

}
void Motion(int x, int y) {
	if (Motion_Act_On == true)
	{
		Drag[1].x = x;
		Drag[1].y = HighSize - y;
	}
	
	glutPostRedisplay();

}
float LenghtPts(int x1,int y1, int x2, int y2) {
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

BOOL InCircle(int x, int y, int mx, int my , int size) {
	if (LenghtPts(x, y, mx, my) < size) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}
int temp_x, temp_y;
int move_start_count;//왼쪽에서 나타날 도형들

void Timerfunction(int value) {
	//타이머 내용
	Time_count++;
	if (Time_count % 20 == 0) {
		if (move_start_count < Sub_t_num)
			move_start_count++;
	}
	for (int i = 0; i < move_start_count; i++) {
		if (sub_t[i].pos.x > WideSize) {//화면 밖으로 나가면 초기 상태로 돌려준다.
			sub_t[i].pos.x = -Sub_t_size * 20;//20을 곱한 이유는 타이밍을 맞추기 위해서.
		}
		
		sub_t[i].pos.x += 10;
		sub_t[i].rot.degree += 10;
	}

	for (int i = 0; i < 4; i++) {
		main_R[0].pos[i].y += 10;
	}


	//m = (y2 - y1)/(x2 - x1)
	//y = mx
	//x = y/m

	if (!Mouse_Act_On) {//충돌체크 시작
	//기울기에 따라서 충돌체크가 달라진다
	//기울기 양수
		if (Gradient == 1 || Gradient == 3) {
			temp_y = Drag[2].degree *WideSize/2 ;
			if ((main_R[0].pos[3].x < temp_y) && (temp_y < main_R[0].pos[0].x)) {
				main_R[0].move.y -= 10;
				printf("1");
			}
	

		}
		else if (Gradient == 2 || Gradient == 4) {
			temp_y = (WideSize / 2) *((Drag[1].y - Drag[0].y) / (Drag[1].x - Drag[1].x));
			if ((main_R[0].pos[1].y < temp_y) && (temp_y < main_R[0].pos[0].y)) {
				main_R[0].move.y -= 10;
				printf("5");
			}
			printf("음수 %d \n", Gradient);
		}
		printf("3");
	}
	

	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}
int ttt;
int ani_count;
void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{

	case '2'://직각투영 유무

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
	//gluPerspective(45, WideSize / HighSize, 1, Z_Size); //윈도우를 초기화 하는 함수입니다!
	//glTranslatef(0, 0, -300);
	glOrtho(0, WideSize, 0, HighSize, -Z_Size / 2, Z_Size / 2); //윈도우를 초기화 하는 함수입니다!
	//glRotatef(-60, 1, 0, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}