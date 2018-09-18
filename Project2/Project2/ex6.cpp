#include <GL/freeglut.h> 
#include <random>
#include <time.h>
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

#define WideSize 800
#define HighSize 600
BOOL Mouse_click;
//타이머 제어 함수
BOOL Time_Stop;
int Time_count;

POINT click; //마우스 좌표
typedef struct Shape {
	POINT pos;
	int color_R;
	int color_G;
	int color_B;
	int size;
	int Speed;
	BOOL LOOK; //방향 T = 아래 F = UP
	BOOL SIZE;
	BOOL Sha;//모양
	int Go;//방향
	
	POINT save;

	//실습 6번-------
	BOOL Spec_play = FALSE; //특수 키 활성화 여부
	int Spec_Go; //특수 좌표 조작
	BOOL Spec_look; //특수 키 방향
	int Spec_Count;//특수키 활성화 보조
};

Shape shape[9];//사각형 정의
int shape_count = 0;//10개 넘어가는거 체크
int shape_count2 = 0;
BOOL COUNT = FALSE;

void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Timerfunction(int value);

void main(int argc, char *argv[]) {
	//초기화
	for (int i = 0; i < 9; i++) {
		shape[i].color_R = 255;
		shape[i].color_G = 255;
		shape[i].color_B = 255;
		shape[i].size = 0;
		shape[i].pos.x = 0;
		shape[i].pos.y = 0;
		shape[i].Speed = i * 0.2;

	}
	int MainMenu;
	int SubMenu1 = 0;
	int SubMenu2 = 0;
	glutInit(&argc, argv);



	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);//윈도우 띄우기 좌표
	glutInitWindowSize(WideSize, HighSize); //윈도우 띄우기 크기
	glutCreateWindow("ex6");
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
	//4각형 출력
	for (int i = 0; i < shape_count2; i++) {
		glColor4f(
			(float)shape[i].color_R / 255,
			(float)shape[i].color_G / 255,
			(float)shape[i].color_B / 255, 1.0f
		);
		if (shape[i].Sha == TRUE) {
			glRectf(
				shape[i].pos.x - shape[i].size,
				shape[i].pos.y - shape[i].size,
				shape[i].pos.x + shape[i].size,
				shape[i].pos.y + shape[i].size
			);

		}
		else {
			glBegin(GL_TRIANGLES);
			glVertex2f(shape[i].pos.x - shape[i].size, shape[i].pos.y - shape[i].size);
			glVertex2f(shape[i].pos.x + shape[i].size, shape[i].pos.y - shape[i].size);
			glVertex2f(shape[i].pos.x + shape[i].size, shape[i].pos.y + shape[i].size);
			glEnd();

		}

	}


	glFlush();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		shape[shape_count].Go = 1;
		shape[shape_count].LOOK = TRUE;
		//shape[shape_count].Spec_look = TRUE;
		shape[shape_count].pos.x = x; //마우스 좌표를 넘긴다.
		shape[shape_count].pos.y = y;
		shape[shape_count].color_R = 100;
		shape[shape_count].color_G = 100;
		shape[shape_count].color_B = 100;
		shape[shape_count].size = 10;
		if (shape_count == 10) {
			shape_count = 0;
			COUNT = TRUE;
		}
		else {
			shape_count++;
			if (COUNT) {
				shape_count2 = 9;
			}
			else {
				shape_count2++;
			}
		}

	}

}
void Timerfunction(int value) {



	//	shape[shape_count].color_G = +5;
	//	shape[shape_count].color_B = +5;
	
	if (Time_Stop) {
		;
	}
	else {
		for (int i = 0; i < 9; i++) {
			shape[i].color_R += 10;

			if (shape[i].SIZE) {
				if (shape[i].size == 20) {
					shape[i].SIZE = FALSE;
					shape[i].color_R = 0;
				}
				shape[i].color_B -= 10;
				shape[i].color_G += 10;
				shape[i].size += 2;
			}
			else {
				if (shape[i].size == 10) {
					shape[i].SIZE = TRUE;
					shape[i].color_R = 0;

				}
				shape[i].color_G -= 10;
				shape[i].color_B += 10;
				shape[i].size -= 2;
			}

	//한바퀴 도는거 마저 하기

			if (shape[i].Spec_play) { //사각형 돌기가 활성화 되어있다.
				//(shape[i].pos.x + shape[i].size > WideSize &&
				if ( shape[i].Spec_Go == 1) {
					if ((shape[i].pos.x + shape[i].size + 10 > WideSize)) {
						shape[i].Spec_Go = 2;
					}
					shape[i].pos.x += 10 + shape[i].Speed;
				}	
				else if(shape[i].Spec_Go == 2) {
					if ((shape[i].pos.y + shape[i].size + 10 > HighSize)) {
						shape[i].Spec_Go = 3;
					}
					shape[i].pos.y += 10 + shape[i].Speed;
				}
				else if (shape[i].Spec_Go == 3) {
					if (shape[i].pos.x - shape[i].size - 10 < 0)
						shape[i].Spec_Go = 4;
					shape[i].pos.x -= 10 + shape[i].Speed;
				}
				else if(shape[i].Spec_Go == 4) {
					if (shape[i].pos.y - shape[i].size - 10 < shape[i].save.y)
						shape[i].Spec_Go = 5;
					shape[i].pos.y -= 10 + shape[i].Speed;
				}
				else if (shape[i].Spec_Go == 5) {
					if (shape[i].pos.x + shape[i].size - 10 > shape[i].save.x)
						shape[i].Spec_play = FALSE;
					shape[i].pos.x += 10 + shape[i].Speed;
				}

			}//if 끝
			else {//특수 키 입력이 아닐때
				if (shape[i].pos.x + shape[i].size > WideSize || shape[i].pos.y + shape[i].size > HighSize) {
					if (shape[i].LOOK == TRUE) {
						shape[i].Go = 2;
					}
					else if (shape[i].LOOK == FALSE) {
						shape[i].Go = 3;
					}
				}
				else if (shape[i].pos.x - shape[i].size  < 0 || shape[i].pos.y - shape[i].size  < 0) {
					if (shape[i].LOOK == TRUE) {
						shape[i].Go = 1;
					}
					else if (shape[i].LOOK == FALSE) {
						shape[i].Go = 4;
					}
				}


				//커맨드에 따라 입력을 받는다.

				if (shape[i].Go == 1) {
					shape[i].pos.x += 10 + shape[i].Speed;
					shape[i].pos.y += 10 + shape[i].Speed;
					shape[i].LOOK = TRUE;

				}
				else if (shape[i].Go == 2) {
					shape[i].pos.x += 10 + shape[i].Speed;
					shape[i].pos.y -= 10 + shape[i].Speed;
					shape[i].LOOK = FALSE;
				}
				else if (shape[i].Go == 3) {
					shape[i].pos.x -= 10 + shape[i].Speed;
					shape[i].pos.y -= 10 + shape[i].Speed;
					shape[i].LOOK = FALSE;
				}
				else if (shape[i].Go == 4) {
					shape[i].pos.x -= 10 + shape[i].Speed;
					shape[i].pos.y += 10 + shape[i].Speed;
					shape[i].LOOK = TRUE;
				}
			}

			
		}
	}
	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'q'://빠르게
		for (int i = 0; i < 9; i++) {
			shape[i].Speed += 2;
		}
		break;
	case 'w'://느리게
		for (int i = 0; i < 9; i++) {
			shape[i].Speed -= 2;
		}
		break;
	case 's'://멈춤
		Time_count++;
		if (Time_count % 2 == 0) {
			Time_Stop = TRUE;
		}
		else if(Time_count%2 == 1){
			Time_Stop = FALSE;
		}
		break;
	case '1'://모든 사각형 액션
		shape[0].Spec_Count++;
			if (shape[0].Spec_Count % 2 == 1) {
				shape[0].save.x = shape[0].pos.x;
				shape[0].save.y = shape[0].pos.y;
				shape[0].Spec_play = TRUE;
				shape[0].Spec_Go = 1;
				shape[0].Spec_look = TRUE;
			}
			else if (shape[0].Spec_Count % 2 == 0) {
				shape[0].Spec_play = FALSE;
				shape[0].Spec_look = FALSE;
				shape[0].Spec_Go = 1;
			}

		break;
	case '2'://모든 사각형 액션
		shape[1].Spec_Count++;
		if (shape[1].Spec_Count % 2 == 1) {
			shape[1].save.x = shape[1].pos.x;
			shape[1].save.y = shape[1].pos.y;
			shape[1].Spec_play = TRUE;
			shape[1].Spec_Go = 1;
			shape[1].Spec_look = TRUE;
		}
		else if (shape[1].Spec_Count % 2 == 0) {
			shape[1].Spec_play = FALSE;
			shape[1].Spec_look = FALSE;
			shape[1].Spec_Go = 1;
		}

		break;
	case '3'://모든 사각형 액션
		shape[2].Spec_Count++;
		if (shape[2].Spec_Count % 2 == 1) {
			shape[2].save.x = shape[2].pos.x;
			shape[2].save.y = shape[2].pos.y;
			shape[2].Spec_play = TRUE;
			shape[2].Spec_Go = 1;
			shape[2].Spec_look = TRUE;
		}
		else if (shape[2].Spec_Count % 2 == 0) {
			shape[2].Spec_play = FALSE;
			shape[2].Spec_look = FALSE;
			shape[2].Spec_Go = 1;
		}

		break;
	case '4'://모든 사각형 액션
		shape[3].Spec_Count++;
		if (shape[3].Spec_Count % 2 == 1) {
			shape[3].save.x = shape[3].pos.x;
			shape[3].save.y = shape[3].pos.y;
			shape[3].Spec_play = TRUE;
			shape[3].Spec_Go = 1;
			shape[3].Spec_look = TRUE;
		}
		else if (shape[3].Spec_Count % 2 == 0) {
			shape[3].Spec_play = FALSE;
			shape[3].Spec_look = FALSE;
			shape[3].Spec_Go = 1;
		}

		break;
	case '5'://모든 사각형 액션
		shape[4].Spec_Count++;
		if (shape[4].Spec_Count % 2 == 1) {
			shape[4].save.x = shape[4].pos.x;
			shape[4].save.y = shape[4].pos.y;
			shape[4].Spec_play = TRUE;
			shape[4].Spec_Go = 1;
			shape[4].Spec_look = TRUE;
		}
		else if (shape[4].Spec_Count % 2 == 0) {
			shape[4].Spec_play = FALSE;
			shape[4].Spec_look = FALSE;
			shape[4].Spec_Go = 1;
		}

		break;
	case '6'://모든 사각형 액션
		shape[5].Spec_Count++;
		if (shape[5].Spec_Count % 2 == 1) {
			shape[5].save.x = shape[5].pos.x;
			shape[5].save.y = shape[5].pos.y;
			shape[5].Spec_play = TRUE;
			shape[5].Spec_Go = 1;
			shape[5].Spec_look = TRUE;
		}
		else if (shape[5].Spec_Count % 2 == 0) {
			shape[5].Spec_play = FALSE;
			shape[5].Spec_look = FALSE;
			shape[5].Spec_Go = 1;
		}

		break;
	case '7'://모든 사각형 액션
		shape[6].Spec_Count++;
		if (shape[6].Spec_Count % 2 == 1) {
			shape[6].save.x = shape[6].pos.x;
			shape[6].save.y = shape[6].pos.y;
			shape[6].Spec_play = TRUE;
			shape[6].Spec_Go = 1;
			shape[6].Spec_look = TRUE;
		}
		else if (shape[6].Spec_Count % 2 == 0) {
			shape[6].Spec_play = FALSE;
			shape[6].Spec_look = FALSE;
			shape[6].Spec_Go = 1;
		}

		break;
	case '8'://모든 사각형 액션
		shape[7].Spec_Count++;
		if (shape[7].Spec_Count % 2 == 1) {
			shape[7].save.x = shape[7].pos.x;
			shape[7].save.y = shape[7].pos.y;
			shape[7].Spec_play = TRUE;
			shape[7].Spec_Go = 1;
			shape[7].Spec_look = TRUE;
		}
		else if (shape[7].Spec_Count % 2 == 0) {
			shape[7].Spec_play = FALSE;
			shape[7].Spec_look = FALSE;
			shape[7].Spec_Go = 1;
		}

		break;
	case '9'://모든 사각형 액션
		shape[8].Spec_Count++;
		if (shape[8].Spec_Count % 2 == 1) {
			shape[8].save.x = shape[8].pos.x;
			shape[8].save.y = shape[8].pos.y;
			shape[8].Spec_play = TRUE;
			shape[8].Spec_Go = 1;
			shape[8].Spec_look = TRUE;
		}
		else if (shape[8].Spec_Count % 2 == 0) {
			shape[8].Spec_play = FALSE;
			shape[8].Spec_look = FALSE;
			shape[8].Spec_Go = 1;
		}

		break;
	case '0'://모든 사각형 액션
		shape[9].Spec_Count++;
		if (shape[9].Spec_Count % 2 == 1) {
			shape[9].save.x = shape[9].pos.x;
			shape[9].save.y = shape[9].pos.y;
			shape[9].Spec_play = TRUE;
			shape[9].Spec_Go = 1;
			shape[9].Spec_look = TRUE;
		}
		else if (shape[9].Spec_Count % 2 == 0) {
			shape[9].Spec_play = FALSE;
			shape[9].Spec_look = FALSE;
			shape[9].Spec_Go = 1;
		}

		break;

	case 'o'://모든 사각형 액션
		for (int i = 0; i < 9; i++) {
			shape[i].Spec_Count++;
			if (shape[i].Spec_Count % 2 == 1) {
				shape[i].save.x = shape[i].pos.x;
				shape[i].save.y = shape[i].pos.y;
				shape[i].Spec_play = TRUE;
				shape[i].Spec_Go = 1;
				shape[i].Spec_look = TRUE;
			}
			else if (shape[i].Spec_Count % 2 == 0) {
				shape[i].Spec_play = FALSE;
				shape[i].Spec_look = FALSE;
				shape[i].Spec_Go = 1;
			}
		}
		
		break;
	case 'r'://사각형
		for (int i = 0; i < 9; i++) {
			shape[i].Sha = TRUE;
		}
		break;
	case 't'://삼각형
		for (int i = 0; i < 9; i++) {
			shape[i].Sha = FALSE;
		}
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