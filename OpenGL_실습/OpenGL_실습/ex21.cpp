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
	Translate_pos pos;
	Translate_pos move;
	Translate_pos scale;
	Translate_pos rot;

	int size;
	int select;
	int height;
	int slice;
	int stacks;
	BoOL b;
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

Shape shape;
Shape sub[2];

//카메라-----------------

Shape camera;
Translate_pos EYE;
Translate_pos AT;
Translate_pos UP;

static int __x;
static int __y;
static int __z;

Shape tree;//나무
//Shape tree;
Shape chin_man;//철봉사람
Shape run_man;//철봉사람
Shape run;//철봉사람

Shape press_man;
			  //Shape tree;


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
	glTranslated(WideSize / 2, HighSize / 2, Z_Size / 2);
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

	shape.size = 30;

	shape.pos.x = 0;
	shape.pos.y = 0;
	shape.pos.z = 0;

	//shape.rot.degree = 0;
	shape.rot.x = 0;
	shape.rot.y = 0;
	shape.rot.z = 0;
	shape.stacks = 1;

	run_man.move.degree = 1;
	for (int i = 0; i < 2; i++) {
		sub[i].size = 30;
	}

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

#define BOX_SIZE 200
//2파이 r
GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();//-----------------------------------
	{

		//glRotated(-45, 0, 0, 1);
		glTranslated(camera.move.x, camera.move.y, camera.move.z);
		camera_custom(0, 0, 0, camera.rot.degree, camera.rot.x, camera.rot.y, camera.rot.z, camera.move.x, camera.move.y, camera.move.z);

		gluLookAt(
			EYE.x, EYE.y, EYE.z,  //위5 eye
			AT.x, AT.y, AT.z, //방향 center
			0, 1, 0 //위쪽방향(건들 ㄴㄴ) up
		);
		//들어갈 내용
		glPushMatrix();//---------------------------------------
		{
			//땅
			glPushMatrix();
			{
				glPushMatrix();
				glColor3d((float)134 / 255, (float)229 / 255, (float)127 / 255);
				glTranslated(0, -2, 0);
				glScaled(1, 0.01, 1);
				glutSolidCube(BOX_SIZE);
				glPopMatrix();
							
				glPushMatrix();
				glColor3d((float)71 / 255, (float)200 / 255, (float)62 / 255);
				glTranslated(0, -1, 0);
				glScaled(1, 0.01, 1);
				glRotated(45, 0, 1, 0);
				glutSolidCube(BOX_SIZE - 60);
				glPopMatrix();

			}glPopMatrix();//땅 끝

			glPushMatrix();//벤치 프레스 
			{
				glColor3d((float)204 / 255, (float)61 / 255, (float)61 / 255);
				glTranslated(-BOX_SIZE/2 + 20/2, 0, -BOX_SIZE/2 + 20/2);//위치 변경
				for (int i = 0; i < 2; i++) {
					glPushMatrix();
					if (i == 0) {
						glTranslated(-10, 0, 0);
					}
					else {
						glTranslated(10, 0, 0);
					}

					glScaled(0.2, 1, 0.2);
					glRotated(-90, 1, 0, 0);
					glutSolidCylinder(10 / 2, 30, 6, 6);//나무 몸통
					glPopMatrix();
				}
				glColor3d((float)67 / 255, (float)116 / 255, (float)217 / 255);

				glPushMatrix();
				glTranslated(-10, 10, 30);
				glRotated(90, 0, 1, 0);
				glPushMatrix();

				glPushMatrix();
				//glRotated(-run_man.rot.degree + 90 + 30, 1, 0, 0);
				glutSolidCylinder(2 / 2, 10, 6, 6);//나무 몸통
				glPopMatrix();
				glPopMatrix();
				glPopMatrix();

				glColor3d((float)76 / 255, (float)76 / 255, (float)76 / 255);
				glPushMatrix();//사람
				{
					glTranslated(0, 10, 30);
					glRotated(-90, 1, 0, 0);
					glRotated(180, 0, 1, 0);
					for (int i = 0; i < 4; i++) {
						glPushMatrix();//사람
						if (i < 2) {
							glTranslated(0, -10, 0);
							glPushMatrix();//사람
							if (i % 2 == 0) {
								glTranslated(-2, 10, 0);
								glRotated(-90, 1, 0, 0);
							}
							else {
								glTranslated(2, 10, 0);
								glRotated(-90, 1, 0, 0);
							}
						}
						else {
							glTranslated(0, 10, 0);
							//glRotated(90, 1, 0, 0);
							glPushMatrix();
							if (i % 2 == 0) {
								glTranslated(-2, 10, 0);
								glRotated(-run_man.rot.degree + 90 + 30, 1, 0, 0);
							}
							else {
								glTranslated(2, 10, 0);
								glRotated(-run_man.rot.degree + 90 + 30, 1, 0, 0);
							}
						}

						glutSolidCylinder(2 / 2, 10, 6, 6);//나무 몸통
						glPopMatrix();
						glPopMatrix();//사람 끝
					}
					//몸
					glPushMatrix();
					glTranslated(0, 15, 0);
					glScaled(0.5, 1, 0.4);
					glutSolidCube(10);
					glPopMatrix();

					//머리
					glPushMatrix();
					glTranslated(0, 25, 0);
					glutSolidSphere(2, 8, 8);
					glPopMatrix();
				}
				glPopMatrix();//사람 끝

			}glPopMatrix();//벤치 프레스 끝
		
			glPushMatrix();//나무
			{
				glColor3d((float)204 / 255, (float)61 / 255, (float)61 / 255);
				glTranslated(BOX_SIZE / 2 - 20 , 0, -BOX_SIZE / 2 + 20 );//위치 변경
				glPushMatrix();
				glScaled(0.5, 1, 0.5);
				glRotated(-90, 1, 0, 0);
				glutSolidCylinder(20/2, 20, 6,6);//나무 몸통
				glPopMatrix();
				
				//나무 머리
				glPushMatrix();
				glColor3d((float) 34/ 255, (float)116 / 255, (float)28 / 255);
				glTranslated(0, 20, 0);
				glScaled(tree.scale.x * 2, tree.scale.x * 2, tree.scale.x * 2);
				glutSolidSphere(20, 10, 10);
				glPopMatrix();

			}glPopMatrix();//나무 끝

			glPushMatrix();//철봉
			{
				glColor3d((float)153 / 255, (float)112 / 255, (float)0 / 255);
				glTranslated(-BOX_SIZE / 2 + 20 / 2, 0, BOX_SIZE / 2 - 20 / 2);//위치 변경
				
				for (int i = 0; i < 2; i++) {
					glPushMatrix();
					if (i == 0) {
						glTranslated(-10, 0, -10);
					}
					else {
						glTranslated(10, 0, -10);
					}
					
					glScaled(0.2, 1, 0.2);
					glRotated(-90, 1, 0, 0);
					glutSolidCylinder(10 / 2, 30, 6, 6);//나무 몸통
					glPopMatrix();
				}
				glColor3d((float)67 / 255, (float)116 / 255, (float)217 / 255);

				glPushMatrix();
				glTranslated(-10, 25, -10);
				glPushMatrix();
				glRotated(90, 0, 1, 0);
				glutSolidCylinder(5 / 2, 20, 6, 6);//나무 몸통
				glPopMatrix();
				glPopMatrix();

				glColor3d((float)76 / 255, (float)76 / 255, (float)76 / 255);
				glPushMatrix();//사람
				{
					glTranslated(0, 25, -10);
					glRotated(chin_man.rot.degree, 1, 0, 0);

					for (int i = 0; i < 4; i++) {
						glPushMatrix();//사람
						if (i < 2) {
							glTranslated(0, -10, 0);
							if (i %2 == 0) {
								glTranslated(-2, 10, 0);
								glRotated(-90, 1, 0, 0);
							}
							else {
								glTranslated(2, 10, 0);
								glRotated(-90, 1, 0, 0);
							}
						}
						else {
							glTranslated(0, 10, 0);
							if (i % 2== 0) {
								glTranslated(-2, 10, 0);
								glRotated(-90, 1, 0, 0);
							}
							else {
								glTranslated(2, 10, 0);
								glRotated(-90, 1, 0, 0);
							}
						}

						glutSolidCylinder(2 / 2, 10, 6, 6);//나무 몸통
					
						glPopMatrix();//사람 끝
					}
					//몸
					glPushMatrix();
					glTranslated(0, 15, 0);
					glScaled(0.5, 1, 0.4);
					glutSolidCube(10);
					glPopMatrix();

					//머리
					glPushMatrix();
					glTranslated(0, 25, 0);
					glutSolidSphere(2, 8,8);
					glPopMatrix();
				}
				glPopMatrix();//사람 끝

			}glPopMatrix();//철봉 끝

			glPushMatrix();//런닝머신
			{
				glColor3d((float)204 / 255, (float)61 / 255, (float)61 / 255);
				glTranslated(BOX_SIZE / 2 - 20 / 2, 0, BOX_SIZE / 2 - 20 / 2);//위치 변경
				
				for (int i = 0; i < 2; i++) {
					glPushMatrix();
					if (i == 0) {
						glTranslated(-10, 0, -10);
					}
					else {
						glTranslated(10, 0, -10);
					}

					glScaled(0.2, 1, 0.2);
					glRotated(-90, 1, 0, 0);
					glutSolidCylinder(10 / 2, 30, 6, 6);//나무 몸통
					glPopMatrix();
				}
				glColor3d((float)67 / 255, (float)116 / 255, (float)217 / 255);

				glPushMatrix();
				glTranslated(-10, 25, -10);
				glPushMatrix();
				glRotated(90, 0, 1, 0);
				glutSolidCylinder(5 / 2, 20, 6, 6);//나무 몸통
				glPopMatrix();
				glPopMatrix();
			
				glColor3d((float)25 / 255, (float)25 / 255, (float)25 / 255);
				
				glPushMatrix();
				glTranslated(0, 0, -30);
				glRotated(90, 0, 1, 0);
				glScaled(1, 0.1, 1);
				glPushMatrix();
				glRotated(-run.rot.degree, 0, 0, 1);
				glutWireTorus(10, 20, 6, 6);//나무 몸통
				glPopMatrix();
				glPopMatrix();
 
				glColor3d((float)166 / 255, (float)166 / 255, (float)166 / 255);

				glPushMatrix();
				glTranslated(0, 0, -30);
				glRotated(90, 0, 1, 0);
				glScaled(1, 0.07, 1);
				glPushMatrix();
				glutSolidTorus(10, 20, 6, 6);//나무 몸통
				glPopMatrix();
				glPopMatrix();

				glColor3d((float)6 / 255, (float)0 / 255, (float)0 / 255);

				glPushMatrix();//사람
				{
					glTranslated(0, 0, -30);

					for (int i = 0; i < 4; i++) {
						glPushMatrix();//사람
						if (i < 2) {
							glTranslated(0, 0, 0);
							if (i % 2 == 0) {
								glTranslated(-2, 10, 0);
								glRotated(run_man.rot.degree + 90 , 1, 0, 0);
							}
							else {
								glTranslated(2, 10, 0);
								glRotated(-run_man.rot.degree + 90, 1, 0, 0);
							
							}
						}
						//팔
						else {
							glTranslated(0, 10, 0);
							if (i % 2 == 0) {
								glTranslated(-2, 10, 0);
								glRotated(50, 1, 0, 0);
							}
							else {
								glTranslated(2, 10, 0);
								glRotated(50, 1, 0, 0);
							}
						}

						glutSolidCylinder(2 / 2, 10, 6, 6);//나무 몸통

						glPopMatrix();//사람 끝
					}
					//몸
					glPushMatrix();
					glTranslated(0, 15, 0);
					glScaled(0.5, 1, 0.4);
					glutSolidCube(10);
					glPopMatrix();

					//머리
					glPushMatrix();
					glTranslated(0, 25, 0);
					glutSolidSphere(2, 8, 8);
					glPopMatrix();
				}
				glPopMatrix();//사람 끝


			}glPopMatrix();//러닝머신 끝

		}
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
int Time_count;
void Timerfunction(int value) {
	
	//타이머 내용
	if (Time_count % 20 == 0) {
		if (tree.any) {
			tree.any = FALSE;
		}
		else {
			tree.any = TRUE;
		}
	
	}
	if (Time_count % 20 == 0) {
		if (run_man.any) {
			run_man.any = FALSE;
			run_man.move.degree *= -1;
		}
		else {
			run_man.any = TRUE;
			run_man.move.degree *= -1;
		}

	}

	if (!tree.any) {
			tree.scale.x += 0.01;
	}
	else {
			tree.scale.x -= 0.01;
	}

	if (!run_man.any) {
		run_man.rot.degree += 2;
	}
	else {
		run_man.rot.degree -= 2;
	}
	chin_man.rot.degree += 1;

	run.rot.degree += 10;
	
	Time_count++;
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