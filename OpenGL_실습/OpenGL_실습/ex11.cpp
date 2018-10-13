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
Shape sp1[PT];//도형
int st_help; // 직선그리기 도우미

POINT move;
POINT scale;
POINT rot;
POINT dm;
BOOL ani = FALSE;
BOOL Look = FALSE;

void main(int argc, char *argv[]) {
	//초기화
	scale.x = 1;
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
	glTranslatef(400, 300, 0);
	glScaled(scale.x, scale.x, 1);
	glTranslatef(-400, -300, 0);

	//glScaled(scale.x,0,0);
	glPointSize(2);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < PT; i++) {
		glVertex3i(sp1[i].x , sp1[i].y, sp1[i].z);//도형 그리기
	}
	glEnd();
	glPopMatrix(); //상태 저장 닫기
//	glPopMatrix();

	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	}

}
void Timerfunction(int value) {

	//타이머 내용 입력
	
	if (ani) {
		move_count++;
		if (!Look) {
			move.x -= 1;
		}
		else {
			move.x += 1;
		}
	}

	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case '1'://사인곡선

		//이동 초기화
		move.x = 0;
		move.y = 0;

		for (int i = 0; i < PT; i++) {
			sp1[i].x = i * WideSize / 50 + move.x;
			sp1[i].y = 100 * sin(PI * i * 10 / 90) + HighSize / 2;
			sp1[i].z = 100;
		}
		break;

	case '2'://스프링
			 //이동 초기화
		move.x = 0;
		move.y = 0;
		for (int i = 0; i < PT; i++) {
			sp1[i].x = 100 * cos(PI * i * 10 / 90) + WideSize / 50 + i * 10 + move.x;
			sp1[i].y = 100 * sin(PI * i * 10 / 90) + HighSize / 2;
			sp1[i].z = 100;
		}
		break;
	case '3'://리본
		
		//이동 초기화
		move.x = 0;
		move.y = 0;
		//왼쪽 리본
		st_help = 0;
		for (int i = 0; i < PT/6 * 1; i++) {
			sp1[i].x = -st_help * 6+ WideSize/2 + move.x;
			sp1[i].y = -st_help * 6+ HighSize/2;
			sp1[i].z = 100;
			st_help++;
		}
		st_help = 0;
		for (int i = PT / 6 * 1; i < PT / 6 * 2; i++) {
			sp1[i].x = sp1[PT / 6 * 1 - 1].x + move.x;
			sp1[i].y = st_help * 12 + sp1[PT / 6 * 1 - 1].y;
			sp1[i].z = 100;
			st_help++;
		}
		st_help = 0;
		for (int i = PT / 6 * 2; i < PT / 6 * 3; i++) {
			sp1[i].x = -st_help * 6 + WideSize / 2 + move.x;
			sp1[i].y = st_help * 6 + HighSize / 2;
			sp1[i].z = 100;
			st_help++;
		}
				
		//오른쪽
		st_help = 0;
		for (int i = PT/6 * 3; i < PT / 6 * 4; i++) {
			sp1[i].x = st_help * 6 + WideSize / 2 + move.x;
			sp1[i].y = -st_help * 6 + HighSize / 2;
			sp1[i].z = 100;
			st_help++;
		}
		st_help = 0;
		for (int i = PT / 6 * 4; i < PT / 6 * 5; i++) {
			sp1[i].x = sp1[PT / 6 * 4 - 1].x + move.x;
			sp1[i].y = st_help * 12 + sp1[PT / 6 * 4 - 1].y;
			sp1[i].z = 100;
			st_help++;
		}
		st_help = 0;
		for (int i = PT / 6 * 5; i < PT / 6 * 6; i++) {
			sp1[i].x = st_help * 6 + WideSize / 2 + move.x;
			sp1[i].y = st_help * 6 + HighSize / 2;
			sp1[i].z = 100;
			st_help++;
		}


		break;
	case '4'://사각형
			 //이동 초기화
		move.x = 0;
		move.y = 0;
		st_help = 0;
		for (int i = PT / 4 * 0; i < PT / 4 * 1; i++) {
			sp1[i].x = WideSize / 5 * 2 +move.x;
			sp1[i].y = st_help * HighSize /100 + HighSize / 5 * 2;
			sp1[i].z = 100;
			st_help++;
		}
		st_help = 0;
		for (int i = PT / 4 * 1; i < PT / 4 * 2; i++) {
			sp1[i].x = st_help * WideSize / 100 + sp1[PT / 4 * 1 - 1].x + move.x;
			sp1[i].y = sp1[PT / 4 * 1 - 1].y;
			sp1[i].z = 100;
			st_help++;
		}
		st_help = 0;
		for (int i = PT / 4 * 2; i < PT / 4 * 3; i++) {
			sp1[i].x = sp1[PT / 4 * 2 - 1].x + move.x;
			sp1[i].y = sp1[PT / 4 * 2 - 1].y - st_help * HighSize / 100;
			sp1[i].z = 100;
			st_help++;
		}
		st_help = 0;
		for (int i = PT / 4 * 3; i < PT / 4 * 4; i++) {
			sp1[i].x = sp1[PT / 4 * 3 - 1].x - st_help * WideSize / 100 + move.x;
			sp1[i].y = sp1[PT / 4 * 3 - 1].y ;
			sp1[i].z = 100;
			st_help++;
		}
		break;
		//명령어 모음
	case 'x'://x 축 따라 -
		move.x--;
		break;
	case 'X':
		move.x++;
		break;
	case 'y'://x 축 따라 -
		move.y--;
		break;
	case 'Y':
		move.y++;
		break;
	case 's':// -
		scale.x-= 1;
		break;
	case 'S':
		scale.x+= 1;
		break;
	case 'r'://-
		move.x -= 10;
		break;
	case 'R':
		move.x += 10;
		break;
	case 'a'://x 축 따라 -
		move_count = 0; 
		ani = TRUE;
		Look = FALSE;
		break;
	case 'A':
		move_count = 0;
		ani = TRUE;
		Look = TRUE;
		break;
	case 't':
		ani = FALSE;
		break;
	case 'q' || 'Q':
		PostQuitMessage(0);
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