#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>
#include "cross.h"



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
	vector2 pos2[4];
	vector2 save_pos;
	Translate_pos move;
	Translate_pos scale;
	Translate_pos rot;
	float t;
	int size;
	int select;
	BoOL b;
	BOOL any;

};

typedef struct Block
{
	int init;
	vector2 pos;
	BOOL full_L;
	BOOL full_R;
};

BOOL Activation;//Activation 트루 : 충돌 / 폴 :충돌 x 
#define Sub_t_num 10
#define Main_t_num 20
#define Sub_t_size 50

				//삼각형
Shape sub_t[Sub_t_num];//위쪽에 있는 삼각형
//드래그 선

vector2 Drag[3];
Shape Rect;
static int temp_x, temp_y;
int move_start_count;//왼쪽에서 나타날 도형들

Shape throw_t[Main_t_num];
Block blcok_L[4][8];
Block blcok_R[4][8];

void main(int argc, char *argv[]) {
	
	//초기화
	//격자칸 초기화
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			blcok_L[i][j].pos.x = Sub_t_size * (j + 1) - 20;
			blcok_L[i][j].pos.y = Sub_t_size * (i + 1) + 25;
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			blcok_R[i][j].pos.x = WideSize - Sub_t_size * (j + 1) + 20;
			blcok_R[i][j].pos.y = Sub_t_size * (i + 1) + 25;
		}
	}

	Rect.pos2[0].x = 0, Rect.pos2[0].y = Sub_t_size / 2;
	Rect.pos2[1].x = -Sub_t_size / 2, Rect.pos2[1].y = 0;
	Rect.pos2[2].x = Sub_t_size / 2, Rect.pos2[2].y = 0;
	Rect.pos2[3].x = 0, Rect.pos2[3].y = -Sub_t_size / 2;

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
int Grand_j; // 새로운 j
GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//테두리 그리기
	glColor3f((float)255 / 255, (float)255 / 255, (float)255 / 255);
	
	glBegin(GL_LINES);
	glVertex2i(0, HighSize - Sub_t_size - 10);
	glVertex2i(WideSize, HighSize - Sub_t_size - 10);
	glEnd();

	//테두리 그리기
	for (int i = 0; i < 4; i++) {
		glBegin(GL_LINES);
		glVertex2i(0, Sub_t_size * (i + 1));
		glVertex2i(Sub_t_size * (8), Sub_t_size * (i + 1));
		glEnd();
	}
	//테두리 그리기
	for (int i = 0; i < 4; i++) {
		glBegin(GL_LINES);
		glVertex2i(WideSize , Sub_t_size * (i + 1));
		glVertex2i(WideSize - Sub_t_size * (8), Sub_t_size * (i + 1));
		glEnd();
	}


	//아래 테두리
	for (int i = 0; i < 8; i++) {
		glBegin(GL_LINES);
		glVertex2i(Sub_t_size * (i + 1), Sub_t_size * (4));
		glVertex2i(Sub_t_size * (i + 1), 0);
		glEnd();
	}
	for (int i = 0; i < 8; i++) {
		glBegin(GL_LINES);
		glVertex2i(WideSize - Sub_t_size * (i + 1), Sub_t_size * (4));
		glVertex2i(WideSize - Sub_t_size * (i + 1), 0);
		glEnd();
	}
	//8 * 4
	glColor3f((float)80 / 255, (float)1 / 255, (float)20 / 255);

	glBegin(GL_LINES);
	glVertex2i(WideSize / 2, 0);
	glVertex2i(WideSize / 2, HighSize);
	glEnd();

	glPushMatrix();//-----------------------------------
	{
		//위에 굴러다니는 삼각형
		glColor3f((float)200 / 255, (float)200 / 255, (float)20 / 255);
		for (int i = 0; i <move_start_count; i++) {
				glPushMatrix();
				glTranslated(sub_t[i].move.x, HighSize - Sub_t_size - 10, 0);
				glPushMatrix();
				glRotatef(sub_t[i].rot.degree, 0, 0, 1);
				glBegin(GL_POLYGON);
				glVertex2i(sub_t[i].pos.x, sub_t[i].pos.y + Sub_t_size/ 2);
				glVertex2i(sub_t[i].pos.x - Sub_t_size / 2, sub_t[i].pos.y - Sub_t_size/ 2);
				glVertex2i(sub_t[i].pos.x + Sub_t_size / 2, sub_t[i].pos.y - Sub_t_size / 2);
				glEnd();
				glPopMatrix();
				glPopMatrix();
			}


		//사각형
		if (Gradient == 0) {

			glColor3f((float)255 / 255, (float)255 / 255, (float)255 / 255);
			glPushMatrix();
			glTranslated(Rect.move.x, Rect.move.y, 0);
			glBegin(GL_POLYGON);
			glVertex2i(Rect.pos2[0].x, Rect.pos2[0].y);
			glVertex2i(Rect.pos2[2].x, Rect.pos2[2].y);
			glVertex2i(Rect.pos2[3].x, Rect.pos2[3].y);
			glVertex2i(Rect.pos2[1].x, Rect.pos2[1].y);
			glEnd();
			glPopMatrix();
		}
			//슬라이스 삼각형
			glColor3f((float)200 / 255, (float)200 / 255, (float)255 / 255);
			for (int i = 0; i < Main_t_num; i++) {
				glPushMatrix();
				{
					if (throw_t[i].any) {
						glTranslated((int)throw_t[i].move.x, (int)throw_t[i].move.y, 0);
						glRotatef(throw_t[i].rot.degree, 0, 0, 1);
						glBegin(GL_TRIANGLES);
						glVertex2i(throw_t[i].pos2[0].x, throw_t[i].pos2[0].y);
						glVertex2i(throw_t[i].pos2[1].x, throw_t[i].pos2[1].y);
						glVertex2i(throw_t[i].pos2[2].x, throw_t[i].pos2[2].y);
						glEnd();
					}
				}
				glPopMatrix();

			}
		//나중에 추가기능 구현할때 Main_t_num 구조를 큐로 바꿔도 좋을듯합니다.
		
		//드래그 선
		if (Motion_Act_On) {
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
	
		Motion_Act_On = true;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		Mouse_Act_On = true;
		Motion_Act_On = false;
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
float LenghtPts(int x1, int y1, int x2, int y2) {
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

BOOL InCircle(int x, int y, int mx, int my, int size) {
	if (LenghtPts(x, y, mx, my) < size) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

void Timerfunction(int value) {
	//타이머 내용
	

	Time_count++;
	if (Time_count % 20 == 0) {
		if (move_start_count < Sub_t_num) {
			move_start_count++;
		}
	}
	for (int i = 0; i < move_start_count; i++) {
		if (sub_t[i].move.x > WideSize) {
			sub_t[i].move.x = -Sub_t_size * 10;
		}
		sub_t[i].move.x += 10;
		sub_t[i].rot.degree += 10;
		sub_t[i].pos.y = 0;
	}
	
	//사각형
	if (Rect.move.y > HighSize || Gradient != 0) {
		Rect.move.y = 0;
	}
	Rect.move.y += 10;

	Rect.move.x = WideSize / 2;

	Gradient = 0;
	if (Mouse_Act_On) {
		//충돌체크
		if ( //충돌체크 조심하길
			((WideSize / 2 - Sub_t_size< Drag[0].x) && (Drag[0].x < WideSize / 2 + Sub_t_size)) 
			|| (( Rect.pos2[1].y < Drag[0].y) && (Drag[1].x < Rect.pos2[2].x) && (Drag[0].x > Rect.pos2[1].x)) 
			)
		{
			Gradient = 1;//충돌!
			throw_t[Grand_j].any = TRUE;//보여준다.
										//충돌했으니 삼각형을 바꿔주자.
			throw_t[Grand_j].pos2[0].x = Rect.pos2[0].x, throw_t[Grand_j].pos2[0].y = Rect.pos2[0].y;
			throw_t[Grand_j].pos2[1].x = Rect.pos2[1].x, throw_t[Grand_j].pos2[1].y = Rect.pos2[1].y;
			throw_t[Grand_j].pos2[2].x = Rect.pos2[3].x, throw_t[Grand_j].pos2[2].y = Rect.pos2[3].y;

			throw_t[Grand_j].save_pos.x = Rect.move.x;
			throw_t[Grand_j].save_pos.y = Rect.move.y;
			

			printf("삼각형 생성 %d \n" , Grand_j);

			if (Grand_j < Main_t_num) {
				Grand_j++;
			}
			throw_t[Grand_j].any = TRUE;//보여준다.
										//충돌했으니 삼각형을 바꿔주자.
			throw_t[Grand_j].pos2[0].x = Rect.pos2[0].x, throw_t[Grand_j].pos2[0].y = Rect.pos2[0].y;
			throw_t[Grand_j].pos2[1].x = Rect.pos2[3].x, throw_t[Grand_j].pos2[1].y = Rect.pos2[3].y;
			throw_t[Grand_j].pos2[2].x = Rect.pos2[2].x, throw_t[Grand_j].pos2[2].y = Rect.pos2[2].y;

			throw_t[Grand_j].save_pos.x = Rect.move.x;
			throw_t[Grand_j].save_pos.y = Rect.move.y;

			temp_y = rand() % 4 + 1;
			temp_x = rand() % 7 + 1;

			printf("삼각형 생성 %d \n", Grand_j);

			if (Grand_j < Main_t_num) {
				Grand_j++;
			}
		}

	}
	//삼각형 이동
	for (int i = 0; i < Main_t_num; i++) {
		
		//번스타인 다항식
		if (throw_t[i].any) {
			
			if (i % 2 == 0) {

				
				if (throw_t[i].t <= 1) {
					throw_t[i].t += 0.01;
					throw_t[i].move.x =
						(
						((1 - throw_t[i].t)*(1 - throw_t[i].t)*(throw_t[i].save_pos.x))//원좌표
							+ (2 * (throw_t[i].t) * (1 - throw_t[i].t) * (blcok_L[temp_y - 1][temp_x - 1].pos.x))//컨트롤 포인트
							+ ((throw_t[i].t) * (throw_t[i].t) * blcok_L[temp_y - 1][temp_x - 1].pos.x)//떨어져야 하는 좌표
							);

					throw_t[i].move.y =
						(
						((1 - throw_t[i].t)*(1 - throw_t[i].t)*(throw_t[i].save_pos.y))
							+ (2 * (throw_t[i].t) * (1 - throw_t[i].t) * (throw_t[i].save_pos.y))
							+ ((throw_t[i].t) * (throw_t[i].t) * blcok_L[temp_y - 1][temp_x - 1].pos.y)
							);

					throw_t[i].rot.degree = 45 / throw_t[i].t;
				}
			}
			else {

				temp_y = rand() % 4 + 0;
				temp_x = rand() % 8 + 0;

				if (throw_t[i].t <= 1) {
					throw_t[i].t += 0.01;
					throw_t[i].move.x =
						(
						((1 - throw_t[i].t)*(1 - throw_t[i].t)*(throw_t[i].save_pos.x))//원좌표
							+ (2 * (throw_t[i].t) * (1 - throw_t[i].t) * (blcok_R[1][1].pos.x))//컨트롤 포인트
							+ ((throw_t[i].t) * (throw_t[i].t) * blcok_R[1][1].pos.x)//떨어져야 하는 좌표
							);

					throw_t[i].move.y =
						(
						((1 - throw_t[i].t)*(1 - throw_t[i].t)*(throw_t[i].save_pos.y))
							+ (2 * (throw_t[i].t) * (1 - throw_t[i].t) * (throw_t[i].save_pos.y))
							+ ((throw_t[i].t) * (throw_t[i].t) * blcok_R[1][1].pos.y)
							);

					throw_t[i].rot.degree = (45 / throw_t[i].t);
				}
			}
			
		}
		
		
	}
	


	Mouse_Act_On = false;
	printf("%d \n", Gradient);

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