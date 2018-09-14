#include <GL/freeglut.h> 
#include <random>
#include <time.h>
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

#define WideSize 800
#define HighSize 600
BOOL Mouse_click;
POINT click; //마우스 좌표
typedef struct Shape {
	POINT pos;
	int color_R;
	int color_G;
	int color_B;
	int size;
	BOOL look;
	BOOL Sha;//모양
	int Go;//방향
};
Shape shape[10];//사각형 정의
int shape_count = 0;//10개 넘어가는거 체크

void Mouse(int button, int state, int x, int y);
void Timerfunction(int value);

void main(int argc, char *argv[]) {
	//초기화
	for (int i = 0; i < 10; i++) {
		shape[i].color_R = 255;
		shape[i].color_G = 255;
		shape[i].color_B = 255;
		shape[i].size= 0;
		shape[i].pos.x = 0;
		shape[i].pos.y = 0;

	}
	int MainMenu;
	int SubMenu1 = 0;
	int SubMenu2 = 0;
	glutInit(&argc, argv);



	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);//윈도우 띄우기 좌표
	glutInitWindowSize(WideSize, HighSize); //윈도우 띄우기 크기
	glutCreateWindow("ex4");
	// - 랜덤으로 시작 도형 설정하기
	//도형 그리기

	glutDisplayFunc(drawScene);
	glutMouseFunc(Mouse);
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
	for (int i = 0; i < 10; i++) {
		glColor4f(
			(float)shape[i].color_R / 255,
			(float)shape[i].color_G / 255,
			(float)shape[i].color_B / 255, 1.0f
		);

		glRectf(
			shape[i].pos.x - shape[i].size,
			shape[i].pos.y - shape[i].size,
			shape[i].pos.x + shape[i].size,
			shape[i].pos.y + shape[i].size
		);

	}

	
	glFlush();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		shape[shape_count].pos.x = x; //마우스 좌표를 넘긴다.
		shape[shape_count].pos.y = y;
			shape[shape_count].color_R = 100;
			shape[shape_count].color_G = 100;
			shape[shape_count].color_B = 100;
			shape[shape_count].size = 10;
			if (shape_count == 10) {
				shape_count = 0;
			}
			else {
				shape_count++;
			}
		
	}

}
void Timerfunction(int value) {
	for (int i = 0; i < 10; i++) {
		shape[i].color_R += 10;

		if (shape[i].look) {
			if (shape[i].size == 20) {
				shape[i].look = FALSE;
				shape[i].color_R = 0;
			}
			shape[i].color_B -= 10;
			shape[i].color_G += 10;
			shape[i].size += 2;
		}
		else {
			if (shape[i].size == 10) {
				shape[i].look = TRUE;
				shape[i].color_R = 0;

			}
			shape[i].color_G -= 10;
			shape[i].color_B += 10;
			shape[i].size -= 2;
		}
	
	//사각형 움직이기
		if (shape[i].pos.x + shape[i].size > WideSize || shape[i].pos.y + shape[i].size > ) {
			shape[i].Go = 1;
		}
		else {
			shape[i].pos.x += 10;
			shape[i].pos.y += 10;
		}
	
	}


//	shape[shape_count].color_G = +5;
//	shape[shape_count].color_B = +5;
	glutPostRedisplay(); //타이머에 넣는다.

	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력
	
}

GLvoid Reshape(int w, int h)
{
	glViewport(-1, -1, w, h);
	glOrtho(0, WideSize, HighSize,0, 0, 1.0); //윈도우를 초기화 하는 함수입니다!

}