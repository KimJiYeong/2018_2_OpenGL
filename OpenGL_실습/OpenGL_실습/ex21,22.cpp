#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>
//#include "other_draw.h"
#include "base_struct.h"
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
void SpecialKeys(int key, int x, int y);

#define PT 20//도형 갯수 설정
#define PI 3.141592 //파이

int change_count;
int next_rot;
int st_help;
BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;

Shape tra;
int rot_count;
int rot_command;

Shape sub[2];

//카메라-----------------

Shape camera;
Translate_pos EYE;
Translate_pos AT;
Translate_pos UP;

static int __x;
static int __y;
static int __z;


#define Size 36

Shape tree;//나무
//Shape tree;
Shape chin_man;//철봉사람
Shape run_man;//철봉사람
Shape run;//철봉사람
Shape plan;
Shape press_man;
			  //Shape tree;
Shape ball;
Shape clain;
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

Shape sh[100];

//-------------

typedef struct QUAD
{
	Color cl[8];//색상
	Translate_pos pos[8];
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

//컬링
BOOL culling;
int culling_count;
QUAD index;


void Inde_Box(int pos_x, int pos_y, int pos_z, int size) {

	//인덱스 버퍼 저장
	index.pos[0].x = -1;
	index.pos[0].y = -1;
	index.pos[0].z = 1;

	index.cl[0].R = 1;
	index.cl[0].G = 0;
	index.cl[0].B = 0;

	index.pos[1].x = 1;
	index.pos[1].y = -1;
	index.pos[1].z = 1;

	index.cl[1].R = 1;
	index.cl[1].G = 0;
	index.cl[1].B = 1;

	index.pos[2].x = 1;
	index.pos[2].y = -1;
	index.pos[2].z = -1;

	index.cl[2].R = 0;
	index.cl[2].G = 0;
	index.cl[2].B = 1;

	index.pos[3].x = -1;
	index.pos[3].y = -1;
	index.pos[3].z = -1;

	index.cl[3].R = 0;
	index.cl[3].G = 0;
	index.cl[3].B = 0;

	index.pos[4].x = -1;
	index.pos[4].y = 1;
	index.pos[4].z = 1;

	index.cl[4].R = 1;
	index.cl[4].G = 1;
	index.cl[4].B = 0;

	index.pos[5].x = 1;
	index.pos[5].y = 1;
	index.pos[5].z = 1;

	index.cl[5].R = 1;
	index.cl[5].G = 1;
	index.cl[5].B = 1;

	index.pos[6].x = 1;
	index.pos[6].y = 1;
	index.pos[6].z = -1;

	index.cl[6].R = 0;
	index.cl[6].G = 1;
	index.cl[6].B = 1;

	index.pos[7].x = -1;
	index.pos[7].y = 1;
	index.pos[7].z = -1;

	index.cl[7].R = 0;
	index.cl[7].G = 1;
	index.cl[7].B = 0;

	glPushMatrix(); {

		glTranslated(pos_x, pos_y, pos_z);
		glScaled(size, size, size);

		glBegin(GL_POLYGON); {//아래

			glColor3f(index.cl[1].R, index.cl[1].G, index.cl[1].B);
			glVertex3f(index.pos[1].x, index.pos[1].y, index.pos[1].z);//Yellow

			glColor3f(index.cl[0].R, index.cl[0].G, index.cl[0].B);
			glVertex3f(index.pos[0].x, index.pos[0].y, index.pos[0].z);//Yellow

			glColor3f(index.cl[3].R, index.cl[3].G, index.cl[3].B);
			glVertex3f(index.pos[3].x, index.pos[3].y, index.pos[3].z);//Yellow

			glColor3f(index.cl[2].R, index.cl[2].G, index.cl[2].B);
			glVertex3f(index.pos[2].x, index.pos[2].y, index.pos[2].z);//Yellow

		}
		glEnd();

		glBegin(GL_POLYGON); {//왼
			glColor3f(index.cl[0].R, index.cl[0].G, index.cl[0].B);
			glVertex3f(index.pos[0].x, index.pos[0].y, index.pos[0].z);//Yellow

			glColor3f(index.cl[4].R, index.cl[4].G, index.cl[4].B);
			glVertex3f(index.pos[4].x, index.pos[4].y, index.pos[4].z);//Yellow

			glColor3f(index.cl[7].R, index.cl[7].G, index.cl[7].B);
			glVertex3f(index.pos[7].x, index.pos[7].y, index.pos[7].z);//Yellow

			glColor3f(index.cl[3].R, index.cl[3].G, index.cl[3].B);
			glVertex3f(index.pos[3].x, index.pos[3].y, index.pos[3].z);//Yellow

		}
		glEnd();

		glBegin(GL_POLYGON); {//뒷면

			glColor3f(index.cl[2].R, index.cl[2].G, index.cl[2].B);
			glVertex3f(index.pos[2].x, index.pos[2].y, index.pos[2].z);//Yellow

			glColor3f(index.cl[3].R, index.cl[3].G, index.cl[3].B);
			glVertex3f(index.pos[3].x, index.pos[3].y, index.pos[3].z);//Yellow

			glColor3f(index.cl[7].R, index.cl[7].G, index.cl[7].B);
			glVertex3f(index.pos[7].x, index.pos[7].y, index.pos[7].z);//Yellow

			glColor3f(index.cl[6].R, index.cl[6].G, index.cl[6].B);
			glVertex3f(index.pos[6].x, index.pos[6].y, index.pos[6].z);//Yellow


		}
		glEnd();

		glBegin(GL_POLYGON); {//오른쪽면
			glColor3f(index.cl[1].R, index.cl[1].G, index.cl[1].B);
			glVertex3f(index.pos[1].x, index.pos[1].y, index.pos[1].z);//Yellow

			glColor3f(index.cl[2].R, index.cl[2].G, index.cl[2].B);
			glVertex3f(index.pos[2].x, index.pos[2].y, index.pos[2].z);//Yellow

			glColor3f(index.cl[6].R, index.cl[6].G, index.cl[6].B);
			glVertex3f(index.pos[6].x, index.pos[6].y, index.pos[6].z);//Yellow					

			glColor3f(index.cl[5].R, index.cl[5].G, index.cl[5].B);
			glVertex3f(index.pos[5].x, index.pos[5].y, index.pos[5].z);//Yellow

		}
		glEnd();
		glPushMatrix();
		{
			glTranslated(0, index.pos[7].y, index.pos[7].z);
			glRotated(-index.move.x * 10, 1, 0, 0);
			glTranslated(0, -index.pos[7].y, -index.pos[7].z);

			glBegin(GL_POLYGON); {//뚜껑

				glColor3f(index.cl[4].R, index.cl[4].G, index.cl[4].B);
				glVertex3f(index.pos[4].x, index.pos[4].y, index.pos[4].z);//Yellow

				glColor3f(index.cl[5].R, index.cl[5].G, index.cl[5].B);
				glVertex3f(index.pos[5].x, index.pos[5].y, index.pos[5].z);//Yellow

				glColor3f(index.cl[6].R, index.cl[6].G, index.cl[6].B);
				glVertex3f(index.pos[6].x, index.pos[6].y, index.pos[6].z);//Yellow				

				glColor3f(index.cl[7].R, index.cl[7].G, index.cl[7].B);
				glVertex3f(index.pos[7].x, index.pos[7].y, index.pos[7].z);//Yellow

			}
			glEnd();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslated(0, index.pos[0].y, index.pos[0].z);
			glRotated(index.move.y * 10, 1, 0, 0);
			glTranslated(0, -index.pos[0].y, -index.pos[0].z);

			glBegin(GL_POLYGON); {//앞면

				glColor3f(index.cl[0].R, index.cl[0].G, index.cl[0].B);
				glVertex3f(index.pos[0].x, index.pos[0].y, index.pos[0].z);//Yellow

				glColor3f(index.cl[1].R, index.cl[1].G, index.cl[1].B);
				glVertex3f(index.pos[1].x, index.pos[1].y, index.pos[1].z);//Yellow

				glColor3f(index.cl[5].R, index.cl[5].G, index.cl[5].B);
				glVertex3f(index.pos[5].x, index.pos[5].y, index.pos[5].z);//Yellow


				glColor3f(index.cl[4].R, index.cl[4].G, index.cl[4].B);
				glVertex3f(index.pos[4].x, index.pos[4].y, index.pos[4].z);//Yellow
			}
			glEnd();
		}
		glPopMatrix();
	}
	glPopMatrix();

}

Shape man_man;

//---------------

void SetupRC()
{
	//초기화
//	glTranslated(WideSize / 2, HighSize / 2, Z_Size / 2);

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

	ball.size = 20;

	ball.pos.x = 0;
	ball.pos.y = 0;
	ball.pos.z = 0;

	//shape.rot.degree = 0;
	ball.rot.x = 0;
	ball.rot.y = 0;
	ball.rot.z = 0;
	ball.stacks = 1;

	plan.scale.x = 1.0f;
	plan.scale.y = 0.5f;
	plan.scale.z = 0.3f;


	plan.pos.degree = 1;
	run_man.move.degree = 1;
	for (int i = 0; i < 2; i++) {
		sub[i].size = 30;
	}
	for (int i = 0; i < Size; i++) {
		sh[i].pos.y = 50 * sin(PI * i * 10 / 180);
		sh[i].pos.x = 50 * cos(PI * i * 10 / 180);

	}


	clain.size = 20;

	clain.pos.x = 0;
	clain.pos.y = 0;
	clain.pos.z = 0;

	//shape.rot.degree = 0;
	clain.rot.x = 0;
	clain.rot.y = 0;
	clain.rot.z = 0;
	clain.stacks = 1;

	for (int i = 0; i < 2; i++) {
		sub[i].size = 10;
	}
	ball.stacks = 1;

	plan.move.y = 90;
	man_man.rot.x = 1;
	man_man.move.y = 20;
//컬링 여부
	//glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);


}
void main(int argc, char *argv[]) {
	

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);//윈도우 띄우기 좌표
	glutInitWindowSize(WideSize, HighSize); //윈도우 띄우기 크기
	glutCreateWindow("ex16");
	// - 랜덤으로 시작 도형 설정하기
	//도형 그리기
	SetupRC();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeys);
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
		//컬링 여부
		if (culling) {
			//glFrontFace(GL_CCW);
			glEnable(GL_CULL_FACE);
		}
		else {
			//glFrontFace(GL_CCW);
			glDisable(GL_CULL_FACE);
		}


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
			glPushMatrix();
			{
				Inde_Box(0, 0, 0, 110);
			}
			glPopMatrix();


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

						   //삼각형 판
			glPushMatrix();
			{
				for (int i = 0; i < 4; i++) {
				glTranslated(0, 0, 0);//위치 이동
				glRotated(i * 90, 0, 1, 0);
				glPushMatrix();//
				{
					glColor3d((float)((71 * i)% 255 )/ 255, (float)((10 * i) % 255) / 255, (float)((200 * i) % 255) / 255);
					glBegin(GL_TRIANGLES);
					glVertex3d(BOX_SIZE / 2, 0, 0);
					glVertex3d(0, 0, BOX_SIZE / 2);
					glVertex3d(BOX_SIZE / 2, 0, BOX_SIZE / 2);
					glEnd();
				}
				glPopMatrix();
				}
			}
			glPopMatrix();



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
				glTranslated(-10, 10, 0);
				glRotated(90, 0, 1, 0);
				glPushMatrix();
				glTranslated(0, press_man.move.y, 0);
				glPushMatrix();
				//glRotated(-run_man.rot.degree + 90 + 30, 1, 0, 0);
				
				glutSolidCylinder(4 / 2, 20, 6, 6);//나무 몸통
				glPopMatrix();
				glPopMatrix();
				glPopMatrix();

				glColor3d((float)76 / 255, (float)76 / 255, (float)76 / 255);
				glPushMatrix();//사람
				{
					glTranslated(0, 5, 20);
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

							glutSolidCylinder(2 / 2, 10, 6, 6);//나무 몸통
							glPopMatrix();
							glPopMatrix();//사람 끝
						}
						else {
							glTranslated(0, 10, 0);
							//glRotated(90, 1, 0, 0);
							
							for (int k = 0; k < 2; k++) {
								glPushMatrix();
								if (k == 0) {
									if (i % 2 == 0) {
										glTranslated(-2, 10, 0);
										glRotated(-run_man.rot.degree + 90 + 30, 0, -1, 0);
									}
									else {
										glTranslated(2, 10, 0);
										glRotated(-run_man.rot.degree + 90 + 30, 0, 1, 0);
									}
									glutSolidCylinder(2 / 2, 5, 6, 6);//나무 몸통
								}
								else {
									
									if (i % 2 == 0) {
										glTranslated(-5, 10, -15);
										glRotated(10, 0, 0, 1);
										glTranslated( 0,0,-press_man.move.y + 10);
										
									}
									else {
										glTranslated(5, 10, -15);
										glRotated(10, 0, 0, 1);
										glTranslated(0,0,-press_man.move.y + 10);
									
									}
									glutSolidCylinder(2 / 2, press_man.move.y + 2, 6, 6);//나무 몸통
								}
								glPopMatrix();
							}
						
							
						
							glPopMatrix();//사람 끝
						}

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

			glPushMatrix();//비행기
			{
				glColor3d((float)204 / 255, (float)61 / 255, (float)61 / 255);
				glTranslated(plan.move.x * plan.pos.degree, plan.move.y, plan.move.z);//위치 변경
				glScaled(0.5, 0.5, 0.5);
				glPushMatrix();
				glRotated(-plan.rot.degree * plan.pos.degree, 0, 1, 0);
				
				//비행기 몸체 머리
				glPushMatrix();
				glColor3d((float)255 / 255, (float)187 / 255, (float)0 / 255);
				glScaled(0.5, 0.5, 1.5);
				glutSolidSphere(10, 10, 10);
				glPopMatrix();

				glPushMatrix();
				glColor3d((float)204 / 255, (float)61 / 255, (float)61 / 255);
				glScaled(2, 0.5, 0.5);
				glutSolidSphere(10, 10, 10);
				glPopMatrix();

				//비행기 꼬리
				for (int i = 0; i < 2; i++) {
					glPushMatrix();
					glColor3d((float)92 / 255, (float)209 / 255, (float)229 / 255);
					if (i == 0) {
						glTranslated(10, -2, 0);
					}
					else {
						glTranslated(-10, -2, 0);
					}
					glScaled(plan.scale.x, plan.scale.x, plan.scale.x);
					glutSolidSphere(5, 10, 10);
					glPopMatrix();
				}
				for (int i = 0; i < 2; i++) {
					glPushMatrix();
					glColor3d((float)92 / 255, (float)209 / 255, (float)229 / 255);
					if (i == 0) {
						glTranslated(10, -2, -10);
					}
					else {
						glTranslated(-10, -2, -10);
					}
					glScaled(plan.scale.y, plan.scale.y, plan.scale.y);
					glutSolidSphere(5, 10, 10);
					glPopMatrix();
				}
				for (int i = 0; i < 2; i++) {
					glPushMatrix();
					glColor3d((float)92 / 255, (float)209 / 255, (float)229 / 255);
					if (i == 0) {
						glTranslated(10, -2, -20);
					}
					else {
						glTranslated(-10, -2, -20);
					}
					glScaled(plan.scale.z, plan.scale.z, plan.scale.z);
					glutSolidSphere(5, 10, 10);
					glPopMatrix();
				}


				glPushMatrix();
				glColor3d((float)152 / 255, (float)0 / 255, (float)0 / 255);
				glTranslated(0,0, - 10);
				glScaled(1, 1, 1);
				glutSolidSphere(5, 10, 10);
				glPopMatrix();

				//프로펠러
					glPushMatrix();
					glColor3d((float)67 / 255, (float)116 / 255, (float)217 / 255);
					glTranslated(0, 0, 15);
					glRotated(plan.rot.degree + 90, 0, 0, 1);
					glScaled(1, 0.4, 0.2);
						glPushMatrix();
						glutSolidSphere(5, 10, 10);
						glPopMatrix();
					
					glPopMatrix();
				
					glPushMatrix();
					glColor3d((float)67 / 255, (float)116 / 255, (float)217 / 255);
					glTranslated(0, 0, 15);
					glRotated(plan.rot.degree + 0, 0, 0, 1);
					glScaled(1, 0.4, 0.2);
					glPushMatrix();
					glutSolidSphere(5, 10, 10);
					glPopMatrix();

					glPopMatrix();

			}glPopMatrix();//비행기 끝
			glPopMatrix();

			glPushMatrix();//래킹볼
			{
				glTranslatef(ball.pos.x + ball.move.x, ball.pos.y + ball.move.y + ball.size, ball.pos.z + ball.move.z);

				glPushMatrix();
				{

					glRotatef(ball.rot.degree, ball.rot.x, ball.rot.y, ball.rot.z);

					glutWireSphere(ball.size, 10, 10);
					//glRotatef(Time_count, 0, 1, 0);

				}
				glPopMatrix();
			}
			glPopMatrix();

			glPushMatrix();//크레인
			{
				glTranslatef(clain.move.x, clain.move.y, clain.move.z);
				
				glPushMatrix();
				{
					if (clain.any) {
						glRotatef(clain.rot.degree, clain.rot.x, clain.rot.y, clain.rot.z);
					}

					glutSolidCube(clain.size);
					//glRotatef(Time_count, 0, 1, 0);
				}
				glPushMatrix();
				{
					glRotatef(sub[0].rot.degree, sub[0].rot.x, 0, 0);
					glRotatef(sub[0].rot.z, 0, sub[0].rot.y, 0);
					glTranslatef(0, sub[0].size, 0);
					glScalef(0.7, 1, 0.7);
					glColor3f((float)0 / 255, (float)100 / 255, (float)0 / 255);
					glutSolidCube(sub[0].size);

					glPushMatrix();
					{
						glRotatef(sub[1].rot.degree, sub[1].rot.x, 0, 0);
						glRotatef(sub[1].rot.y, 0, 0, sub[1].rot.z);
						glTranslatef(0, sub[1].size, 0);
						glScalef(0.5, 1, 0.5);
						glColor3f((float)100 / 255, (float)0 / 255, (float)0 / 255);
						glutSolidCube(sub[1].size);

					}
					glPopMatrix();

				}
				glPopMatrix();

				glPopMatrix();
			}
			glPopMatrix();//크레인 끝

			//로봇
			glPushMatrix();
			{
				glTranslated(man_man.move.x, man_man.move.y, man_man.move.z);
				glRotated(man_man.move.degree, 0, 1, 0);

				glColor3f((float)255 / 255, (float)200 / 255, (float)100 / 255);
				glPushMatrix();//머리
				{
					glTranslated(0, 10, 0);
					glScaled(0.5, 1, 0.5);
					glutSolidCube(10);
				}
				glPopMatrix();

				glColor3f((float)255 / 255, (float)100 / 255, (float)100 / 255);
				glPushMatrix();//몸통
				{
					glScaled(0.5, 1, 0.5);
					glutSolidCube(15);
				}
				glPopMatrix();

				glColor3f((float)100 / 255, (float)100 / 255, (float)100 / 255);
				glPushMatrix();//팔다리
				{
								
					glTranslated(0, 10, 0);
					glRotated(man_man.rot.degree, 1, 0, 0);
					glTranslated(-5, -10, 0);
					glScaled(0.2, 1, 0.2);
					
					glutSolidCube(10);
					
				}
				glPopMatrix();
				
				glColor3f((float)100 / 255, (float)100 / 255, (float)100 / 255);
				glPushMatrix();//팔다리
				{
					glTranslated(0, 10, 0);
					glRotated(-man_man.rot.degree, 1, 0, 0);
					glTranslated(5, -10, 0);
					glScaled(0.2, 1, 0.2);

					glutSolidCube(10);

				}
				glPopMatrix();

				glColor3f((float)100 / 255, (float)50 / 255, (float)100 / 255);
				glPushMatrix();//팔다리
				{
					glTranslated(-2, -10, 0);

					glTranslated(0, 10, 0);
					glRotated(-man_man.rot.degree, 1, 0, 0);
					glTranslated(0, -10, 0);

					glScaled(0.2, 1, 0.2);
					glutSolidCube(10);	}
				glPopMatrix();

				glColor3f((float)100 / 255, (float)50 / 255, (float)100 / 255);
				glPushMatrix();//팔다리
				{
					glTranslated(2, -10, 0);
			
					glTranslated(0, 10, 0);
					glRotated(man_man.rot.degree, 1, 0, 0);
					glTranslated(0, -10, 0);

					glScaled(0.2, 1, 0.2);
					glutSolidCube(10);
				}
				glPopMatrix();


			}
			glPopMatrix();

		
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
		press_man.move.x -= 0.5;
		press_man.move.y -= 0.5;
	}
	else {
		run_man.rot.degree -= 2;
		press_man.move.x += 0.5;
		press_man.move.y += 0.5;
	}
	chin_man.rot.degree += 1;

	run.rot.degree += 10;
	
	plan.rot.degree += 10;
	if (Time_count % 100 == 0) {
		plan.move.degree = 0;

	}
	plan.move.degree += 10;

	plan.move.y = 90;
	plan.move.z = sh[Time_count % Size].pos.y;
	plan.move.x = sh[Time_count % Size].pos.x;

		if (plan.scale.x <= 0.1f)
			plan.scale.x = 1.f;

		plan.scale.x -= 0.1f;

		if (plan.scale.y <= 0.1f) 
			plan.scale.y = 1.f;
		
		plan.scale.y -= 0.1f;
	
		if (plan.scale.z = 0.1f) 
			plan.scale.z = 1.f;
		
		plan.scale.z -= 0.1f;
	
		//---------볼

		if (!ani) {
			move_count += 1;//타이머 카운트
		}
		
		
		ball.rot.degree +=( PI * 2) * ball.stacks;
		if (ball.b.b_x)
		{
				if ((ball.pos.x + ball.move.x  < 100)) {
					ball.move.x += 1;			
				}
			
		}
		else{
			if ((ball.pos.x + ball.move.x  > 0)) {
				ball.move.x -= 1;
			}
		}




		//크레인
		//이동하는 코드
		if ((clain.move.x == 100) || (clain.move.x == -100)) {
			clain.stacks *= -1;
		}
		clain.move.x += (1 * clain.stacks);
		//y 축 회전
		if (clain.any) {
			clain.rot.degree += 2;
			clain.rot.x = 0;
			clain.rot.y = 1;
			clain.rot.z = 0;
		}

		//크레인과 레킹볼 
		if ((clain.move.x - clain.size < ball.move.x + ball.pos.x) && ( ball.move.x + ball.pos.x < clain.move.x + clain.size)) {// z 축
			if((clain.move.z - clain.size < ball.move.z + ball.pos.z) && (ball.move.z + ball.pos.z < clain.move.z + clain.size)) {// z 축
				printf("clain");
				ball.stacks *= -1;
				if (ball.b.b_z) {
					ball.b.b_z = false;
				}
				else {
					ball.b.b_z = true;
				}
			}
		
		}
		

		if ((ball.pos.z + ball.move.z > 100)) {
				ball.b.b_z = true;
				ball.stacks *= -1;
				printf("a");

		}			
		if ((ball.pos.z + ball.move.z < -100)) {
			ball.b.b_z = false;
			ball.stacks *= -1;
			printf("b");
		}

		if (ball.b.b_z)
		{
			ball.move.z -= 2;
		}
		else {
			ball.move.z += 2;
		}

			ball.rot.x = 1;
			ball.cl.R = false;
		

			if (index.b.b_y) {
				if (index.move.y < 9) {
	
					index.move.y += 1;
				}
			}
			else {
				if (index.move.y > 0) {

					index.move.y -= 1;
				}


			}

			if (man_man.rot.degree == 30 || man_man.rot.degree == -30) {
				man_man.rot.x *= -1;
			}
			man_man.rot.degree += 5 * man_man.rot.x;
		//키 입력 안받기


			
			if (man_man.b.b_y) {
				if (man_man.move.y == 30)
				{
					man_man.b.b_y = false;
				}
				man_man.move.y += 1;
			}
			else {
				if (man_man.move.y > 20) {
					man_man.move.y -= 1;
				}
			}

	Time_count++;
	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}
int ttt;
int ani_count;

void SpecialKeys(int key, int x, int y) {
	if (key == GLUT_KEY_DOWN) {
		
			if (((70 < man_man.move.z) && (-65 > man_man.move.x)) || ((72 < man_man.move.z) && (28 < man_man.move.x))) {
				man_man.b.b_z = false;
				man_man.b.b_y = true;
			}
			else {
			man_man.b.b_z = true;
			}
		
		if (man_man.b.b_z) {
			if (BOX_SIZE / 2  > man_man.move.z + 10) {
				printf("%f , %f \n", man_man.move.x, man_man.move.z);
				man_man.move.z += 1;
			}
		}
		man_man.move.degree = 0;
	}

	if (key == GLUT_KEY_UP) {
	
		if (((-60 > man_man.move.z) && (-60 > man_man.move.x)) || ((-50 > man_man.move.z) && (60 < man_man.move.x))) {
			man_man.b.b_z = false;
			man_man.b.b_y = true;
		}
		else {
			man_man.b.b_z = true;
		}

		if (man_man.b.b_z) {
		if (-BOX_SIZE / 2  < man_man.move.z - 10) {
			printf("%f , %f \n", man_man.move.x, man_man.move.z);
			man_man.move.z -= 1;
		}
		}
		man_man.move.degree = 0;
	}
	if (key == GLUT_KEY_LEFT) {
		
		if (((-60 > man_man.move.z) && (-60 > man_man.move.x)) || ((70 < man_man.move.z) && (-65 > man_man.move.x))) {
			man_man.b.b_x = false;
			man_man.b.b_y = true;
		}
		else {
			man_man.b.b_x = true;
		}

		if (man_man.b.b_x) {
			if (-BOX_SIZE / 2 < man_man.move.x - 10) {
				printf("%f , %f \n", man_man.move.x, man_man.move.z);
				man_man.move.x -= 1;
			}
			man_man.move.degree = 90;
		}
	}
	
	if (key == GLUT_KEY_RIGHT) {
		
		if (((-67 > man_man.move.z) && (60 < man_man.move.x)) || ((28 < man_man.move.z) && (72 < man_man.move.x))) {
			man_man.b.b_x = false;
			man_man.b.b_y = true;
		}
		else {
			man_man.b.b_x = true;
		}

		if (man_man.b.b_x) {
			if (BOX_SIZE / 2  > man_man.move.x + 10) {
				printf("%f , %f \n", man_man.move.x, man_man.move.z);
				man_man.move.x += 1;
			}
			man_man.move.degree = 90;
		}
	}
}

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

	//비행기 방향 바꾸기
	case '/':
		plan.pos.degree *= -1;
		break;


	//래킹볼
	case 'l':
		ani = TRUE;
		ttt++;
		if (ttt % 2 == 0) {
			ball.any = TRUE;
			ball.move.x = 0;
			ball.b.b_x = TRUE;
			ball.b.b_y = FALSE;
			ball.b.b_z = FALSE;

			//	shape.rot.x = 0;
			//	shape.rot.y = 0;
			ball.rot.z = 1;
			ball.cl.R = 0;
		}
		else {
			ball.pos.x += ball.move.x;

			ball.b.b_x = FALSE;
			ball.b.b_y = FALSE;
			ball.b.b_z = FALSE;
			//	shape.rot.x = 0;
			//	shape.rot.y = 0;
			//	shape.rot.z = 0;
			ball.any = FALSE;


		}
		break;

	case 'L':
		ani = TRUE;
		ttt++;
		if (ttt % 2 == 0) {
			ball.any = TRUE;
			ball.move.x = 0;
			ball.b.b_x = TRUE;
			ball.b.b_y = FALSE;
			ball.b.b_z = FALSE;

			//	shape.rot.x = 0;
			//	shape.rot.y = 0;
			ball.rot.z = 1;
			ball.cl.R = 1;
		}
		else {
			ball.pos.x += ball.move.x;

			ball.b.b_x = FALSE;
			ball.b.b_y = FALSE;
			ball.b.b_z = FALSE;
			//	shape.rot.x = 0;
			//	shape.rot.y = 0;
			//	shape.rot.z = 0;
			ball.any = FALSE;
		}
		break;

	case 'm':
		ani = TRUE;
		ttt++;
		if (ttt % 2 == 0) {
			ball.any = TRUE;
			ball.move.y = 0;
			ball.b.b_x = FALSE;
			ball.b.b_y = TRUE;
			ball.b.b_z = FALSE;

			//	shape.rot.x = 0;
			ball.rot.y = 1;
			//	shape.rot.z = 0;
			ball.cl.G = 0;
		}
		else {
			ball.pos.y += ball.move.y;
			ball.b.b_x = FALSE;
			ball.b.b_y = FALSE;
			ball.b.b_z = FALSE;
			//	shape.rot.x = 0;
			//	shape.rot.y = 0;
			//	shape.rot.z = 0;
			ball.any = FALSE;
		}
		break;
	case 'M':
		ani = TRUE;
		ttt++;
		if (ttt % 2 == 0) {
			ball.any = TRUE;
			ball.move.y = 0;
			ball.b.b_x = FALSE;
			ball.b.b_y = TRUE;
			ball.b.b_z = FALSE;

			//	shape.rot.x = 0;
			ball.rot.y = 1;
			//	shape.rot.z = 0;
			ball.cl.G = 1;
		}
		else {
			ball.pos.y += ball.move.y;
			ball.b.b_x = FALSE;
			ball.b.b_y = FALSE;
			ball.b.b_z = FALSE;
			//	shape.rot.x = 0;
			//	shape.rot.y = 0;
			//	shape.rot.z = 0;
			ball.any = FALSE;
		}
		break;
	case 'n':
		ani = TRUE;
		ttt++;
		if (ttt % 2 == 0) {
			ball.any = TRUE;
			ball.move.z = 0;
			ball.b.b_x = FALSE;
			ball.b.b_y = FALSE;
			ball.b.b_z = TRUE;

			ball.rot.x = 1;
			//	shape.rot.y = 0;
			//	shape.rot.z = 0;
		}
		else {
			ball.pos.z += ball.move.z;
			ball.b.b_x = FALSE;
			ball.b.b_y = FALSE;
			ball.b.b_z = FALSE;
			//	shape.rot.x = 0;
			//	shape.rot.y = 0;
			//	shape.rot.z = 0;
			ball.any = FALSE;
		}
		break;

	case 'N':
		ani = TRUE;
		ttt++;
		if (ttt % 2 == 0) {
			ball.any = TRUE;
			ball.move.z = 0;
			ball.b.b_x = FALSE;
			ball.b.b_y = FALSE;
			ball.b.b_z = TRUE;

			ball.rot.x = 1;
			//	shape.rot.y = 0;
			//	shape.rot.z = 0;
		}
		else {
			ball.pos.z += ball.move.z;
			ball.b.b_x = FALSE;
			ball.b.b_y = FALSE;
			ball.b.b_z = FALSE;
			//	shape.rot.x = 0;
			//	shape.rot.y = 0;
			//	shape.rot.z = 0;
			ball.any = FALSE;
		}
		break;

	case '4'://컬링
		culling_count++;
		if (culling_count % 2 == 1) {
			culling = true;
			printf("컬링 On \n");
		}
		else {
			printf("컬링 Off \n");
			culling = false;
		}
		break;

	//크레인
	case '7':
		ttt++;

		break;
		//y
	case '8':
		ani = TRUE;
		ttt++;
		if (ttt % 2 == 0) {
			clain.any = TRUE;
		}
		else {
			clain.any = FALSE;
		}
		break;
		//----------첫번째 서브 x 축 이동
	case 'f':
		sub[0].any = TRUE;
		sub[0].rot.degree--;
		sub[0].rot.x = 1;
		//sub[0].rot.y = 0;
		//sub[0].rot.z = 0;

		break;

	case 'F':
		sub[0].any = TRUE;
		sub[0].rot.degree++;
		break;
		//----------첫번째 서브 y 축 이동
	case 'g':
		sub[0].any = TRUE;
		sub[0].rot.z--;
		sub[0].rot.y = 1;
		//sub[0].rot.y = 0;
		//sub[0].rot.z = 0;
		break;

	case 'G':
		sub[0].any = TRUE;
		sub[0].rot.z++;
		break;

		//----------두번째 서브 x 축 이동
	case 'c':
		sub[1].any = TRUE;
		sub[1].rot.degree--;
		sub[1].rot.x = 1;
		//sub[0].rot.y = 0;
		//sub[0].rot.z = 0;

		break;

	case 'C':
		sub[1].any = TRUE;
		sub[1].rot.degree++;
		break;
		//----------두번째 서브 z 축 이동
	case 'v':
		sub[1].any = TRUE;
		sub[1].rot.y--;
		sub[1].rot.z = 1;
		//sub[0].rot.y = 0;
		//sub[0].rot.z = 0;

		break;

	case 'V':
		sub[1].any = TRUE;
		sub[1].rot.y++;
		break;


		//앞면 닫기
	case 'j':
		index.b.b_y = true;
		break;
	case 'J':
		ball.b.b_y = false;
		break;

	case ' ':
		man_man.b.b_y = true;
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