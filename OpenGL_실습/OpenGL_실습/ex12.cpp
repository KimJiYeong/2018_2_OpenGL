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

#define PT 100//도형 갯수 설정
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
int save_count;
int st_help;//직선 그리기 도우미

int zig_zag;//지그재그 그리기 도우미
BOOL ZIG;//지그재그

BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;

Shape tra;
Shape sh[PT];
Shape small;
typedef struct Po
{
	float x;
	float y;
};
Po save;

void main(int argc, char *argv[]) {
	
	tra.size = 40;

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
	glPointSize(4);
	glLineWidth(2);
	glColor3f((float)255 / 255, (float)255 / 255, (float)255 / 255);


	glMatrixMode(GL_MODELVIEW);
	//출력 설정
	glColor3f((float)100 / 255, (float)200 / 255, (float)100 / 255);
	//좌표축 그리기

	glPushMatrix(); //상태 저장 열기
	if (!Mouse_Act_On) {
		glTranslated(WideSize / 2, HighSize / 2, 0); //도형 그리기 할때는 초기화 시켜준다.
	}
	glRotatef(tra.rot.degree, 0, 1, 0);

	for (int i = 0; i < PT - 1; i++) {
		glBegin(GL_LINES);
		glVertex3d(sh[i].pos.x, sh[i].pos.y, sh[i].pos.z);
		glVertex3d(sh[i + 1].pos.x, sh[i + 1].pos.y, sh[i + 1].pos.z);
		glEnd();
	}

	//작은 삼각형------------------
	glPushMatrix();
	glTranslated(sh[move_count].pos.x, sh[move_count].pos.y, sh[move_count].pos.z);
	glRotated(small.rot.degree, 0, 1,0);
	if (small.select) {
		glBegin(GL_TRIANGLES);
		glVertex3d(0, -small.size / 2, 0);
		glVertex3d(small.size / 2, small.size / 2, 0);
		glVertex3d(-small.size / 2, small.size / 2, 0);
		glEnd();

	}
	else {
		glBegin(GL_POLYGON);
		glVertex3d(small.size / 2, -small.size / 2, 0);
		glVertex3d(small.size / 2, small.size / 2, 0);
		glVertex3d(-small.size / 2, small.size / 2, 0);
		glVertex3d(-small.size / 2, -small.size / 2, 0);

		glEnd();

	}
	
	
	glPopMatrix();


	glPopMatrix(); //상태 저장 닫기
	
	
	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	
	if (Mouse_Act_On) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if (click_count < 5) {
				sh[click_count].sa.x = x;
				sh[click_count].sa.y = y;
			
			}
			click_count++;
			save_count++;
		}
		else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {//우클릭시 그리기 시작
			Save = TRUE;
			click_count = 0;
			for (int i = 0; i < PT; i++) {
				if (i % (PT / (save_count  - 1)) == 0) {

					sh[i].pos.x = sh[click_count].sa.x;
					sh[i].pos.y = sh[click_count].sa.y;
					
					save.x = (float)((sh[click_count + 1].sa.x - sh[click_count].sa.x) / (PT / (save_count - 1)));
					save.y = (float)((sh[click_count + 1].sa.y - sh[click_count].sa.y) / (PT / (save_count - 1)));
					
					if (click_count != save_count - 2) {
						click_count++;
					}
				}
				else {
					sh[i].pos.x = save.x + sh[i - 1].pos.x;
					sh[i].pos.y = save.y + sh[i - 1].pos.y;
					//sh[i].pos.x += save.x;
					//sh[i].pos.y += save.y;
				}
			}
		}
	}
	
}
void Timerfunction(int value) {

	//타이머 내용 입력
	if (move_count == PT) {
		move_count = 0;
	}
	move_count++;
	
	if (ani) {
		if (small.size >= 40) {
			ani = FALSE;
		}

		small.size++;
	}
	else {
		if (small.size <= 20) {
			ani = TRUE;
		}
		small.size--;
	}

	if (Look) {
		small.rot.degree += 3.6;
	}
	else {
		small.rot.degree = 0;
	}

	glutPostRedisplay(); //타이머에 넣는다.
	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case '0'://원
		Mouse_Act_On = FALSE;
		Look = TRUE;//회전하는거 트루로
		for (int i = 0; i < PT; i++) {
			sh[i].pos.x = 100 * cos(PI * i * 10 / 180);
			sh[i].pos.y = 100 * sin(PI * i * 10 / 180);
		}

		break;

	case '1'://사인
		Mouse_Act_On = FALSE;
		Look = FALSE;
		for (int i = 0; i < PT; i++) {
			sh[i].pos.x = i * 10 - WideSize/2;
			sh[i].pos.y = 100 * sin(PI * i * 10 / 90);
		}
		break;
	case '2'://회오리
		Mouse_Act_On = FALSE;
		for (int i = 0; i < PT; i++) {
			sh[i].pos.x = i * 10 + 100 * cos(PI * i * 10 / 180) - WideSize / 2;
			sh[i].pos.y = 100 * sin(PI * i * 10 / 180);
		}
		break;
	case '3'://지그재그
		st_help = 0;
		zig_zag = 1;
		Save = FALSE;
		Look = FALSE;
		Mouse_Act_On = FALSE;
		for (int i = 0; i < PT; i++) {
			
			if (i % 20 == 0) {
				zig_zag *= (-1);
				//지그재그로 교차되는 선을 만든다.
			}
			
			if (ZIG) {
				st_help--;
			}
			else {
				st_help++;
			}

			sh[i].pos.x = i * 10 - WideSize / 2;
			sh[i].pos.y = zig_zag * (st_help * 5);
			
			if (st_help == 10) {
				ZIG = TRUE;
			}
			else if(st_help == -10){
				ZIG = FALSE;
			}
		}
		break;
	case '4'://경로 그리기
		st_help = 0;//직선 그리기 도우미
		Mouse_Act_On = TRUE;//우클릭을 누르지 않으면 하지 않는다.
		Look = FALSE;
		click_count = 0;
		save_count = 0;
		break;
	case 'y'://y축 방향으로 회전
		tra.rot.degree += 1;

		break;
	case 'Y'://y축 방향으로 회전
		tra.rot.degree -= 1;

		break;
	case 'r'://사각형
		small.select = FALSE;
		break;
	case 't'://삼각형
		small.select = TRUE;
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