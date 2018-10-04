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
	float x;
	float y;
	float z;
	float degree;
};

#define PT 100//���� ���� ����
#define PI 3.141592 //����
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
};

int click_count;
int save_count;
int st_help;
BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;
Shape sp1[PT];
Shape small[2];
Shape tra;

void main(int argc, char *argv[]) {
	//�ʱ�ȭ
	st_help = 0;
	for (int i = PT / 3 * 0; i < PT / 3 * 1; i++) {
		sp1[i].pos.x = st_help * 5;
		sp1[i].pos.y = st_help * 10 - 150;
		st_help++;
	}
	st_help = 0;
	for (int i = PT / 3 * 1; i < PT / 3 * 2; i++) {
		sp1[i].pos.x = -st_help * 10 + sp1[PT / 3 * 1 - 1].pos.x;
		sp1[i].pos.y = sp1[PT / 3 * 1 - 1].pos.y;
		st_help++;
	}
	st_help = 0;
	for (int i = PT / 3 * 2; i < PT / 3 * 3; i++) {
		sp1[i].pos.x = st_help * 5 + sp1[PT / 3 * 2 - 1].pos.x;
		sp1[i].pos.y = -st_help * 10 + sp1[PT / 3 * 2 - 1].pos.y;
		st_help++;
	}
	//���� �ﰢ��
	for (int i = 0; i < 2; i++) {
		small[i].size = 50;
	}
	tra.rot.x = 1;
	tra.rot.y = 1;
	tra.rot.z = 1;

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
	glColor3f((float)255 / 255, (float)255 / 255, (float)255 / 255);
	/*glBegin(GL_LINES);
	glVertex2i(0, HighSize / 2);
	glVertex2i(WideSize, HighSize / 2);
	glEnd();

	glBegin(GL_LINES);
	glVertex2i(WideSize / 2, 0);
	glVertex2i(WideSize / 2, HighSize);
	glEnd();*/
	
	glMatrixMode(GL_MODELVIEW);
	//��� ����
	glColor3f((float)100 / 255, (float)200 / 255, (float)100 / 255);
	//��ǥ�� �׸���

	glPushMatrix(); //���� ���� ����


	glPushMatrix();
	glTranslated(WideSize / 2, HighSize / 2, 0);
	//glPushMatrix();
	glScaled(tra.rot.x, tra.rot.y, tra.rot.z);
	glRotatef(0 + tra.rot.degree, 0, 1, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < PT; i++) {
		glVertex3i(sp1[i].pos.x, sp1[i].pos.y, sp1[i].pos.z);

	}
	glEnd();

	glPushMatrix();
	glRotatef(90 + tra.rot.degree, 0, 1, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < PT; i++) {
		glVertex3i(sp1[i].pos.x, sp1[i].pos.y, sp1[i].pos.z);

	}
	glEnd();
	glPopMatrix();


	//----------���� �ﰢ��


	glPushMatrix(); //���� ���� ����
	//glTranslated(sp1[0].pos.x, sp1[0].pos.y, sp1[0].pos.z);//��ǥ�� ��ġ ����
	glScaled(1, 1, 1);
	glBegin(GL_TRIANGLES);
	glVertex3i(small[0].pos.x, small[0].pos.y - small[0].size / 2, small[0].pos.z);
	glVertex3i(small[0].pos.x + small[0].size / 2, small[0].pos.y + small[0].size / 2, small[0].pos.z);				
	glVertex3i(small[0].pos.x - small[0].size / 2, small[0].pos.y + small[0].size / 2, small[0].pos.z);
	glEnd();
	glPopMatrix();//�������� �ݱ�

	glPushMatrix();
	glRotatef(90 + tra.rot.degree, 0, 1, 0);
	glBegin(GL_TRIANGLES);
	glVertex3i(small[0].pos.x, small[0].pos.y - small[0].size / 2, small[0].pos.z);
	glVertex3i(small[0].pos.x + small[0].size / 2, small[0].pos.y + small[0].size / 2, small[0].pos.z);
	glVertex3i(small[0].pos.x - small[0].size / 2, small[0].pos.y + small[0].size / 2, small[0].pos.z);
	glEnd();

	glPopMatrix();
	glPopMatrix(); //���� ���� �ݱ�



	glPopMatrix(); //���� ���� �ݱ�


	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	}
}
void Timerfunction(int value) {
	Time_count++;
	//Ÿ�̸� ���� �Է�
	
	if (Time_count < PT) {
		if (Time_count == PT - 1) {
			Time_count = 0;
	}
		small[0].pos.x = sp1[Time_count].pos.x;
		small[0].pos.y = sp1[Time_count].pos.y;
		small[0].pos.z = sp1[Time_count].pos.z;

	}


	//�������� ������������ �Ÿ��� 100���� ������
	//���Ѵ� �� �ƴѰ�?
	//�������� ������ ������ �Ÿ��� 100���� ������. -> movex��� �̵������� ���� �����.
	/*
	tra.move.x += (sp1[PT / 3 * 1].pos.x - sp1[PT / 3 * 0].pos.x) / 100;
	tra.move.y += (sp1[PT / 3 * 1].pos.y - sp1[PT / 3 * 0].pos.y) / 100;


	for (int i = 0; i < PT; i++) {

			if (small[i].move.x < (sp1[PT / 3 * 1].pos.x - sp1[PT / 3 * 0].pos.x)) {
				small[i].move.x = tra.move.x;
				small[i].move.y = tra.move.y;
			}
	}
	*/

	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.
	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'y':
		if (tra.rot.degree == 360) {
			tra.rot.degree = 0;
		}
		tra.rot.degree -= 2;

		break;
	case 'Y':
		if (tra.rot.degree == 360) {
			tra.rot.degree = 0;
		}
		tra.rot.degree += 2;
		break;
	case 's':
		if (tra.rot.x > 0) {
			tra.rot.x -= 0.1;
			tra.rot.y -= 0.1;
			tra.rot.z -= 0.1;
		}
		break;
	case 'S':
		if (tra.rot.x < 300) {
			tra.rot.x += 0.1;
			tra.rot.y += 0.1;
			tra.rot.z += 0.1;

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
	//gluPerspective(WideSize, HighSize, Z_Size / 4 * 1, Z_Size / 4 * 3);
}