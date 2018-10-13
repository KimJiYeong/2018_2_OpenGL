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
int TIme_count;

int move_count; //타이머 카운트
int move_count2; //타이머 카운트
int move_count3; //타이머 카운트
int move_count4; //타이머 카운트

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

#define PT 36//도형 갯수 설정
#define PI 3.141592 //파이
typedef struct Shape
{
	Color cl;//색상
	Translate_pos pos;
	Translate_pos move;
	Translate_pos scale;
	Translate_pos rot;
	Translate_pos sa;
	int size;
	BOOL select;
	int height;
	int slice;
	int stacks;

};

int click_count;

BOOL Save = false;
BOOL ani = FALSE;
int ani_count = 0;
BOOL Look = FALSE;
int Look_count;

Shape sh[PT];
Shape sh2[PT];
Shape camera;

//회전 선택 관련 함수
BOOL Sel_Rot;
int old_rot;//초기값
int next_rot;//선택
			 //--------------

typedef struct Po
{
	float x;
	float y;
};

void rot_custom(int init_sel, int sel, int degree, int rot_x, int rot_y, int rot_z, int move_x, int move_y, int move_z) {
	//initialize 초기화
	//x = 0
	//y = 1
	//z = 2
	int init = (PI * 10 / 180);

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
	GLfloat all_rotate[16] = {
		(cos(rot_y * init) * cos(rot_z * init)) ,
		(((-1) * cos(rot_y * init)) * sin(rot_z * init)) ,
		(sin(rot_y * init)),
		0,
		(sin(rot_x * init) * sin(rot_y * init) * cos(rot_z * init) + cos(rot_x * init) * sin(rot_z * init)) ,
		(((-1) * sin(rot_x * init)) * sin(rot_y * init) * cos(rot_z * init) + cos(rot_x * init) * cos(rot_z * init)) ,
		(cos(rot_z * init) * ((-1)* sin(rot_x * init))),
		0
		(((-1)* cos(rot_x * init)) * sin(rot_y * init) * cos(rot_z * init) + sin(rot_x * init) * sin(rot_z * init)) ,
		(cos(rot_x * init) * sin(rot_y * init) * sin(rot_z * init) + sin(rot_x * init) * cos(rot_z * init)),
		(cos(rot_x * init) * cos(rot_y)),
		0,
		0,0,0,1
	};

	if (sel == 1) {
		glMultMatrixf(all_rotate);

	}
};
void main(int argc, char *argv[]) {

	for (int i = 0; i < PT; i++) {
		sh[i].pos.x = 100 * cos(PI * i * 10 / 180);
		sh[i].pos.z = 100 * sin(PI * i * 10 / 180);
		sh[i].pos.y = 1;
	}
	for (int i = 0; i < PT; i++) {
		sh2[i].pos.x = 100 * cos(PI * i * 10 / 180);
		sh2[i].pos.z = 100 * sin(PI * i * 10 / 180);
		sh2[i].pos.y = 1;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPointSize(4);
	glLineWidth(1);

	glMatrixMode(GL_MODELVIEW);
	//출력 설정
	glColor3f((float)100 / 255, (float)200 / 255, (float)100 / 255);
	//좌표축 그리기

	glPushMatrix(); { //상태 저장 열기

		rot_custom(old_rot, next_rot, camera.rot.degree, camera.rot.x, camera.rot.y, camera.rot.z);
		gluLookAt(
			camera.move.x, camera.move.y, camera.move.z,  //위5 eye
			0, 0, 0, //방향 center
			0, 1, 0); //위쪽방향(건들 ㄴㄴ) up
					  //glTranslated(0, 0, 0);


		glPushMatrix();
		if (Look) {//솔리드 낫 솔리드
			glutWireSphere(40, 10, 10);
		}
		else {
			glutSolidSphere(40, 10, 10);
		}
		glPushMatrix(); {
			glScalef(2, 2, 2);
			glColor3f((float)67 / 255, (float)116 / 255, (float)217 / 255);
			for (int i = 0; i < 3; i++) {
				if (i == 0) {
					;
				}
				else if (i == 1) {
					glRotatef(45, 0, 0, 1);
				}
				else if (i == 2) {
					glRotatef(-90, 0, 0, 1);
				}
				for (int i = 0; i < PT - 1; i++) {

					glBegin(GL_LINES);
					glVertex3d(sh[i].pos.x, sh[i].pos.y, sh[i].pos.z);
					glVertex3d(sh[i + 1].pos.x, sh[i + 1].pos.y, sh[i + 1].pos.z);
					glEnd();
				}

				//-------

				glPushMatrix(); {
					if (i == 0) {
						glTranslated(sh[move_count].pos.x, sh[move_count].pos.y, sh[move_count].pos.z);
					}
					else if (i == 1) {
						glTranslated(sh[move_count3].pos.x, sh[move_count3].pos.y, sh[move_count3].pos.z);
					}
					else if (i == 2) {
						glTranslated(sh[move_count4].pos.x, sh[move_count4].pos.y, sh[move_count4].pos.z);

					}
					glPushMatrix();
					if (Look) {//솔리드 낫 솔리드
						glutWireSphere(10, 10, 10);
					}
					else {
						glutSolidSphere(10, 10, 10);
					}
					glScalef(0.5, 0.5, 0.5);

					glColor3f((float)67 / 255, (float)116 / 255, (float)217 / 255);

					for (int i = 0; i < PT - 1; i++) {

						glBegin(GL_LINES);
						glVertex3d(sh2[i].pos.x, sh2[i].pos.y, sh2[i].pos.z);
						glVertex3d(sh2[i + 1].pos.x, sh2[i + 1].pos.y, sh2[i + 1].pos.z);
						glEnd();

					}
					glPopMatrix();

					//------
					glPushMatrix(); {
						glScalef(0.5, 0.5, 0.5);
						glTranslated(sh2[move_count2].pos.x, sh2[move_count2].pos.y, sh2[move_count2].pos.z);

						glColor3f((float)267 / 255, (float)116 / 255, (float)100 / 255);
						glScalef(0.5, 0.5, 0.5);
						if (Look) {//솔리드 낫 솔리드
							glutWireSphere(10, 10, 10);
						}
						else {
							glutSolidSphere(10, 10, 10);
						}


					}glPopMatrix(); //상태 저장 닫기

									//-----


				}glPopMatrix(); //상태 저장 닫기

								//------
				glColor3f((float)67 / 255, (float)116 / 255, (float)217 / 255);

			}


		}glPopMatrix();
		glPopMatrix(); //상태 저장 닫기

	}glPopMatrix(); //상태 저장 닫기



	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {

	if (Mouse_Act_On) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
		}

	}

}
void Timerfunction(int value) {

	//타이머 내용 입력
	TIme_count++;

	move_count = TIme_count % PT;
	move_count2 = (TIme_count * 2) % PT;
	move_count3 = (TIme_count * 3) % PT;
	move_count4 = (TIme_count * 2) % PT;

	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case '1'://솔리드 유무
		Look_count++;
		if (Look_count % 2 == 1) {
			Look = TRUE;//회전하는거 트루로
		}
		else {
			Look = FALSE;//회전하는거 트루로
		}
		break;
	case '2'://직각투영 유무
		ani_count++;
		if (ani_count % 2 == 1) {
			ani = TRUE;//회전하는거 트루로
		}
		else {
			ani = FALSE;//회전하는거 트루로
		}
		break;
		//---------카메라
	case 'x':
		camera.rot.degree -= 1;
		old_rot = next_rot;
		next_rot = 0;
		Sel_Rot = TRUE;
		break;
	case 'X':
		camera.rot.degree -= 1;
		old_rot = next_rot;
		next_rot = 0;
		Sel_Rot = TRUE;
		break;
	case 'y':
		camera.rot.degree += 1;
		old_rot = next_rot;
		next_rot = 1;
		break;
	case 'Y':
		camera.rot.degree += 1;
		old_rot = next_rot;
		next_rot = 1;
		break;
	case 'z':
		camera.rot.degree += 1;
		old_rot = next_rot;
		next_rot = 2;
		break;
	case 'Z':
		camera.rot.degree += 1;
		camera.rot.degree += 1;
		old_rot = next_rot;
		next_rot = 2;
		break;
	case 'w':
		camera.move.y += 2;
		break;
	case 'a':
		camera.move.x -= 2;
		break;
	case 's':
		camera.move.y -= 2;
		break;
	case 'd':
		camera.move.x += 2;
		break;
	case '+':
		camera.move.z += 2;
		break;
	case '-':
		camera.move.z -= 2;
		break;
	case 'i':
		camera.rot.degree = 0;
		camera.move.x = 0;
		camera.move.y = 0;
		camera.move.z = 0;
		break;

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
	glViewport(-1, -1, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (ani) {
		gluPerspective(60.0f, w / h, 1.0, 1000.0);
		glTranslatef(0.0, 0.0, -300.0);     // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
	}
	else {
		glOrtho(0, WideSize, HighSize, 0, -Z_Size / 2, Z_Size / 2); //윈도우를 초기화 하는 함수입니다!
		glTranslated(WideSize / 2, HighSize / 2, 0);
		glRotatef(30, 1, 0, 0);

	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}