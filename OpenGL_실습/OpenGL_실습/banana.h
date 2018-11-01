#pragma once
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
	float degree;
};


typedef struct BoOL
{
	int b_x;
	int b_y;
	int b_z;
};

typedef struct QUAD
{
	Color cl[74 / 2];//색상
	Translate_pos pos[74 / 2];//총 정점 수 74 / 2
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

class Banana
{
public:
	Banana();
	~Banana();

private:

};

Banana::Banana()
{
}

Banana::~Banana()
{
}



void banana_draw(int pivot_x, int pivot_y, int pivot_z, int size) {

	//인덱스 버퍼 저장
	index.pos[0].x = -1;
	index.pos[0].y = -1;
	index.pos[0].z = 1;


	glPushMatrix(); {

		glTranslated(pivot_x, pivot_y, pivot_z);
		glScaled(size, size, size);

		glBegin(GL_POLYGON); {//아래

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

	}
	glPopMatrix();

}
