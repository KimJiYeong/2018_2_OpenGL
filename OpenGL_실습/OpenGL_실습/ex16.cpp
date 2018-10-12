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

#define PT 20//도형 갯수 설정
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

Translate_pos camera;
Shape tra;
int rot_count;
int rot_command;

void SetupRC()
{
	//초기화
	glTranslated(WideSize / 2, HighSize / 2, Z_Size/2);
}
void main(int argc, char *argv[]) {
	//초기화
	click_count = 0;
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
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(100, Timerfunction, 1);
	
	srand(time(NULL));
	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(
		camera.x, camera.y, camera.z,  //위5 eye
		0, 0, 0, //방향 center
		0, 1, 0); //위쪽방향(건들 ㄴㄴ) up

	glLineWidth(2);
	glColor3f((float)255 / 255, (float)255 / 255, (float)255 / 255);
	glMatrixMode(GL_MODELVIEW);
	//좌표축 그리기
	
	glPushMatrix();
	glScalef(1, 0.001, 1);
	glutSolidCube(200);
		
	glPopMatrix();
	//가운데 막대 그리기
	glColor3f((float)255 / 255, (float)0 / 255, (float)0 / 255);

	for (int i = 0; i < 3; i++) {
		glPushMatrix();
		if (i == 0) {
			glColor3f((float)255 / 255, (float)0 / 255, (float)0 / 255);
			glRotated(90, 1, 0, 0);
		}
		else if (i == 1) {
			glColor3f((float)0 / 255, (float)255 / 255, (float)0 / 255);
			glRotated(90, 0, 1, 0);
		}
		else if (i == 2) {
			glColor3f((float)0 / 255, (float)0 / 255, (float)255 / 255);
			glRotated(90, 0, 0, 1);
		}
		glScalef(1, 0.1, 0.1);
		glutSolidCube(40);
		glPopMatrix();
	}
	
	glPopMatrix();

	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	}
	glutPostRedisplay();

}
void Timerfunction(int value) {
	
	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'x':
		camera.x += 1;
		break;
	case 'X':
		camera.x -= 1;
		break;
	case 'y':
		camera.y += 1;
		break;
	case 'Y':
		camera.y -= 1;
		break;
	case 'z':
		camera.z += 1;
		break;
	case 'Z':
		camera.z -= 1;
		break;
	case 'r':
		camera.x = 0;
		camera.y = 0;
		camera.z = 1;
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
	gluPerspective(45, WideSize / HighSize, 1, Z_Size); //윈도우를 초기화 하는 함수입니다!
	glTranslatef(0, 0, -300);
	glMatrixMode(GL_MODELVIEW);

}