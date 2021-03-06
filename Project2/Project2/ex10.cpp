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

				//마우스 제어
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
POINT ms_click; //마우스 좌표
BOOL Mouse_Act_On; //마우스 활성화 여부
bool left_button;


//키보드 제어
void Keyboard(unsigned char key, int x, int y);
BOOL Keyboard_Act_On; //키보드 활성화 여부
					  //색상 선정
typedef struct Color
{
	int R;
	int B;
	int G;
};

typedef struct Shape {
	POINT pt;//좌표
	int size;//사이즈
	int trans_size;
	BOOL collide;//충돌
	Color cl;//컬러

	//삼각형 애니메이션 변수
	BOOL coll_Move;//지우개랑 처음으로 만날때
	BOOL Shape_Tri;//모양이 삼각형
	//사다리꼴 애니메이션 변수
	int tri2_pt;//사다리꼴
	int Time_count; //타이머 카운트

	BOOL LOOK = false;
	int Go = rand() % 4 + 1;
	BOOL Draw_Now = FALSE;
};



Shape back[30]; //배경 함수
Shape erase;//지우개 함수



void main(int argc, char *argv[]) {
	//초기화
	for (int i = 0; i < 30; i++) {
		//사각형 초기화 함수
		back[i].cl.R = rand() % 255;
		back[i].cl.G = rand() % 255;
		back[i].cl.B = rand() % 255;
		back[i].size = 10;
		back[i].tri2_pt = back[i].size; //사다리꼴 만들기

		back[i].collide = FALSE;

		back[i].pt.x = rand() % WideSize - back[i].size;
		back[i].pt.y = rand() % HighSize - back[i].size;

	}
	erase.size = back[0].size * 2;
	erase.cl.R = 10;
	erase.cl.G = 20;
	erase.cl.B = 10;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);//윈도우 띄우기 좌표
	glutInitWindowSize(WideSize, HighSize); //윈도우 띄우기 크기
	glutCreateWindow("ex10");
	// - 랜덤으로 시작 도형 설정하기
	//도형 그리기

	glutDisplayFunc(drawScene);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
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
	for (int i = 0; i < 30; i++) {

		//back[i].size += back[i].trans_size;

		glColor4f(
			(float)back[i].cl.R / 255,
			(float)back[i].cl.G / 255,
			(float)back[i].cl.B / 255, 1.0f
		);
		glBegin(GL_POLYGON);
		glVertex2i(back[i].pt.x - back[i].size, back[i].pt.y - back[i].size);
		glVertex2i(back[i].pt.x - back[i].tri2_pt, back[i].pt.y + back[i].size);
		glVertex2i(back[i].pt.x + back[i].tri2_pt, back[i].pt.y + back[i].size);
		glVertex2i(back[i].pt.x + back[i].size, back[i].pt.y - back[i].size);


		glEnd();

	}



	if (left_button) {
		glColor4f(
			(float)erase.cl.R / 255,
			(float)erase.cl.G / 255,
			(float)erase.cl.B / 255, 1.0f
		);
		glRectf(
			erase.pt.x - erase.size,
			erase.pt.y - erase.size,
			erase.pt.x + erase.size,
			erase.pt.y + erase.size
		);

	}

	glFlush();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		left_button = true;

	}
	else
	{
		left_button = false;
	}

}
void Motion(int x, int y) {
	if (left_button == true) {
		erase.pt.x = x;
		erase.pt.y = y;
	}

	for (int i = 0; i < 30; i++) {


		if (!back[i].Shape_Tri) {
			//삼각형일 때 지우개 사각형과 충돌체크를 하지 않는다.
			if (erase.pt.x + erase.size > back[i].pt.x - back[i].size
				&& erase.pt.y + erase.size > back[i].pt.y - back[i].size
				&& erase.pt.x - erase.size < back[i].pt.x + back[i].size
				&& erase.pt.y - erase.size < back[i].pt.y + back[i].size
				) {
				back[i].collide = TRUE;
				back[i].Draw_Now = TRUE;
				back[i].Time_count = 0;
			}
		}
	}

}
void Timerfunction(int value) {
	//타이머 내용 입력
	for (int i = 0; i < 30; i++)
	{
		if (back[i].collide) {//충돌하면
				if (back[i].tri2_pt >= 0) {
					if (back[i].tri2_pt == 0) {
						back[i].Shape_Tri = TRUE; //삼각형 만들기 실행

					}
					if (back[i].Draw_Now) {
						back[i].tri2_pt--;
					}
				}
				if (back[i].Shape_Tri) {//삼각형이 되면
					if (back[i].Time_count > 20) {
						back[i].Shape_Tri = FALSE;
						back[i].collide = FALSE;
						back[i].Draw_Now = FALSE;
					}
					if (back[i].Draw_Now) {
						back[i].size++;
						back[i].Time_count++; //시간초 증가
					}
		
				}

		

		}
		else {
			back[i].Time_count = 0;
			if (back[i].tri2_pt < 10)
				back[i].tri2_pt++;
			if (back[i].size > 10) {
				back[i].size--;
			}
		}
		//------이동하는 부분
		if (back[i].pt.x + back[i].size > WideSize || back[i].pt.y + back[i].size > HighSize) {
			if (back[i].LOOK == TRUE) {
				back[i].Go = 2;
			}
			else if (back[i].LOOK == FALSE) {
				back[i].Go = 3;
			}
		}
		else if (back[i].pt.x - back[i].size < 0 || back[i].pt.y - back[i].size < 0) {
			if (back[i].LOOK == TRUE) {
				back[i].Go = 1;
			}
			else if (back[i].LOOK == FALSE) {
				back[i].Go = 4;
			}
		}


		//커맨드에 따라 입력을 받는다.

		if (back[i].Go == 1) {
			back[i].pt.x += 10;
			back[i].pt.y += 10;
			back[i].LOOK = TRUE;

		}
		else if (back[i].Go == 2) {
			back[i].pt.x += 10;
			back[i].pt.y -= 10;
			back[i].LOOK = FALSE;
		}
		else if (back[i].Go == 3) {
			back[i].pt.x -= 10;
			back[i].pt.y -= 10;
			back[i].LOOK = FALSE;
		}
		else if (back[i].Go == 4) {
			back[i].pt.x -= 10;
			back[i].pt.y += 10;
			back[i].LOOK = TRUE;
		}
	}


	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'r'://빠르게
			 //초기화
		for (int i = 0; i < 30; i++) {

			//사각형 초기화 함수
			back[i].cl.R = rand() % 255;
			back[i].cl.G = rand() % 255;
			back[i].cl.B = rand() % 255;
			back[i].size = 10;
			back[i].collide = FALSE;
			back[i].tri2_pt = back[i].size; //사다리꼴 만들기

			back[i].pt.x = rand() % WideSize + back[i].size;
			back[i].pt.y = rand() % HighSize + back[i].size;

			back[i].LOOK = false;
			back[i].Go = rand() % 4 + 1;
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