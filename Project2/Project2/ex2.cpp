#include <GL/freeglut.h> 
#include <random>
#include <time.h>

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

					  //색 선정
typedef struct Color
{
	int R;
	int B;
	int G;
};
//특정 개수 최대치
int max_x;
int max_y;

//특정개수 나누기
int num_x;
int num_y;

Color color[10];
Color change_color[10][10];
void main(int argc, char *argv[]) {
	//초기화
	max_x = rand() % 10 + 1;
	max_y = rand() % 10 + 1;

	for (int i = 0; i < max_x; i++) {
		color[i].R = rand() % 200 + 50;
		color[i].G = rand() % 200 + 50;
		color[i].B = rand() % 200 + 100;
		change_color[i][0].R = color[i].R;
		change_color[i][0].G = color[i].G;
		change_color[i][0].B = color[i].B;
	}

	for (int i = 1; i < max_x; i++)
	{
		for (int j = 0; j < max_y - 1; j++)
		{
			change_color[i][j].R = change_color[i - 1][j].R - 20;
			change_color[i][j].G = change_color[i - 1][j].G - 20;
			change_color[i][j].B = change_color[i - 1][j].B - 5;

		}

	}

	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);//윈도우 띄우기 좌표
	glutInitWindowSize(WideSize, HighSize); //윈도우 띄우기 크기
	glutCreateWindow("ex2");
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
	//출력
	for (int i = 0; i < max_y; i++) {
		for (int j = 0; j < max_x; j++) {
			//4각형 그리기
			glColor4f(
				(float)change_color[i][j].R / 255,
				(float)change_color[i][j].G / 255,
				(float)change_color[i][j].B / 255, 1.0f
			);
			glRectf((WideSize / max_x) * j, (HighSize/max_y)* i, (WideSize / max_x) * (j + 1), (HighSize / max_y)* (i+1));

		}
	

	}
	glFlush();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	}

}
void Timerfunction(int value) {

	//타이머 내용 입력

	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'r'://빠르게
		max_x = rand() % 10+2;
		max_y = rand() % 10+2;

		for (int i = 0; i < max_x; i++) {

			color[i].R = rand() % 100 + 50;
			color[i].G = rand() % 150 + 100;
			color[i].B = rand() % 150 + 100;

			if (color[i].R > 200) {
				color[i].R = rand() % 100 + 50;
			}

			change_color[i][0].R = color[i].R;
			change_color[i][0].G = color[i].G;
			change_color[i][0].B = color[i].B;
		}
		
		for (int i = 0; i < max_y; i++)
		{
			for (int j = 1; j < max_x; j++)
			{
				change_color[i][j].R = change_color[i][j-1].R - 90;
				change_color[i][j].G = change_color[i][j-1].G - 40;
				change_color[i][j].B = change_color[i][j-1].B - 35;

			}

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