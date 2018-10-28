#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>
GLvoid Reshape(int w, int h);

//해상도 설정
#define WideSize 800
#define HighSize 600
#define Z_Size 400
#define PI 3.141592
#define Point_num 100
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

					  //색 선정
typedef struct Color
{
	int R;
	int B;
	int G;
};

//변수목록
struct Shape
{
	int x =0;//x좌표
	int y=0;//y좌표
	int z=0;//z좌표

	int choose_shape;//1 - 사인 2- 스프링 3 - 리본 4 - 사각형

};
Shape shape[Point_num];
Color cl;
void main(int argc, char *argv[]) {
	//초기화
	for (int i = 0; i < Point_num; i++) {
			ball[i].x = 0;
			ball[i].y = 0;
			ball[i].z = 0;

			ball[i].choose_shape = 1;

	}
	cl.R = 100;
	cl.G = 100;
	cl.B = 100;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);//윈도우 띄우기 좌표
	glutInitWindowSize(WideSize, HighSize); //윈도우 띄우기 크기
	glutCreateWindow("ex4");
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
	glPointSize(3);
	glLineWidth(3);
	
	glColor3f((float)100 / 255, (float)200 / 255, (float)100 / 255);
	
	glBegin(GL_LINES);
	glVertex3i(0, HighSize / 2, -(Z_Size / 2));
	glVertex3i(WideSize, HighSize / 2, (Z_Size / 2));
	glEnd();

	glColor3f((float) cl.R/ 255, (float) cl.G/255, (float) cl.B/ 255);

	glBegin(GL_LINE_STRIP);
	
	for (int i = 0; i < Point_num; i++) {
		glVertex3i(ball[i].x , ball[i].y, ball[i].z);
	}
	glEnd();

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
	case '1'://사인곡선
		for (int i = 0; i < Point_num; i++) {
				ball[i].x = i * WideSize / 50;
				ball[i].y = 100 *  sin(PI * i * 10/90) + HighSize/2;
		/*
					cir[i].pt2[j].x = -cos(PI * j * 24 / 180) * cir[i].rad[j] + cir[i].pt[60].x * 2;
							cir[i].pt2[j].y = sin(PI * j * 24 / 180) * cir[i].rad[j];
*/
				
				ball[i].z = 100;
				//우빈이 도움: z는 cos으로 해야한다.
				ball[i].choose_shape = 1;

		}
		break;
	case '2'://스프링
		for (int i = 0; i < Point_num; i++) {
			ball[i].x = 100 * cos(PI * i * 10 / 90) + WideSize / 50 + i * 10 ;
			ball[i].y = 100 * sin(PI * i * 10 / 90) + HighSize / 2;
			ball[i].z = cos(PI * i * 10 / 90);
			//우빈이 도움: z는 cos으로 해야한다.
			ball[i].choose_shape = 1;

		}
		break;
	case '3'://리본

		break;
	case '4'://사각형

		break;
	default:
		;
		break;
	}
}

GLvoid Reshape(int w, int h)
{
	glViewport(-1, -1, w, h);
	glOrtho(0, WideSize, 0, HighSize, -(Z_Size/2), (Z_Size/2)); //윈도우를 초기화 하는 함수입니다!

}