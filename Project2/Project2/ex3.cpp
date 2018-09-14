#include <GL/freeglut.h> 
#include <random>
#include <time.h>
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

#define WideSize 900
#define HighSize 900

typedef struct Shape {
	GLint pt[2];

};
Shape line;
Shape triangle;
static int start_shape;
static BOOL go_vertex;
void main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);//윈도우 띄우기 좌표
	glutInitWindowSize(WideSize, HighSize); //윈도우 띄우기 크기
	glutCreateWindow("ex3");
	// - 랜덤으로 시작 도형 설정하기
	//도형 그리기
	
	if (start_shape == 3) {
		go_vertex = FALSE;
	}
	else {
		go_vertex = TRUE;
	}

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	
	srand(time(NULL));
	start_shape = rand() % 4;
	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	/*
	좌표 for문 미사용
	
	GLint w_line[6][2];

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			if (j % 2 == 0) {
				w_line[i][j] = 0;
				w_line[i+1][j] = 900;
			}
			else {
				w_line[i][j] = 300 * i;
				w_line[i+1][j] = 300 * i;
			}
		}
	}
*/

	//격자 칸 그리기 

	glLineWidth(1.0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	for (int i = 1; i < 3; i++) {
		GLint w_line1[2] = { 0,300 * i };
		GLint w_line2[2] = { 900 , 300 * i };
		glVertex2iv(w_line1);
		glVertex2iv(w_line2);

	}
	glEnd();

	glBegin(GL_LINES);
	for (int i = 1; i < 3; i++) {
		GLint h_line1[2] = {300 * i,0};
		GLint h_line2[2] = { 300 * i , 900 };
		glVertex2iv(h_line1);
		glVertex2iv(h_line2);

	}
	glEnd();

	
	// 도형 좌표는 100 ~ 200 이면 될듯

	glLineWidth(1.0);
	glColor3f(0.0f, 0.0f, 0.4f);

	//기본 시작 좌표는 주어지고 여기에서 하나씩 늘려간다

	for (int j = 0; j < 3; j++) {
	for (int i = 0; i < 3; i++) {
			glBegin(GL_LINE_LOOP);
			GLint d_line1[2] = { 100 + i * 300 , 100 + j * 300};
			GLint d_line4[2] = { 100 + 300 * i, 200 + j * 300};
			GLint d_line3[2] = { 200 + 300 * i, 200 + j * 300};
			GLint d_line2[2] = { 200 + 300 * i, 100 + j * 300 };
			GLint d_line5[2] = { 250 + 300 * i, 150 + j * 300 };

			if (start_shape == 0) {
				glVertex2iv(d_line1);
				glVertex2iv(d_line3);

				go_vertex = TRUE;
				start_shape++;
			}
			else if (start_shape == 1) {
				glVertex2iv(d_line1);
				glVertex2iv(d_line2);
				glVertex2iv(d_line3);
				if (go_vertex) {
					start_shape++;
				}
				else {
					start_shape--;
				}

			}//삼각형
			else if (start_shape == 2) {

				glVertex2iv(d_line1);
				glVertex2iv(d_line4);
				glVertex2iv(d_line3);
				glVertex2iv(d_line2);
				if (go_vertex) {
					start_shape++;
				}
				else {
					start_shape--;
				}

			}//사각형
			else if (start_shape == 3) {
				glVertex2iv(d_line1);
				glVertex2iv(d_line4);
				glVertex2iv(d_line3);
				glVertex2iv(d_line5);
				glVertex2iv(d_line2);

				go_vertex = FALSE;
				start_shape--;

			}//오각형	
			glEnd();

		}
		
	}
		
	glFlush();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0,900, 900, 0, -1.0, 1.0); //윈도우를 초기화 하는 함수입니다!

}