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

Shape sub[2];

//ī�޶�-----------------

Shape camera;
Translate_pos EYE;
Translate_pos AT;
Translate_pos UP;

static int __x;
static int __y;
static int __z;


#define Size 36

Shape tree;//����
//Shape tree;
Shape chin_man;//ö�����
Shape run_man;//ö�����
Shape run;//ö�����
Shape plan;
Shape press_man;
			  //Shape tree;
Shape ball;
Shape clain;
const void camera_custom
(double pos_x, double pos_y, double pos_z, //��ġ
	double degree, const double rot_x, const double rot_y, const double rot_z, //ȸ��
	const double move_x, const double move_y, const double move_z //������
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

void SetupRC()
{
	//�ʱ�ȭ
	glTranslated(WideSize / 2, HighSize / 2, Z_Size / 2);
}
void main(int argc, char *argv[]) {
	//�ʱ�ȭ
	camera.rot.x = 0;
	camera.rot.y = 0;
	camera.rot.z = 0;

	camera.move.x = 0;
	camera.move.y = 0;
	camera.move.z = 0;

	EYE.x = 0, EYE.y = 0, EYE.z = 300;//EYE���� �ʱ�ȭ
	AT.x = 0, AT.y = 0, AT.z = 0;//EYE���� �ʱ�ȭ
	UP.x = 0, UP.y = 1, UP.z = 0;//EYE���� �ʱ�ȭ

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
	

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);//������ ���� ��ǥ
	glutInitWindowSize(WideSize, HighSize); //������ ���� ũ��
	glutCreateWindow("ex16");
	// - �������� ���� ���� �����ϱ�
	//���� �׸���
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
//2���� r
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
			EYE.x, EYE.y, EYE.z,  //��5 eye
			AT.x, AT.y, AT.z, //���� center
			0, 1, 0 //���ʹ���(�ǵ� ����) up
		);
		//�� ����
		glPushMatrix();//---------------------------------------
		{
			//��
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

			}glPopMatrix();//�� ��

			glPushMatrix();//��ġ ������ 
			{
				glColor3d((float)204 / 255, (float)61 / 255, (float)61 / 255);
				glTranslated(-BOX_SIZE/2 + 20/2, 0, -BOX_SIZE/2 + 20/2);//��ġ ����
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
					glutSolidCylinder(10 / 2, 30, 6, 6);//���� ����
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
				
				glutSolidCylinder(4 / 2, 20, 6, 6);//���� ����
				glPopMatrix();
				glPopMatrix();
				glPopMatrix();

				glColor3d((float)76 / 255, (float)76 / 255, (float)76 / 255);
				glPushMatrix();//���
				{
					glTranslated(0, 5, 20);
					glRotated(-90, 1, 0, 0);
					glRotated(180, 0, 1, 0);
					for (int i = 0; i < 4; i++) {
						glPushMatrix();//���
						if (i < 2) {
							glTranslated(0, -10, 0);
							glPushMatrix();//���
							if (i % 2 == 0) {
								glTranslated(-2, 10, 0);
								glRotated(-90, 1, 0, 0);
							}
							else {
								glTranslated(2, 10, 0);
								glRotated(-90, 1, 0, 0);
							}

							glutSolidCylinder(2 / 2, 10, 6, 6);//���� ����
							glPopMatrix();
							glPopMatrix();//��� ��
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
									glutSolidCylinder(2 / 2, 5, 6, 6);//���� ����
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
									glutSolidCylinder(2 / 2, press_man.move.y + 2, 6, 6);//���� ����
								}
								glPopMatrix();
							}
						
							
						
							glPopMatrix();//��� ��
						}

					}
					//��
					glPushMatrix();
					glTranslated(0, 15, 0);
					glScaled(0.5, 1, 0.4);
					glutSolidCube(10);
					glPopMatrix();

					//�Ӹ�
					glPushMatrix();
					glTranslated(0, 25, 0);
					glutSolidSphere(2, 8, 8);
					glPopMatrix();
				}
				glPopMatrix();//��� ��

			}glPopMatrix();//��ġ ������ ��
		
			glPushMatrix();//����
			{
				glColor3d((float)204 / 255, (float)61 / 255, (float)61 / 255);
				glTranslated(BOX_SIZE / 2 - 20 , 0, -BOX_SIZE / 2 + 20 );//��ġ ����
				glPushMatrix();
				glScaled(0.5, 1, 0.5);
				glRotated(-90, 1, 0, 0);
				glutSolidCylinder(20/2, 20, 6,6);//���� ����
				glPopMatrix();
				
				//���� �Ӹ�
				glPushMatrix();
				glColor3d((float) 34/ 255, (float)116 / 255, (float)28 / 255);
				glTranslated(0, 20, 0);
				glScaled(tree.scale.x * 2, tree.scale.x * 2, tree.scale.x * 2);
				glutSolidSphere(20, 10, 10);
				glPopMatrix();

			}glPopMatrix();//���� ��

			glPushMatrix();//ö��
			{
				glColor3d((float)153 / 255, (float)112 / 255, (float)0 / 255);
				glTranslated(-BOX_SIZE / 2 + 20 / 2, 0, BOX_SIZE / 2 - 20 / 2);//��ġ ����
				
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
					glutSolidCylinder(10 / 2, 30, 6, 6);//���� ����
					glPopMatrix();
				}
				glColor3d((float)67 / 255, (float)116 / 255, (float)217 / 255);

				glPushMatrix();
				glTranslated(-10, 25, -10);
				glPushMatrix();
				glRotated(90, 0, 1, 0);
				glutSolidCylinder(5 / 2, 20, 6, 6);//���� ����
				glPopMatrix();
				glPopMatrix();

				glColor3d((float)76 / 255, (float)76 / 255, (float)76 / 255);
				glPushMatrix();//���
				{
					glTranslated(0, 25, -10);
					glRotated(chin_man.rot.degree, 1, 0, 0);

					for (int i = 0; i < 4; i++) {
						glPushMatrix();//���
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

						glutSolidCylinder(2 / 2, 10, 6, 6);//���� ����
					
						glPopMatrix();//��� ��
					}
					//��
					glPushMatrix();
					glTranslated(0, 15, 0);
					glScaled(0.5, 1, 0.4);
					glutSolidCube(10);
					glPopMatrix();

					//�Ӹ�
					glPushMatrix();
					glTranslated(0, 25, 0);
					glutSolidSphere(2, 8,8);
					glPopMatrix();
				}
				glPopMatrix();//��� ��

			}glPopMatrix();//ö�� ��

			glPushMatrix();//���׸ӽ�
			{
				glColor3d((float)204 / 255, (float)61 / 255, (float)61 / 255);
				glTranslated(BOX_SIZE / 2 - 20 / 2, 0, BOX_SIZE / 2 - 20 / 2);//��ġ ����
				
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
					glutSolidCylinder(10 / 2, 30, 6, 6);//���� ����
					glPopMatrix();
				}
				glColor3d((float)67 / 255, (float)116 / 255, (float)217 / 255);

				glPushMatrix();
				glTranslated(-10, 25, -10);
				glPushMatrix();
				glRotated(90, 0, 1, 0);
				glutSolidCylinder(5 / 2, 20, 6, 6);//���� ����
				glPopMatrix();
				glPopMatrix();
			
				glColor3d((float)25 / 255, (float)25 / 255, (float)25 / 255);
				
				glPushMatrix();
				glTranslated(0, 0, -30);
				glRotated(90, 0, 1, 0);
				glScaled(1, 0.1, 1);
				glPushMatrix();
				glRotated(-run.rot.degree, 0, 0, 1);
				glutWireTorus(10, 20, 6, 6);//���� ����
				glPopMatrix();
				glPopMatrix();
 
				glColor3d((float)166 / 255, (float)166 / 255, (float)166 / 255);

				glPushMatrix();
				glTranslated(0, 0, -30);
				glRotated(90, 0, 1, 0);
				glScaled(1, 0.07, 1);
				glPushMatrix();
				glutSolidTorus(10, 20, 6, 6);//���� ����
				glPopMatrix();
				glPopMatrix();

				glColor3d((float)6 / 255, (float)0 / 255, (float)0 / 255);

				glPushMatrix();//���
				{
					glTranslated(0, 0, -30);

					for (int i = 0; i < 4; i++) {
						glPushMatrix();//���
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
						//��
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

						glutSolidCylinder(2 / 2, 10, 6, 6);//���� ����

						glPopMatrix();//��� ��
					}
					//��
					glPushMatrix();
					glTranslated(0, 15, 0);
					glScaled(0.5, 1, 0.4);
					glutSolidCube(10);
					glPopMatrix();

					//�Ӹ�
					glPushMatrix();
					glTranslated(0, 25, 0);
					glutSolidSphere(2, 8, 8);
					glPopMatrix();
				}
				glPopMatrix();//��� ��


			}glPopMatrix();//���׸ӽ� ��

			glPushMatrix();//�����
			{
				glColor3d((float)204 / 255, (float)61 / 255, (float)61 / 255);
				glTranslated(plan.move.x * plan.pos.degree, plan.move.y, plan.move.z);//��ġ ����
				glScaled(0.5, 0.5, 0.5);
				glPushMatrix();
				glRotated(-plan.rot.degree * plan.pos.degree, 0, 1, 0);
				
				//����� ��ü �Ӹ�
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

				//����� ����
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

				//�����緯
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

			}glPopMatrix();//����� ��
			glPopMatrix();

			glPushMatrix();//��ŷ��
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

			glPushMatrix();//ũ����
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
	
	//Ÿ�̸� ����
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

	plan.move.y = 100;
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
	
		//---------��

		if (!ani) {
			move_count += 1;//Ÿ�̸� ī��Ʈ
		}

		ball.rot.degree += PI * 2;
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




		//ũ����
		//�̵��ϴ� �ڵ�
		if ((clain.move.x == 100) || (clain.move.x == -100)) {
			clain.stacks *= -1;
		}
		clain.move.x += (1 * clain.stacks);
		//y �� ȸ��
		if (clain.any) {
			clain.rot.degree += 2;
			clain.rot.x = 0;
			clain.rot.y = 1;
			clain.rot.z = 0;
		}

		//ũ���ΰ� ��ŷ�� 
		if ((clain.move.x - clain.size < ball.move.x + ball.pos.x) && ( ball.move.x + ball.pos.x < clain.move.x + clain.size)) {// z ��
			if((clain.move.z - clain.size < ball.move.z + ball.pos.z) && (ball.move.z + ball.pos.z < clain.move.z + clain.size)) {// z ��
				printf("clain");
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
				printf("a");

		}			
		if ((ball.pos.z + ball.move.z < -100)) {
			ball.b.b_z = false;
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
		

		//Ű �Է� �ȹޱ�


	Time_count++;
	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.
	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���

}
int ttt;
int ani_count;

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
		//---------ī�޶�
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

		// z�� �״�� camera.z 

		//-----------ī�޶� �� --------
	
	case '2'://�������� ����
		ani_count++;
		if (ani_count % 2 == 1) {
			ani = TRUE;//ȸ���ϴ°� Ʈ���
			Reshape(WideSize, HighSize);
		}
		else {
			ani = FALSE;//ȸ���ϴ°� Ʈ���
			Reshape(WideSize, HighSize);
		}
		break;

	//����� ���� �ٲٱ�
	case '/':
		plan.pos.degree *= -1;
		break;


	//��ŷ��
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
	//ũ����
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
		//----------ù��° ���� x �� �̵�
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
		//----------ù��° ���� y �� �̵�
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

		//----------�ι�° ���� x �� �̵�
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
		//----------�ι�° ���� z �� �̵�
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
	//gluPerspective(45, WideSize / HighSize, 1, Z_Size); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!
	//glTranslatef(0, 0, -300);


	if (!ani) {
		//glRotatef(30, 0, 1, 0);
		gluPerspective(60.0f, w / h, 1.0, 1000.0);

		glTranslated(0.0, 0.0, -300.0);     // ���� ������ ȭ�� �������� �̵��Ͽ� �þ߸� Ȯ���Ѵ�.
	}
	else {
		glOrtho(0, WideSize, HighSize, 0, -Z_Size / 2, Z_Size / 2); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!
		glTranslated(WideSize / 2, HighSize / 2, 0);
		//glRotatef(-60, 1, 0, 0);

	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}