#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>
GLvoid Reshape(int w, int h);

//해상도 설정
#define WideSize 900
#define HighSize 900
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

Shape camera;
Shape tra;
int rot_count;
int rot_command;

Shape shape[36];
Shape view;
void SetupRC()
{
	//초기화
	glTranslated(WideSize / 2, HighSize / 2, Z_Size / 2);
}
void main(int argc, char *argv[]) {
	//초기화

	for (int i = 0; i < 36; i++) {
		shape[i].pos.x = 0;
		shape[i].pos.y = 0;

	}

	camera.pos.x = 0;
	camera.pos.y = 1;
	camera.pos.z = 1;

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
void rot_custom(int sel, int degree) {
	//x = 0
	//y = 1
	//z = 2
	GLdouble x_rotate[16] = {
		1, 0, 0, 0,
		0, cos(degree * PI * 10 / 180),  -sin(degree * PI * 10 / 180), 0,
		0, sin(degree * PI * 10 / 180), cos(degree * PI * 10 / 180), 0,
		0, 0, 0, 1
	};
	GLdouble y_rotate[16] = {
		cos(degree * PI * 10 / 180), 0, sin(degree * PI * 10 / 180), 0,
		0, 1,  0, 0,
		-sin(degree * PI * 10 / 180), 0,cos(degree * PI * 10 / 180), 0,
		0, 0, 0, 1
	};
	GLdouble z_rotate[16] = {
		cos(degree * PI * 10 / 180),  -sin(degree * PI * 10 / 180), 0,0,
		sin(degree * PI * 10 / 180), cos(degree * PI * 10 / 180), 0,0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	if (sel == 0) {
		glLoadMatrixd(x_rotate);
		glMultMatrixd(x_rotate);
	}
	else if (sel == 1) {

		glLoadMatrixd(y_rotate);
		glMultMatrixd(y_rotate);
	}
	else if (sel == 2) {

		glLoadMatrixd(z_rotate);
		glMultMatrixd(z_rotate);
	}
	
};
GLvoid drawScene(GLvoid)
{
	//오늘의 나에게 내가 지금 너무 피곤해서 죽을것같으니
	//
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();{

		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		//glRotatef(view.rot.degree, view.rot.x, view.rot.y, view.rot.z);
		rot_custom(select_count, camera.rot.degree);
		gluLookAt(
			0, 0 , 1,  //위5 eye
			0, 0, 0, //방향 center
			0, 1, 0); //위쪽방향(건들 ㄴㄴ) up
		//glTranslated(0, 0, 0);
		
		glLineWidth(2);
	
		//가운데 막대 그리기
		glColor3f((float)255 / 255, (float)0 / 255, (float)0 / 255);
		glTranslated(0, 0, 0);
			//for (int i = 0; i < 3; i++) {

			//	glPushMatrix();
			//	if (i == 0) {
			//		glColor3f((float)255 / 255, (float)0 / 255, (float)0 / 255);
			//		glRotated(90, 1, 0, 0);
			//	}
			//	else if (i == 1) {
			//		glColor3f((float)0 / 255, (float)255 / 255, (float)0 / 255);
			//		glRotated(90, 0, 1, 0);
			//	}
			//	else if (i == 2) {
			//		glColor3f((float)0 / 255, (float)0 / 255, (float)255 / 255);
			//		glRotated(90, 0, 0, 1);
			//	}
			//	glScalef(1, 0.1, 0.1);
			//	glutSolidCube(40);
			//	glPopMatrix();
			//	glPushMatrix();
			//	glScaled(1, 0.01, 1);
			//	glutSolidCube(20);
			//	glPopMatrix();

			//}//좌표계 그리기

			//그리기
			glPushMatrix();
			glTranslated(0, 0, 0);
			glutSolidSphere(20, 20, 20);
			glPopMatrix();
			//그리기 끝

			//그리기
			glPushMatrix();
			glTranslated(70, 0, 0);

			for (int i = 0; i < 3; i++) {
				glColor3f((float)0 / 255, (float)0 / 255, (float)255 / 255);
				glTranslated(0, 0, 0);
				//glPushMatrix();
				if (i == 0) {
					glRotatef(90, 1, 0, 0);
				}
				else if (i == 1) {
					glRotatef(90, 0, 1, 0);
				}
				else if (i == 2) {
					glRotatef(90, 0, 0, 1);
				}
				//glPopMatrix();
				glutSolidSphere(15, 15, 15);
			}
			glPopMatrix();
			//그리기 끝

			 //glRotatef(Time_count, 0, 1, 0);
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
	
	//------------------카메라------------------------
	case 'x':
		camera.rot.degree += 1;
		select_count = 0;
		// z는 그대로 camera.z 
		break;
	case 'y':
		camera.rot.degree += 1;
		select_count = 1;
		// z는 그대로 camera.z 
		break;
	case 'z':
		camera.rot.degree += 1;
		select_count = 2;


		// z는 그대로 camera.z 
		break;

		//-----------카메라 끝 --------
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
	glTranslated(0, 0, -300);
	glMatrixMode(GL_MODELVIEW);

}