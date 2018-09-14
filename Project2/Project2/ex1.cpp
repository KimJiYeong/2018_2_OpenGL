#include <GL/freeglut.h> 
GLvoid drawScene (GLvoid); 
GLvoid Reshape (int w, int h);

void main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);//������ ���� ��ǥ
	glutInitWindowSize(800, 600); //������ ���� ũ��
	glutCreateWindow("Example2");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid)
	{
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//4���� �׸���
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glRectf(-1.0f, -1.0f, -0.5f, -0.5f);
	
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glRectf(1.0f, 1.0f, 0.5f, 0.5f);
	
	glColor4f(0.0f, 0.5f, 1.0f, 1.0f);
	glRectf(-1.0f, 1.0f, -0.5f, 0.5f);

	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	glRectf(1.0f, -1.0f, 0.5f, -0.5f);

	//�����׸���
	GLint p1[2] = { -1.0f, 0.0f};
	GLint p2[2] = { 1.0f, 0.0f};
	GLint p3[2] = { 0.0f, 1.0f };
	GLint p4[2] = { 0.0f, -1.0f };

	glLineWidth(1.0);
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_LINES);
	glVertex2iv(p1);
	glVertex2iv(p2);
	glEnd();

	glBegin(GL_LINES);
	glVertex2iv(p3);
	glVertex2iv(p4);
	glEnd();

	//�ﰢ�� �׸���
	glColor4f(0.0f, 1.0f, 0.5f, 1.0f);
	
	glBegin(GL_TRIANGLES);
	glVertex3f(0.2f, -0.2f, 0.0f);
	glVertex3f(-0.2f, -0.2f, 0.0f);
	glVertex3f(0.0f, 0.2f, 0.0f);
	glEnd();

	glFlush();
}

GLvoid Reshape(int w, int h)
	{
	glViewport(0, 0, w, h);

	}