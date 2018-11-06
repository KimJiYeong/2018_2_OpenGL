#pragma once
#include <GL/freeglut.h> 

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
	int degree;
};


typedef struct BoOL
{
	int b_x;
	int b_y;
	int b_z;
};

typedef struct QUAD
{
	Color cl[8];//»ö»ó
	Translate_pos pos[8];
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


QUAD index;


void Inde_Box(int pos_x, int pos_y, int pos_z, int size) {

	//ÀÎµ¦½º ¹öÆÛ ÀúÀå
	index.pos[0].x = -1;
	index.pos[0].y = -1;
	index.pos[0].z = 1;

	index.cl[0].R = 1;
	index.cl[0].G = 0;
	index.cl[0].B = 0;

	index.pos[1].x = 1;
	index.pos[1].y = -1;
	index.pos[1].z = 1;

	index.cl[1].R = 1;
	index.cl[1].G = 0;
	index.cl[1].B = 1;

	index.pos[2].x = 1;
	index.pos[2].y = -1;
	index.pos[2].z = -1;

	index.cl[2].R = 0;
	index.cl[2].G = 0;
	index.cl[2].B = 1;

	index.pos[3].x = -1;
	index.pos[3].y = -1;
	index.pos[3].z = -1;

	index.cl[3].R = 0;
	index.cl[3].G = 0;
	index.cl[3].B = 0;

	index.pos[4].x = -1;
	index.pos[4].y = 1;
	index.pos[4].z = 1;

	index.cl[4].R = 1;
	index.cl[4].G = 1;
	index.cl[4].B = 0;

	index.pos[5].x = 1;
	index.pos[5].y = 1;
	index.pos[5].z = 1;

	index.cl[5].R = 1;
	index.cl[5].G = 1;
	index.cl[5].B = 1;

	index.pos[6].x = 1;
	index.pos[6].y = 1;
	index.pos[6].z = -1;

	index.cl[6].R = 0;
	index.cl[6].G = 1;
	index.cl[6].B = 1;

	index.pos[7].x = -1;
	index.pos[7].y = 1;
	index.pos[7].z = -1;

	index.cl[7].R = 0;
	index.cl[7].G = 1;
	index.cl[7].B = 0;

	glPushMatrix(); {
		
		glTranslated(pos_x, pos_y, pos_z);
		glScaled(size, size, size);

		glBegin(GL_POLYGON); {//¾Æ·¡

			glColor3f(index.cl[1].R, index.cl[1].G, index.cl[1].B);
			glVertex3f(index.pos[1].x, index.pos[1].y, index.pos[1].z);//Yellow

			glColor3f(index.cl[0].R, index.cl[0].G, index.cl[0].B);
			glVertex3f(index.pos[0].x, index.pos[0].y, index.pos[0].z);//Yellow

			glColor3f(index.cl[3].R, index.cl[3].G, index.cl[3].B);
			glVertex3f(index.pos[3].x, index.pos[3].y, index.pos[3].z);//Yellow

			glColor3f(index.cl[2].R, index.cl[2].G, index.cl[2].B);
			glVertex3f(index.pos[2].x, index.pos[2].y, index.pos[2].z);//Yellow

		}
		glEnd();

		glBegin(GL_POLYGON); {//¿Þ
			glColor3f(index.cl[0].R, index.cl[0].G, index.cl[0].B);
			glVertex3f(index.pos[0].x, index.pos[0].y, index.pos[0].z);//Yellow

			glColor3f(index.cl[4].R, index.cl[4].G, index.cl[4].B);
			glVertex3f(index.pos[4].x, index.pos[4].y, index.pos[4].z);//Yellow

			glColor3f(index.cl[7].R, index.cl[7].G, index.cl[7].B);
			glVertex3f(index.pos[7].x, index.pos[7].y, index.pos[7].z);//Yellow

			glColor3f(index.cl[3].R, index.cl[3].G, index.cl[3].B);
			glVertex3f(index.pos[3].x, index.pos[3].y, index.pos[3].z);//Yellow

		}
		glEnd();

		glBegin(GL_POLYGON); {//µÞ¸é

			glColor3f(index.cl[2].R, index.cl[2].G, index.cl[2].B);
			glVertex3f(index.pos[2].x, index.pos[2].y, index.pos[2].z);//Yellow

			glColor3f(index.cl[3].R, index.cl[3].G, index.cl[3].B);
			glVertex3f(index.pos[3].x, index.pos[3].y, index.pos[3].z);//Yellow

			glColor3f(index.cl[7].R, index.cl[7].G, index.cl[7].B);
			glVertex3f(index.pos[7].x, index.pos[7].y, index.pos[7].z);//Yellow

			glColor3f(index.cl[6].R, index.cl[6].G, index.cl[6].B);
			glVertex3f(index.pos[6].x, index.pos[6].y, index.pos[6].z);//Yellow


		}
		glEnd();

		glBegin(GL_POLYGON); {//¿À¸¥ÂÊ¸é
			glColor3f(index.cl[1].R, index.cl[1].G, index.cl[1].B);
			glVertex3f(index.pos[1].x, index.pos[1].y, index.pos[1].z);//Yellow

			glColor3f(index.cl[2].R, index.cl[2].G, index.cl[2].B);
			glVertex3f(index.pos[2].x, index.pos[2].y, index.pos[2].z);//Yellow

			glColor3f(index.cl[6].R, index.cl[6].G, index.cl[6].B);
			glVertex3f(index.pos[6].x, index.pos[6].y, index.pos[6].z);//Yellow					

			glColor3f(index.cl[5].R, index.cl[5].G, index.cl[5].B);
			glVertex3f(index.pos[5].x, index.pos[5].y, index.pos[5].z);//Yellow

		}
		glEnd();
		glPushMatrix();
		{
			glTranslated(0, index.pos[7].y, index.pos[7].z);
			glRotated(-index.move.x * 10, 1, 0, 0);
			glTranslated(0, -index.pos[7].y, -index.pos[7].z);

			glBegin(GL_POLYGON); {//¶Ñ²±

				glColor3f(index.cl[4].R, index.cl[4].G, index.cl[4].B);
				glVertex3f(index.pos[4].x, index.pos[4].y, index.pos[4].z);//Yellow

				glColor3f(index.cl[5].R, index.cl[5].G, index.cl[5].B);
				glVertex3f(index.pos[5].x, index.pos[5].y, index.pos[5].z);//Yellow

				glColor3f(index.cl[6].R, index.cl[6].G, index.cl[6].B);
				glVertex3f(index.pos[6].x, index.pos[6].y, index.pos[6].z);//Yellow				

				glColor3f(index.cl[7].R, index.cl[7].G, index.cl[7].B);
				glVertex3f(index.pos[7].x, index.pos[7].y, index.pos[7].z);//Yellow

			}
			glEnd();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslated(0, index.pos[0].y, index.pos[0].z);
			glRotated(index.move.y * 10, 1, 0, 0);
			glTranslated(0, -index.pos[0].y, -index.pos[0].z);

			glBegin(GL_POLYGON); {//¾Õ¸é

				glColor3f(index.cl[0].R, index.cl[0].G, index.cl[0].B);
				glVertex3f(index.pos[0].x, index.pos[0].y, index.pos[0].z);//Yellow

				glColor3f(index.cl[1].R, index.cl[1].G, index.cl[1].B);
				glVertex3f(index.pos[1].x, index.pos[1].y, index.pos[1].z);//Yellow

				glColor3f(index.cl[5].R, index.cl[5].G, index.cl[5].B);
				glVertex3f(index.pos[5].x, index.pos[5].y, index.pos[5].z);//Yellow


				glColor3f(index.cl[4].R, index.cl[4].G, index.cl[4].B);
				glVertex3f(index.pos[4].x, index.pos[4].y, index.pos[4].z);//Yellow
			}
			glEnd();
		}
		glPopMatrix();
	}
	glPopMatrix();

}
