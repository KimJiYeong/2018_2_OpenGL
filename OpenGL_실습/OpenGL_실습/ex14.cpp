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

#define PT 20//���� ���� ����
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
Shape sp[PT];
Shape small[2];
Shape tra;

void main(int argc, char *argv[]) {
	//�ʱ�ȭ

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
	glMatrixMode(GL_MODELVIEW);
	//��ǥ�� �׸���
	
	for (int i = 0; i < save_count; i++) {
	
		//��� ����
		glColor3f((float)sp[i].cl.R / 255, (float)sp[i].cl.G / 255, (float)sp[i].cl.B / 255);

		glPushMatrix(); //���� ���� ����	
		
		glTranslated(sp[i].pos.x, sp[i].pos.y, 0);//�Է��� ���� �Էµȴ�.
		glRotatef(sp[i].rot.degree, sp[i].rot.x, sp[i].rot.y, sp[i].rot.z);
		glScaled(sp[i].scale.x, sp[i].scale.x, sp[i].scale.x);
		
		if (sp[i].select == 0) {//��
			glutWireCube(sp[i].size);
		}
		else if (sp[i].select == 1) {//����
			glutWireCone(sp[i].size / 2, sp[i].size / 2, sp[i].slice, sp[i].stacks);
		}
		else if (sp[i].select == 2) {//��
			glutWireSphere(sp[i].size / 2, sp[i].slice, sp[i].stacks);
		}
		else if (sp[i].select == 3) {//�䷯��
			glutWireTorus(sp[i].size, sp[i].size / 4, sp[i].slice, sp[i].stacks);
		}
		else if (sp[i].select == 4) {//������
			glutWireTeapot(sp[i].size);
		}
		glPopMatrix(); //���� ���� �ݱ�
	}


	
	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//��ǥ
		sp[click_count].pos.x = x;
		sp[click_count].pos.y = y;
		//���� ����
		sp[click_count].select = rand() % 5;
		//���� ����
		sp[click_count].size = rand() % 100 + 50;
		sp[click_count].slice = rand()% 10 + 4;
		sp[click_count].stacks = rand() % 10 + 4;
		
		sp[click_count].rot.x = rand() % 2;
		sp[click_count].rot.y = rand() % 2;
		sp[click_count].rot.z = rand() % 2;

		sp[click_count].scale.x = 1;
		
		sp[click_count].cl.R = rand() % 200 + 100;
		sp[click_count].cl.G = rand() % 200 + 100;
		sp[click_count].cl.B = rand() % 200 + 100;


		
		if (click_count == PT) {
			click_count = 0;
			Save = TRUE;
		}
		else {
			click_count++;
		}
		if (Save) {
			save_count = PT;
		}
		else {
			save_count++;

		}
	}
}
void Timerfunction(int value) {
	Time_count++;
	for (int i = 0; i < save_count; i++) {
		sp[i].rot.degree += 10;
	
		if (sp[i].scale.x > 0) {
			sp[i].scale.x -= 0.025;
		}
	
	}
	

	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.
	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'y':
		;
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