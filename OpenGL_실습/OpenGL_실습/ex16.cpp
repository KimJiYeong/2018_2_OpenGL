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

	BOOL any;

};

int change_count;
int select_count;
int st_help;
BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;

Translate_pos camera;
Shape tra;
int rot_count;
int rot_command;

Shape shape[2];

void SetupRC()
{
	//초기화
	glTranslated(WideSize / 2, HighSize / 2, Z_Size/2);
}
void main(int argc, char *argv[]) {
	//초기화
	shape[0].pos.x = 100;
	shape[1].pos.x = 100;
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
	glTranslated(0, 0, 0);
	gluLookAt(
		camera.x, camera.y, camera.z,  //위5 eye
		0, 0, 0, //방향 center
		0, 1, 0); //위쪽방향(건들 ㄴㄴ) up
	glLineWidth(2);
	glColor3f((float)255 / 255, (float)255 / 255, (float)255 / 255);
	glMatrixMode(GL_MODELVIEW);
	//좌표축 그리기
	
	glPushMatrix();
	glTranslated(0, -15, 0);
	glScalef(1, 0.001, 1);
	glutSolidCube(200);//발판 만들기
		
	glPopMatrix();//좌표계 그리기
	
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
	}//좌표계 그리기
	
	
	for (int i = 0; i < 2; i++) {
		glPushMatrix();//도형 그리기
		glRotatef(Time_count, 0, 1, 0);
		if (i == 0) {//첫번째 도형
			glTranslated(shape[i].pos.x, 0, 0);
			glRotatef(shape[i].rot.degree, shape[i].rot.x, shape[i].rot.y, shape[i].rot.z);
			if (!Look) {
				if (shape[i].select == 0) {
					glutSolidSphere(15, 10, 10);
				}
				else if (shape[i].select == 1) {
					glutSolidCube(30);
				}
				else if (shape[i].select == 2) {
					glutSolidCone(15, 10, 3, 3);
				}
				else if (shape[i].select == 3) {
					glutSolidTeapot(30);
				}
			}
			else {
				if (shape[i].select == 0) {
					glutWireSphere(15, 10, 10);
				}
				else if (shape[i].select == 1) {
					glutWireCube(30);
				}
				else if (shape[i].select == 2) {
					glutWireCone(15, 10, 3, 3);
				}
				else if (shape[i].select == 3) {
					glutWireTeapot(30);
				}
			}
		}

		if (i == 1) {//두번째 도형
			glTranslated(-shape[i].pos.x, 0, 0);
			glRotatef(shape[i].rot.degree, shape[i].rot.x, shape[i].rot.y, shape[i].rot.z);
			if (Look) {
				if (shape[i].select == 0) {
					glutSolidSphere(15, 10, 10);
				}
				else if (shape[i].select == 1) {
					glutSolidCube(30);
				}
				else if (shape[i].select == 2) {
					glutSolidCone(15, 10, 3, 3);
				}
				else if (shape[i].select == 3) {
					glutSolidTeapot(30);
				}
			}
			else {
				if (shape[i].select == 0) {
					glutWireSphere(15, 10, 10);
				}
				else if (shape[i].select == 1) {
					glutWireCube(30);
				}
				else if (shape[i].select == 2) {
					glutWireCone(15, 10, 3, 3);
				}
				else if (shape[i].select == 3) {
					glutWireTeapot(30);
				}
			}
		}
		//glRotatef(Time_count, 0, 1, 0);

		glPopMatrix();//도형 그리기
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
	if (!ani) {
		Time_count += 1;//타이머 카운트
	}
	for (int i = 0; i < 2; i++) {
		if (shape[i].any) {
			shape[i].rot.degree += 2;
		}
	}

	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'x':
		camera.x += 0.5;
		break;
	case 'X':
		camera.x -= 0.5;
		break;
	case 'y':
		camera.y += 0.5;
		break;
	case 'Y':
		camera.y -= 0.5;
		break;
	case 'z':
		camera.z += 0.5;
		break;
	case 'Z':
		camera.z -= 0.5;
		break;
	case 'l':
		ani = TRUE;
		for (int i = 0; i < 2; i++) {
			if (i == 0) {
				shape[i].any = TRUE;
				shape[i].rot.x = 0;
				shape[i].rot.y = 1;
				shape[i].rot.z = 0;
			}
			else {
				shape[i].any = FALSE;
			}
		}
		break;
	case 'r':
		ani = TRUE;
		for (int i = 0; i < 2; i++) {
			if (i != 0) {
				shape[i].any = TRUE;
				shape[i].rot.x = 0;
				shape[i].rot.y = 1;
				shape[i].rot.z = 0;
			}
			else {
				shape[i].any = FALSE;
			}
		}
		break;
	case 'o':
		ani = FALSE;
		for (int i = 0; i < 2; i++) {
				shape[i].any = TRUE;
				shape[i].rot.x = 0;
				shape[i].rot.y = 1;
				shape[i].rot.z = 0;
		}
		break;
	case 'p':
		camera.x = 0;
		camera.y = 0;
		camera.z = 1;
		break;
	case 'c'://도형 바꾸끼
	case 'C':
		change_count++;
		if (change_count % 2 == 1) {
			Look = TRUE;
		}
		else {
			Look = FALSE;
		}
		break;
	case '1':
		select_count++;
		for (int i = 0; i < 2; i++) {
			shape[i].select = (select_count%4);
		}
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