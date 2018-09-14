//����� ���� �˻�

#include <GL/freeglut.h> 
#include<random>
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

#define WideSize 800
#define HighSize 600

//���� ����
int rw_slide, rh_slide =0;
int r_color[5];

void main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WideSize, HighSize);
	glutCreateWindow("ex2");

	//Ư�� ������ ���� �����ϰ� ����
	//Ư�� ���� ����
	rw_slide = 5;
	rh_slide = 3;

	//Ư�� �� ����
	for (int i = 0; i < rw_slide; i++) {
		r_color[i] = rand() % rh_slide; //���� ���� ���Ѵ�.
	}
	
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT);
	for (int j = 0; j < rh_slide; j++) {
		for (int i = 0; i < rw_slide; i++) {
			glClearColor((float)200, (float)200 - j * 10, (float)200, 1.0f);	//RGB�Է��ϱ� float ����
			glRectf(
				(WideSize / rw_slide) * i , (HighSize / rh_slide) * j, 
				(WideSize / rw_slide) * (i+1), (HighSize / rh_slide) * (j+1));
		}
	}
	glFlush();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0, 800, 600, 0, -1.0, 1.0); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!

}