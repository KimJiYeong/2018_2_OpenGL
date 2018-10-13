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
int move_count; //Ÿ�̸� ī��Ʈ

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

#define PT 100//�� ���� ����
#define PI 3.141592 //����
typedef struct Shape
{
	Color cl;//����
	int x;
	int y;
	int z;
};
Shape sp1[PT];//����
int st_help; // �����׸��� �����

POINT move;
POINT scale;
POINT rot;
POINT dm;
BOOL ani = FALSE;
BOOL Look = FALSE;

void main(int argc, char *argv[]) {
	//�ʱ�ȭ
	scale.x = 1;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);//������ ���� ��ǥ
	glutInitWindowSize(WideSize, HighSize); //������ ���� ũ��
	glutCreateWindow("ex11");
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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(2);
	glColor3f((float)100 / 255, (float)200 / 255, (float)100 / 255);
	glBegin(GL_LINES);
	glVertex2i(0, HighSize / 2);
	glVertex2i(WideSize, HighSize / 2);
	glEnd();

	glBegin(GL_LINES);
	glVertex2i(WideSize / 2, 0);
	glVertex2i(WideSize / 2, HighSize);
	glEnd();
	glMatrixMode(GL_MODELVIEW);
	//��� ����
	//��ǥ�� �׸���

	glPushMatrix(); //���� ���� ����
	//����
	glTranslated(move.x, move.y, 0);
	glTranslatef(400, 300, 0);
	glScaled(scale.x, scale.x, 1);
	glTranslatef(-400, -300, 0);

	//glScaled(scale.x,0,0);
	glPointSize(2);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < PT; i++) {
		glVertex3i(sp1[i].x , sp1[i].y, sp1[i].z);//���� �׸���
	}
	glEnd();
	glPopMatrix(); //���� ���� �ݱ�
//	glPopMatrix();

	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	}

}
void Timerfunction(int value) {

	//Ÿ�̸� ���� �Է�
	
	if (ani) {
		move_count++;
		if (!Look) {
			move.x -= 1;
		}
		else {
			move.x += 1;
		}
	}

	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.
	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case '1'://���ΰ

		//�̵� �ʱ�ȭ
		move.x = 0;
		move.y = 0;

		for (int i = 0; i < PT; i++) {
			sp1[i].x = i * WideSize / 50 + move.x;
			sp1[i].y = 100 * sin(PI * i * 10 / 90) + HighSize / 2;
			sp1[i].z = 100;
		}
		break;

	case '2'://������
			 //�̵� �ʱ�ȭ
		move.x = 0;
		move.y = 0;
		for (int i = 0; i < PT; i++) {
			sp1[i].x = 100 * cos(PI * i * 10 / 90) + WideSize / 50 + i * 10 + move.x;
			sp1[i].y = 100 * sin(PI * i * 10 / 90) + HighSize / 2;
			sp1[i].z = 100;
		}
		break;
	case '3'://����
		
		//�̵� �ʱ�ȭ
		move.x = 0;
		move.y = 0;
		//���� ����
		st_help = 0;
		for (int i = 0; i < PT/6 * 1; i++) {
			sp1[i].x = -st_help * 6+ WideSize/2 + move.x;
			sp1[i].y = -st_help * 6+ HighSize/2;
			sp1[i].z = 100;
			st_help++;
		}
		st_help = 0;
		for (int i = PT / 6 * 1; i < PT / 6 * 2; i++) {
			sp1[i].x = sp1[PT / 6 * 1 - 1].x + move.x;
			sp1[i].y = st_help * 12 + sp1[PT / 6 * 1 - 1].y;
			sp1[i].z = 100;
			st_help++;
		}
		st_help = 0;
		for (int i = PT / 6 * 2; i < PT / 6 * 3; i++) {
			sp1[i].x = -st_help * 6 + WideSize / 2 + move.x;
			sp1[i].y = st_help * 6 + HighSize / 2;
			sp1[i].z = 100;
			st_help++;
		}
				
		//������
		st_help = 0;
		for (int i = PT/6 * 3; i < PT / 6 * 4; i++) {
			sp1[i].x = st_help * 6 + WideSize / 2 + move.x;
			sp1[i].y = -st_help * 6 + HighSize / 2;
			sp1[i].z = 100;
			st_help++;
		}
		st_help = 0;
		for (int i = PT / 6 * 4; i < PT / 6 * 5; i++) {
			sp1[i].x = sp1[PT / 6 * 4 - 1].x + move.x;
			sp1[i].y = st_help * 12 + sp1[PT / 6 * 4 - 1].y;
			sp1[i].z = 100;
			st_help++;
		}
		st_help = 0;
		for (int i = PT / 6 * 5; i < PT / 6 * 6; i++) {
			sp1[i].x = st_help * 6 + WideSize / 2 + move.x;
			sp1[i].y = st_help * 6 + HighSize / 2;
			sp1[i].z = 100;
			st_help++;
		}


		break;
	case '4'://�簢��
			 //�̵� �ʱ�ȭ
		move.x = 0;
		move.y = 0;
		st_help = 0;
		for (int i = PT / 4 * 0; i < PT / 4 * 1; i++) {
			sp1[i].x = WideSize / 5 * 2 +move.x;
			sp1[i].y = st_help * HighSize /100 + HighSize / 5 * 2;
			sp1[i].z = 100;
			st_help++;
		}
		st_help = 0;
		for (int i = PT / 4 * 1; i < PT / 4 * 2; i++) {
			sp1[i].x = st_help * WideSize / 100 + sp1[PT / 4 * 1 - 1].x + move.x;
			sp1[i].y = sp1[PT / 4 * 1 - 1].y;
			sp1[i].z = 100;
			st_help++;
		}
		st_help = 0;
		for (int i = PT / 4 * 2; i < PT / 4 * 3; i++) {
			sp1[i].x = sp1[PT / 4 * 2 - 1].x + move.x;
			sp1[i].y = sp1[PT / 4 * 2 - 1].y - st_help * HighSize / 100;
			sp1[i].z = 100;
			st_help++;
		}
		st_help = 0;
		for (int i = PT / 4 * 3; i < PT / 4 * 4; i++) {
			sp1[i].x = sp1[PT / 4 * 3 - 1].x - st_help * WideSize / 100 + move.x;
			sp1[i].y = sp1[PT / 4 * 3 - 1].y ;
			sp1[i].z = 100;
			st_help++;
		}
		break;
		//��ɾ� ����
	case 'x'://x �� ���� -
		move.x--;
		break;
	case 'X':
		move.x++;
		break;
	case 'y'://x �� ���� -
		move.y--;
		break;
	case 'Y':
		move.y++;
		break;
	case 's':// -
		scale.x-= 1;
		break;
	case 'S':
		scale.x+= 1;
		break;
	case 'r'://-
		move.x -= 10;
		break;
	case 'R':
		move.x += 10;
		break;
	case 'a'://x �� ���� -
		move_count = 0; 
		ani = TRUE;
		Look = FALSE;
		break;
	case 'A':
		move_count = 0;
		ani = TRUE;
		Look = TRUE;
		break;
	case 't':
		ani = FALSE;
		break;
	case 'q' || 'Q':
		PostQuitMessage(0);
		break;
	default:
		;
		break;
	}
}

GLvoid Reshape(int w, int h)
{
	glViewport(-1, -1, w, h);
	glOrtho(0, WideSize, HighSize, 0, -Z_Size / 2, Z_Size / 2); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!

}