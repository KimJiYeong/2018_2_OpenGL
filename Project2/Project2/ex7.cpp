#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>

GLvoid Reshape(int w, int h);

//해상도 설정
#define WideSize 800
#define HighSize 600

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

//키보드 제어
void Keyboard(unsigned char key, int x, int y);
BOOL Keyboard_Act_On; //키보드 활성화 여부

//컬러
typedef struct Color
{
	int R;
	int B;
	int G;
};

//도형 변수
#define cir_amount 15
#define PI 3.14159265358

typedef struct Circle {
	POINT pt[36]; //360도
	Color cl; //색상
	int rad;//반지름
	int Ani_rad; //애니메이션 라디안
	POINT pos;//중심 좌표
	int Max_rad; //맥스 라디언
	//-------------------------------------
	BOOL Ani;//밖으로 나가는 애니메이션 여부
};
int Ani_rand;
BOOL draw;
int circle_count =0;
int draw_count = 0;
Circle cir[cir_amount];

void main(int argc, char *argv[]) {
	//초기화
	for (int i = 0; i < cir_amount; i++) {
	
		cir[i].cl.R = 255;
		cir[i].cl.G = 255;
		cir[i].cl.B = 255;
		cir[i].rad = 0;
	}
	glutInit(&argc, argv);//없으면 화면 안보임

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);//윈도우 띄우기 좌표
	glutInitWindowSize(WideSize, HighSize); //윈도우 띄우기 크기
	glutCreateWindow("ex7");
	// - 랜덤으로 시작 도형 설정하기
	//도형 그리기

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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//그리기 출력	
	
	for (int i = 0; i < cir_amount; i++) {
		glPointSize(3);
		glColor4f(
			(float)cir[i].cl.R / 255,
			(float)cir[i].cl.G / 255,
			(float)cir[i].cl.B / 255,
			1.0f
		);
		glBegin(GL_POINTS);
		for (int j = 0; j < 36; j++) {
			glVertex2f(
				cir[i].pos.x + cir[i].pt[j].x, 
				cir[i].pos.y + cir[i].pt[j].y
			);
		}
		glEnd();
	}


	glFlush();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		
		if((rand() % 2))
			cir[circle_count].Ani = TRUE;
		else
			cir[circle_count].Ani = FALSE;

		cir[circle_count].pos.x = x;
		cir[circle_count].pos.y = y;

		cir[circle_count].cl.R = 100;
		cir[circle_count].cl.G = 100;
		cir[circle_count].cl.B = 200;
		cir[circle_count].rad = 0;

		//circle 카운트 초기화
		if (circle_count == 19) {
			circle_count = 0;
			draw = TRUE;
		}
		else {
			if (draw)
				draw_count = 20;
			else
				draw_count++;

			circle_count++;
		}

	}

}

void Timerfunction(int value) {

	//타이머 내용 입력
	for (int i = 0; i < draw_count; i++) {
		
		if (cir[i].Ani) {//애니메이션이 트루라면
			cir[i].Max_rad = WideSize - cir[i].pos.x + 100;
			if (cir[i].cl.R < 255)
				cir[i].cl.R += 2;
			else
				cir[i].cl.R = 50;
			
			if (cir[i].cl.G < 255)
				cir[i].cl.G += 2;
			else
				cir[i].cl.G = 50;
			
			if (cir[i].cl.G < 255)
				cir[i].cl.G += 2;
			else
				cir[i].cl.G = 50;

		}
		else {
			cir[i].cl.R = 100;
			cir[i].cl.G = 100;
			cir[i].cl.B = 200;

			cir[i].Max_rad = 80; //각도 조절
		}
		
		if (cir[i].rad < cir[i].Max_rad) {
			cir[i].rad += 2; //반지름 증가
		}
		else {
			if (cir[i].Ani)
				;
			else
				cir[i].rad = 0;
		}
		
		for (int j = 0; j < 36; j++) {
				cir[i].pt[j].x = cos(PI * j * 10/ 180) * cir[i].rad;
				cir[i].pt[j].y = sin(PI * j * 10 / 180) * cir[i].rad;
		}
	}

	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'q'://빠르게

		break;
	default:
		;
		break;
	}
}

GLvoid Reshape(int w, int h)
{
	glViewport(-1, -1, w, h);
	glOrtho(0, WideSize, HighSize, 0, 0, 1.0); //윈도우를 초기화 하는 함수입니다!

}