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
typedef struct Translate_pos {
	float x;
	float y;
	float z;
	float degree;
};

#define PT 100//도형 갯수 설정
#define PI 3.141592 //파이
typedef struct Shape
{
	Color cl;//색상
	Translate_pos pos;
	Translate_pos move;
	Translate_pos scale;
	Translate_pos rot;

	int size;
	int select;
	int height;
	int slice;
	int stacks;
};

int click_count;
int save_count;
int st_help;
BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;
Shape sp1[PT];
Shape small[2];
Shape tra;

void main(int argc, char *argv[]) {
	//초기화
	st_help = 0;
	for (int i = PT / 3 * 0; i < PT / 3 * 1; i++) {
		sp1[i].pos.x = st_help * 5;
		sp1[i].pos.y = st_help * 10 - 150;
		st_help++;
	}
	st_help = 0;
	for (int i = PT / 3 * 1; i < PT / 3 * 2; i++) {
		sp1[i].pos.x = -st_help * 10 + sp1[PT / 3 * 1 - 1].pos.x;
		sp1[i].pos.y = sp1[PT / 3 * 1 - 1].pos.y;
		st_help++;
	}
	st_help = 0;
	for (int i = PT / 3 * 2; i < PT / 3 * 3; i++) {
		sp1[i].pos.x = st_help * 5 + sp1[PT / 3 * 2 - 1].pos.x;
		sp1[i].pos.y = -st_help * 10 + sp1[PT / 3 * 2 - 1].pos.y;
		st_help++;
	}
	//작은 삼각형
	for (int i = 0; i < 2; i++) {
		small[i].size = 50;
	}
	tra.rot.x = 1;
	tra.rot.y = 1;
	tra.rot.z = 1;

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
	glColor3f((float)255 / 255, (float)255 / 255, (float)255 / 255);
	/*glBegin(GL_LINES);
	glVertex2i(0, HighSize / 2);
	glVertex2i(WideSize, HighSize / 2);
	glEnd();

	glBegin(GL_LINES);
	glVertex2i(WideSize / 2, 0);
	glVertex2i(WideSize / 2, HighSize);
	glEnd();*/
	
	glMatrixMode(GL_MODELVIEW);
	//출력 설정
	glColor3f((float)100 / 255, (float)200 / 255, (float)100 / 255);
	//좌표축 그리기

	glPushMatrix(); //상태 저장 열기


	glPushMatrix();
	glTranslated(WideSize / 2, HighSize / 2, 0);
	//glPushMatrix();
	glScaled(tra.rot.x, tra.rot.y, tra.rot.z);
	glRotatef(0 + tra.rot.degree, 0, 1, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < PT; i++) {
		glVertex3i(sp1[i].pos.x, sp1[i].pos.y, sp1[i].pos.z);

	}
	glEnd();

	glPushMatrix();
	glRotatef(90 + tra.rot.degree, 0, 1, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < PT; i++) {
		glVertex3i(sp1[i].pos.x, sp1[i].pos.y, sp1[i].pos.z);

	}
	glEnd();
	glPopMatrix();


	//----------작은 삼각형


	glPushMatrix(); //상태 저장 열기
	//glTranslated(sp1[0].pos.x, sp1[0].pos.y, sp1[0].pos.z);//좌표값 위치 변경
	glScaled(1, 1, 1);
	glBegin(GL_TRIANGLES);
	glVertex3i(small[0].pos.x, small[0].pos.y - small[0].size / 2, small[0].pos.z);
	glVertex3i(small[0].pos.x + small[0].size / 2, small[0].pos.y + small[0].size / 2, small[0].pos.z);				
	glVertex3i(small[0].pos.x - small[0].size / 2, small[0].pos.y + small[0].size / 2, small[0].pos.z);
	glEnd();
	glPopMatrix();//상태저장 닫기

	glPushMatrix();
	glRotatef(90 + tra.rot.degree, 0, 1, 0);
	glBegin(GL_TRIANGLES);
	glVertex3i(small[0].pos.x, small[0].pos.y - small[0].size / 2, small[0].pos.z);
	glVertex3i(small[0].pos.x + small[0].size / 2, small[0].pos.y + small[0].size / 2, small[0].pos.z);
	glVertex3i(small[0].pos.x - small[0].size / 2, small[0].pos.y + small[0].size / 2, small[0].pos.z);
	glEnd();

	glPopMatrix();
	glPopMatrix(); //상태 저장 닫기



	glPopMatrix(); //상태 저장 닫기


	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	}
}
void Timerfunction(int value) {
	Time_count++;
	//타이머 내용 입력
	
	if (Time_count < PT) {
		if (Time_count == PT - 1) {
			Time_count = 0;
	}
		small[0].pos.x = sp1[Time_count].pos.x;
		small[0].pos.y = sp1[Time_count].pos.y;
		small[0].pos.z = sp1[Time_count].pos.z;

	}


	//꼭지점과 꼭지점사이의 거리를 100으로 나눠서
	//곱한다 가 아닌가?
	//꼭지점과 꼭지점 사이의 거리를 100으로 나눈다. -> movex라는 이동변수로 따로 만든다.
	/*
	tra.move.x += (sp1[PT / 3 * 1].pos.x - sp1[PT / 3 * 0].pos.x) / 100;
	tra.move.y += (sp1[PT / 3 * 1].pos.y - sp1[PT / 3 * 0].pos.y) / 100;


	for (int i = 0; i < PT; i++) {

			if (small[i].move.x < (sp1[PT / 3 * 1].pos.x - sp1[PT / 3 * 0].pos.x)) {
				small[i].move.x = tra.move.x;
				small[i].move.y = tra.move.y;
			}
	}
	*/

	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'y':
		if (tra.rot.degree == 360) {
			tra.rot.degree = 0;
		}
		tra.rot.degree -= 2;

		break;
	case 'Y':
		if (tra.rot.degree == 360) {
			tra.rot.degree = 0;
		}
		tra.rot.degree += 2;
		break;
	case 's':
		if (tra.rot.x > 0) {
			tra.rot.x -= 0.1;
			tra.rot.y -= 0.1;
			tra.rot.z -= 0.1;
		}
		break;
	case 'S':
		if (tra.rot.x < 300) {
			tra.rot.x += 0.1;
			tra.rot.y += 0.1;
			tra.rot.z += 0.1;

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
	glOrtho(0, WideSize, HighSize, 0, -Z_Size, Z_Size); //윈도우를 초기화 하는 함수입니다!
	//gluPerspective(WideSize, HighSize, Z_Size / 4 * 1, Z_Size / 4 * 3);
}