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
	POINT pt[60]; //360도
	POINT pt2[60];
	Color cl; //색상
	int rad[60];//반지름
	int Ani_rad; //애니메이션 라디안
	POINT pos;//중심 좌표
	int Max_rad; //맥스 라디언
				 //-------------------------------------
	BOOL Ani;//밖으로 나가는 애니메이션 여부
	int Time_count; //타이머 카운트
	
	int Ani1_count;//정방향 돌리기
	int Ani2_count;//역방향 돌리기

	//반대로 돌리기
	int dir; //방향

};
int Ani_rand;
BOOL draw;
int circle_count = 0;
int draw_count = 0;
Circle cir[cir_amount];

void main(int argc, char *argv[]) {
	//초기화
	for (int i = 0; i < cir_amount; i++) {

	//	cir[i].cl.R = 255;
	//	cir[i].cl.G = 255;
	//	cir[i].cl.B = 255;
		for (int k = 0; k < 60; k++) {
			cir[i].rad[k] = 0;
		}
		cir[i].Ani = FALSE;
		cir[i].Ani2_count = 60;

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
		for (int j = 0; j < 60; j++) {
				glVertex2f(
					cir[i].pos.x + cir[i].pt[j].x,
					cir[i].pos.y + cir[i].pt[j].y
				);
				glVertex2f(
					cir[i].pos.x + cir[i].pt2[j].x ,
					cir[i].pos.y + cir[i].pt2[j].y
				);
		}
		
		glEnd();
	}


	glFlush();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	
		cir[circle_count].pos.x = x;
		cir[circle_count].pos.y = y;

		cir[circle_count].cl.R = 100;
		cir[circle_count].cl.G = 100;
		cir[circle_count].cl.B = 200;

		cir[circle_count].dir = rand() % 2;


		for (int k = 0; k < 60; k++) {
			cir[circle_count].rad[k] = 0;
		}
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
		

		cir[i].Time_count++;

		if (cir[i].Time_count %1 == 0) {

			if (cir[i].dir ==1) {//애니메이션이 트루라면
				if (cir[i].Ani1_count <= 60)
				{
					cir[i].Ani = FALSE;
					cir[i].Ani1_count++;

				}
				else {
					cir[i].Ani = TRUE;
				}
				if (cir[i].Ani2_count > 0) {
					if (cir[i].Ani == TRUE) {
						cir[i].Ani2_count--;

						for (int j = 60; j > cir[i].Ani2_count; j--) {
							cir[i].rad[j] = j * 1.5; //반지름 증가
							cir[i].pt2[j].x = -cos(PI * j * 24 / 180) * cir[i].rad[j] + cir[i].pt[60].x * 2;
							cir[i].pt2[j].y = sin(PI * j * 24 / 180) * cir[i].rad[j];

						}
					}

				}

				for (int j = 0; j < cir[i].Ani1_count; j++) {
					cir[i].rad[j] = j * 1.5; //반지름 증가

					cir[i].pt[j].x = cos(PI * j * 24 / 180) * cir[i].rad[j];
					cir[i].pt[j].y = -sin(PI * j * 24 / 180) * cir[i].rad[j];



				}
			}
			else {

			
					if (cir[i].Ani1_count <= 60)
					{
						cir[i].Ani = FALSE;
						cir[i].Ani1_count++;

					}
					else {
						cir[i].Ani = TRUE;
					}
					if (cir[i].Ani2_count > 0) {
						if (cir[i].Ani == TRUE) {
							cir[i].Ani2_count--;

							for (int j = 60; j > cir[i].Ani2_count; j--) {
								cir[i].rad[j] = j * 1.5; //반지름 증가
								cir[i].pt2[j].x = cos(PI * j * 24 / 180) * cir[i].rad[j] + cir[i].pt[60].x * 2;
								cir[i].pt2[j].y = -sin(PI * j * 24 / 180) * cir[i].rad[j];

							}
						}

					}

					for (int j = 0; j < cir[i].Ani1_count; j++) {
						cir[i].rad[j] = j * 1.5; //반지름 증가

						cir[i].pt[j].x = -cos(PI * j * 24 / 180) * cir[i].rad[j];
						cir[i].pt[j].y = sin(PI * j * 24 / 180) * cir[i].rad[j];


					}
			}


			
			}//애니메이션
		
		
	}//타이머 카운트

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