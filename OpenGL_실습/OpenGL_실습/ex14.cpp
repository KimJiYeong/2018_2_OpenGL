#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>
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
	int G;
	int B;
};

#define PT 100//점 갯수 설정
#define PI 3.141592 //파이
typedef struct Shape
{
	Color cl;//색상
	int x;
	int y;
	int z;
};

POINT move;
POINT scale;
POINT rot;

BOOL ani = FALSE;
BOOL Look = FALSE;

void main(int argc, char *argv[]) {
	//초기화

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);//윈도우 띄우기 좌표
	glutInitWindowSize(WideSize, HighSize); //윈도우 띄우기 크기
	glutCreateWindow("ex11");
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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(2);
	glColor3f((float)100 / 255, (float)200 / 255, (float)100 / 255);
	glBegin(GL_LINES);
	glVertex2i(0, HighSize / 2);
	glVertex2i(WideSize, HighSize / 2);
	glEnd();

	glBegin(GL_LINES);
	glVertex2i(WideSize / 2, 0);
	glVertex2i(WideSize / 2, HighSize);
	glEnd();

	glMatrixMode(GL_MODELVIEW);
	//출력 설정
	//좌표축 그리기

	glPushMatrix(); //상태 저장 열기
					//변형
	glTranslated(move.x, move.y, 0);
	//glScaled(scale.x,0,0);
	glPointSize(2);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < PT; i++) {
		glVertex3i(sp[i].x, sp[i].y, sp[i].z);//도형 그리기
	}
	glEnd();

	glPopMatrix(); //상태 저장 닫기

	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//x y 받아오기
		ms_click.x = x;
		ms_click.y = y;
	}

}
void Timerfunction(int value) {

	//타이머 내용 입력
	Time_count++;

	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case '1':
		break;

	default:
		;
		break;
	}
}

GLvoid Reshape(int w, int h)
{
	glViewport(-1, -1, w, h);
	glOrtho(0, WideSize, HighSize, 0, -Z_Size / 2, Z_Size / 2); //윈도우를 초기화 하는 함수입니다!

}