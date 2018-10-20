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
int next_rot;
int st_help;
BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;

Shape tra;
int rot_count;
int rot_command;

Shape shape[2];
Shape view;


//카메라-----------------

Shape camera;
Translate_pos EYE;
Translate_pos AT;
Translate_pos UP;

static int __x;
static int __y;
static int __z;


const void camera_custom
(double pos_x, double pos_y, double pos_z, //위치
	double degree, const double rot_x, const double rot_y, const double rot_z, //회전
	const double move_x, const double move_y, const double move_z //움직임
) {


	EYE.x =
		((cos(rot_y) * cos(rot_z)) +
		(sin(rot_x) * sin(rot_y) * cos(rot_z) + cos(rot_x) * sin(rot_z)) +
			((((-1) * cos(rot_x)) * sin(rot_y) * cos(rot_z)) + (sin(rot_x) * sin(rot_z))));

	EYE.y =
		(((-1) * cos(rot_y) * sin(rot_z)) +
		(((-1) * sin(rot_x) * sin(rot_y) * sin(rot_z)) + (cos(rot_x) * cos(rot_z))) +
			((cos(rot_x) * sin(rot_y) * sin(rot_z)) + (sin(rot_x) * sin(rot_z))));

	EYE.z =
		(sin(rot_y) +
		(((-1) * sin(rot_x)) * cos(rot_y)) +
			(cos(rot_x) * cos(rot_y)));//stay

	AT.x = pos_x;
	AT.y = pos_y;
	AT.z = pos_z;

};



void SetupRC()
{
	//초기화
	glTranslated(WideSize / 2, HighSize / 2, Z_Size/2);
}
void main(int argc, char *argv[]) {
	//초기화
	camera.rot.x = 0;
	camera.rot.y = 0;
	camera.rot.z = 0;

	camera.move.x = 0;
	camera.move.y = 0;
	camera.move.z = 0;

	EYE.x = 0, EYE.y = 0, EYE.z = 300;//EYE백터 초기화
	AT.x = 0, AT.y = 0, AT.z = 0;//EYE백터 초기화
	UP.x = 0, UP.y = 1, UP.z = 0;//EYE백터 초기화

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
	
	glPushMatrix();//-----------------------------------
	//glRotated(-45, 0, 0, 1);
	glTranslated(camera.move.x, camera.move.y, camera.move.z);
	camera_custom(0, 0, 0, camera.rot.degree, camera.rot.x, camera.rot.y, camera.rot.z, camera.move.x, camera.move.y, camera.move.z);

	gluLookAt(
		EYE.x, EYE.y, EYE.z,  //위5 eye
		AT.x, AT.y, AT.z, //방향 center
		0, 1, 0 //위쪽방향(건들 ㄴㄴ) up
	);

	glPushMatrix();//---------------------------------------
				  
				  //glTranslated(0, 0, 0);
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
		glRotatef(move_count, 0, 1, 0);
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
		move_count += 1;//타이머 카운트
	}
	for (int i = 0; i < 2; i++) {
		if (shape[i].any) {
			shape[i].rot.degree += 2;
		}
	}

	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}
int ttt;
int ani_count;
void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
		//---------카메라
		//rotate
	case 'x':
		__x = 1;
		camera.rot.x -= 0.1;
		break;
	case 'X':
		camera.rot.x += 0.1;
		break;

	case 'y':
		__y = 1;
		camera.rot.y -= 0.1;
		break;
	case 'Y':
		camera.rot.y += 0.1;
		break;

	case 'z':
		__z = 1;
		camera.rot.z -= 0.1;
		break;
	case 'Z':
		camera.rot.z += 0.1;
		break;

		//move
	case 'w':
		camera.move.y += 1;
		break;
	case 'a':
		camera.move.x -= 1;
		break;

	case 's':
		camera.move.y -= 1;
		break;

	case 'd':
		camera.move.x += 1;
		break;
	case '+':
		camera.move.z += 1;
		break;
	case '-':
		camera.move.z -= 1;
		break;
	case 'i':
		camera.rot.degree = 0;
		camera.move.x = 0;
		camera.move.y = 0;
		camera.move.z = 0;
		camera.rot.x = 0;
		camera.rot.y = 0;
		camera.rot.z = 0;

		__x = 0;
		__y = 0;
		__z = 0;

		break;

		// z는 그대로 camera.z 

		//-----------카메라 끝 --------



	case 'L':
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
	case 'R':
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
	case 'O':
		ani = TRUE;
		ttt++;
		if (ttt % 2 == 0) {
			for (int i = 0; i < 2; i++) {
				shape[i].any = TRUE;
				shape[i].rot.x = 0;
				shape[i].rot.y = 1;
				shape[i].rot.z = 0;
			}
		}
		else {
			ani = FALSE;
		}
		
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
		next_rot++;
		for (int i = 0; i < 2; i++) {
			shape[i].select = (next_rot%4);
		}
		break;
	case '2'://직각투영 유무
		ani_count++;
		if (ani_count % 2 == 1) {
			ani = TRUE;//회전하는거 트루로
			Reshape(WideSize, HighSize);
		}
		else {
			ani = FALSE;//회전하는거 트루로
			Reshape(WideSize, HighSize);
		}
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


	if (!ani) {
		//glRotatef(30, 0, 1, 0);
		gluPerspective(60.0f, w / h, 1.0, 1000.0);

		glTranslated(0.0, 0.0, -300.0);     // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
	}
	else {
		glOrtho(0, WideSize, HighSize, 0, -Z_Size / 2, Z_Size / 2); //윈도우를 초기화 하는 함수입니다!
		glTranslated(WideSize / 2, HighSize / 2, 0);
		//glRotatef(-60, 1, 0, 0);

	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}