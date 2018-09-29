#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>
GLvoid Reshape(int w, int h);

//�ػ� ����
#define WideSize 800
#define HighSize 600
#define Z_Size 400

//�׸��� ����
GLvoid drawScene(GLvoid);
BOOL draw_Act_On;

//Ÿ�̸� ����
void Timerfunction(int value);
BOOL Time_Act_On; //Ÿ�̸� Ȱ��ȭ ����
int Time_count; //Ÿ�̸� ī��Ʈ

				//���콺 ����
void Mouse(int button, int state, int x, int y);
POINT ms_click; //���콺 ��ǥ
BOOL Mouse_Act_On; //���콺 Ȱ��ȭ ����

				   //Ű���� ����
void Keyboard(unsigned char key, int x, int y);
BOOL Keyboard_Act_On; //Ű���� Ȱ��ȭ ����

					  //�� ����
typedef struct Color
{
	int R;
	int G;
	int B;
};
typedef struct Translate_pos {
	int x;
	int y;
	int z;
	int degree;
};

#define PT 10//���� ���� ����
#define PI 3.141592 //����
#define CubeSize 30
typedef struct Shape
{
	Color cl;//����
	Translate_pos pos;
	Translate_pos move;
	Translate_pos scale;
	Translate_pos rot;

	int size;
	int select;
	int height;
	int slice;
	int stacks;
	BOOL Look = FALSE;


};
//Ÿ���о����
int hour;
int min;
int sec;

int click_count;
int save_count;
BOOL Save = false;
BOOL ani = FALSE;

Shape tra;
Shape pivot[7];
Shape move[5];
Shape clicker[4];
void Time_Input(int value);


void main(int argc, char *argv[]) {

	
	

	pivot[0].pos.x = -70;
	pivot[0].pos.y = 0;

	pivot[2].pos.x = 70;
	pivot[2].pos.y = 0;
	
	pivot[4].pos.x = -70;
	pivot[4].pos.y = 140;
	
	pivot[6].pos.x = 70;
	pivot[6].pos.y = 140;

	pivot[1].pos.x = 0;
	pivot[1].pos.y = -70;
	
	pivot[3].pos.x = 0;
	pivot[3].pos.y = 70;
	
	pivot[5].pos.x = 0;
	pivot[5].pos.y = 210;

	for (int i = 0; i < 6; i++) {
		
		if (i < 4) {
			clicker[i].rot.x = 80;
			clicker[i].rot.y = 100;
		}
		
		
		move[i].move.x = 180 + 190 * i;
		move[i].move.y = HighSize / 4 * 3;
		
		if (i == 2 || i == 3) {
			move[i].move.x += 100;
		}
		else if (i == 4 || i == 5) {
			move[i].move.x += 200;
		}
	}
	//clicker[0].rot.x = WideSize/2;
	//clicker[0].rot.y = HighSize/2;
	clicker[0].rot.x = move[1].move.x - 100;
	clicker[1].rot.x = move[1].move.x - 100;

	clicker[0].rot.y = move[1].move.y - 200;
	clicker[1].rot.y = move[1].move.y - 200 - pivot[1].pos.y;

	clicker[2].rot.x = move[2].move.x - 150;
	clicker[3].rot.x = move[2].move.x - 150;

	clicker[2].rot.y = move[1].move.y - 200;
	clicker[3].rot.y = move[1].move.y - 200 - pivot[1].pos.y;



	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);//������ ���� ��ǥ
	glutInitWindowSize(WideSize, HighSize); //������ ���� ũ��
	glutCreateWindow("ex15");
	// - �������� ���� ���� �����ϱ�
	//���� �׸���

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

	time_t now = time(0);
	struct tm curr_time;

	localtime_s(&curr_time, &now);

	hour = curr_time.tm_hour;
	min = curr_time.tm_min;
	sec = curr_time.tm_sec;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(2);


	glMatrixMode(GL_MODELVIEW);
	//��� ����
	//��ǥ�� �׸���
	glPushMatrix();
	glTranslated(WideSize / 2, HighSize / 2, 0);
	glRotatef(tra.rot.degree, tra.rot.x, tra.rot.y, tra.rot.z);
	glTranslated(-WideSize / 2, -HighSize / 2, 0);

	glPushMatrix(); //���� ���� ����
	glScalef(0.5f, 0.5f, 1.0f);
	
	for(int j=0; j<6; j++)
	{	
		if (j == 0) {
			Time_Input(hour / 10);
		}
		else if (j == 1) {
			Time_Input(hour % 10);
		}
		else if (j == 2) {
			Time_Input(min / 10);
		}
		else if (j == 3) {
			Time_Input(min % 10);
		}
		else if (j == 4) {
			Time_Input(sec / 10);
		}
		else if (j == 5) {
			Time_Input(sec % 10);
		}


		for (int i = 0; i < 7; i++) {

		glPushMatrix(); //���� ���� ����
		glTranslated(
			move[j].move.x + pivot[i].pos.x,
			move[j].move.y + pivot[i].pos.y , 0);
	
		if (i % 2 == 1) {
			glScalef(2.0f, 0.5f, 1.0f);
		}
		else {
			glScalef(0.5f, 2.0f, 1.0f);
		}
		if (pivot[i].Look) {//�ð��� ���� ���̱�
			if (ani) {
				glColor3f((float)100 / 255, (float)100 / 255, (float)100 / 255);
				glutSolidCube(50);
				glColor3f((float)10 / 255, (float)10 / 255, (float)70 / 255);
				glutWireCube(52);
			}
			else {
				glColor3f((float)200 / 255, (float)200 / 255, (float)100 / 255);
				glutWireCube(50);
			}

		}
		glPopMatrix(); //���� ���� �ݱ�  

	}
	}
	
	glPopMatrix(); //���� ���� �ݱ�
	glPopMatrix(); //���� ���� �ݱ�
	
	//==================================================================
	glPushMatrix();
	glTranslated(WideSize / 2, HighSize / 2, 0);
	glRotatef(tra.rot.degree, tra.rot.x, tra.rot.y, tra.rot.z);
	glTranslated(-WideSize / 2, -HighSize / 2, 0);

	for (int i = 0; i < 4; i++) {
		glPushMatrix();
		glTranslated(clicker[i].rot.x, clicker[i].rot.y, 0);
		if (sec % 2 == 0) {
			if (ani) {
				glutSolidCube(20);
			}
			else {
				glutWireCube(20);
			}
			}
		glPopMatrix();
	}
	glPopMatrix(); //���� ���� �ݱ�


glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		

	}
}
void Time_Input(int value) {
	if (value % 10 == 0) {
		pivot[1].Look = TRUE;
		pivot[0].Look = TRUE;
		pivot[2].Look = TRUE;
		pivot[3].Look = FALSE;
		pivot[4].Look = TRUE;
		pivot[6].Look = TRUE;
		pivot[5].Look = TRUE;
	}
	else if (value % 10 == 1) {
		pivot[1].Look = FALSE;
		pivot[0].Look = FALSE;
		pivot[2].Look = TRUE;
		pivot[3].Look = FALSE;
		pivot[4].Look = FALSE;
		pivot[6].Look = TRUE;
		pivot[5].Look = FALSE;
	}
	else if (value % 10 == 2) {
		pivot[0].Look = FALSE;
		pivot[1].Look = TRUE;
		pivot[2].Look = TRUE;
		pivot[3].Look = TRUE;
		pivot[4].Look = TRUE;
		pivot[5].Look = TRUE;
		pivot[6].Look = FALSE;
	}
	else if (value % 10 == 3) {
		pivot[0].Look = FALSE;
		pivot[1].Look = TRUE;
		pivot[2].Look = TRUE;
		pivot[3].Look = TRUE;
		pivot[4].Look = FALSE;
		pivot[5].Look = TRUE;
		pivot[6].Look = TRUE;
	}
	else if (value % 10 == 4) {
		pivot[0].Look = TRUE;
		pivot[1].Look = FALSE;
		pivot[2].Look = TRUE;
		pivot[3].Look = TRUE;
		pivot[4].Look = FALSE;
		pivot[5].Look = FALSE;
		pivot[6].Look = TRUE;
	}
	else if (value % 10 == 5) {
		pivot[0].Look = TRUE;
		pivot[1].Look = TRUE;
		pivot[2].Look = FALSE;
		pivot[3].Look = TRUE;
		pivot[4].Look = FALSE;
		pivot[5].Look = TRUE;
		pivot[6].Look = TRUE;
	}
	else if (value % 10 == 6) {
		pivot[0].Look = TRUE;
		pivot[1].Look = TRUE;
		pivot[2].Look = FALSE;
		pivot[3].Look = TRUE;
		pivot[4].Look = TRUE;
		pivot[5].Look = TRUE;
		pivot[6].Look = TRUE;
	}
	else if (value % 10 == 7) {
		pivot[0].Look = TRUE;
		pivot[1].Look = TRUE;
		pivot[2].Look = TRUE;
		pivot[3].Look = FALSE;
		pivot[4].Look = FALSE;
		pivot[5].Look = FALSE;
		pivot[6].Look = TRUE;
	}
	else if (value % 10 == 8) {
		pivot[0].Look = TRUE;
		pivot[1].Look = TRUE;
		pivot[2].Look = TRUE;
		pivot[3].Look = TRUE;
		pivot[4].Look = TRUE;
		pivot[5].Look = TRUE;
		pivot[6].Look = TRUE;
	}
	else if (value % 10 == 9) {
		pivot[0].Look = TRUE;
		pivot[1].Look = TRUE;
		pivot[2].Look = TRUE;
		pivot[3].Look = TRUE;
		pivot[4].Look = FALSE;
		pivot[5].Look = TRUE;
		pivot[6].Look = TRUE;
	}
	
}

void Timerfunction(int value) {

	//Ÿ�̸� ���� �Է�
	Time_count ++;
	//Ÿ�̸� �밡�� ����
	
		
	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.
	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'x':
		tra.rot.x = 1;
		//tra.rot.y = 0;
		tra.rot.degree -= 1;
		break;

	case 'X':
		tra.rot.x = 1;
		//tra.rot.y = 0;
		tra.rot.degree += 1;
		break;
	case 'y':
		tra.rot.y = 1;
		tra.rot.degree -= 1;
		break;

	case 'Y':
		tra.rot.y = 1;
		tra.rot.degree += 1;
		break;
	case 'b' ://

		click_count++;
		if (click_count % 2 == 1) {
		
			ani = FALSE;
		}
		else {
			ani = TRUE;
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
	glOrtho(0, WideSize, HighSize, 0, -Z_Size, Z_Size); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!

}