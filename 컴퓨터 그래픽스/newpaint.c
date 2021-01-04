/* This program illustrates the use of the glut library for
interfacing with a window system */

//이것들은 걍 enum관련된게 없어서 해놓은듯. 원 그리기나 추가 기능들 넣으려면 여기서 설정 해주고 밑에서 추가적으로 적으면 됨
/*
#define NULL 0
#define LINE 1
#define RECTANGLE 2
#define TRIANGLE  3
#define POINTS 4
#define TEXT 5
#define CIRCLE 6
*/

enum MODE { NULL, LINE, RECTANGLE, TRIANGLE, POINTS, STAR, CIRCLE, DRAW, ERASE };
typedef enum { false1, true1 } boolean;

#include<math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

void mouse(int, int, int, int);
void key(unsigned char, int, int);
void display(void);
void drawSquare(int, int);
void myeshape(GLsizei, GLsizei);

void myinit(void);

void screen_box(int, int, int);
void right_menu(int);
void middle_menu(int);
void color_menu(int);
void pixel_menu(int);
void fill_menu(int);
int pick(int, int);

void menubar();
void Functionbar();
void Button(int, int);
boolean Button_locate(int, int, int, int);
void StringText(char String[], int x, int y);
void Button_click(int);
void getRGB(int, int);
void eraser_thumnail();
boolean menubarFuction(int, int);
/* glbals */

GLsizei wh = 500, ww = 500; /* initial window size */
GLfloat size = 3.0;   /* half side length of square */
int draw_mode = 0; /* drawing mode */
int rx, ry; /*raster position*/
static int xp[2], yp[2];
static int count;

GLfloat r = 1.0, g = 1.0, b = 1.0; /* drawing color */
GLfloat bg_r = 1.0, bg_g = 1.0, bg_b = 1.0;

int fill = 0; /* fill flag */
int dragCheck = 0;

int menubarcheck = 0;
int menucheck = 0;
int buttoncheck = 0;
int drawsize;

struct vertexNode
{
	GLint vx;
	GLint vy;
	struct vertexNode* next;
};


struct object
{
	GLfloat color[4];
	struct vertexNode* vertex;
	int fill;
	int mode;
	struct object* next;
}*head, *tail, *lineObj, *draw, *redo, *redoTail;

//오브젝트 리스트 초기값 설정
void initObj(struct object* obj)
{
	obj->vertex = '\0';
	obj->fill = 0;
	obj->next = '\0';
	obj->mode = 0;
}

//정점리스트 초기값 설정
void initNode(struct vertexNode* node)
{
	node->next = '\0';
	node->vx = 0;
	node->vy = 0;
}

void addObj(struct object* new1, struct object** head)
{
	struct object* p = *head;
	if (p->next == NULL)
		p->next = new1;
	else
	{
		while (p->next != tail)
			p = p->next;
		p->next = new1;
	}
	new1->next = tail;
}



void addVertex(struct vertexNode* new1, struct vertexNode** vertexHead)
{
	struct vertexNode* p = *vertexHead;
	if (p->next == NULL)
		p->next = new1;
	else
	{
		while (p->next != NULL)
			p = p->next;
		p->next = new1;
	}
}

void drawList()
{
	int count = 0;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	struct object* p = head;
	if (p->next == NULL)
		return;
	while (p->next != tail)
	{
		count++;
		p = p->next;
		struct vertexNode* node = p->vertex;
		glColor3f(p->color[0], p->color[1], p->color[2]);
		if (p->fill == 1) glBegin(GL_POLYGON);
		else if (p->fill == 2) glBegin(GL_LINE_STRIP);
		else glBegin(GL_LINE_LOOP);
		if (p->mode != STAR || (p->mode == STAR && p->fill == 0))
		{
			while (node->next != NULL)
			{
				node = node->next;
				glVertex2i((node->vx*drawsize) / 500 + 80, wh - (node->vy*drawsize) / 500 - 20);
			}
			glEnd();
		}
		else
		{
			while (node->next != NULL)

			{
				int count = 0;
				while (count < 3)
				{
					node = node->next;
					glVertex2i((node->vx*drawsize) / 500 + 80, wh - (node->vy*drawsize) / 500 - 20);
					count++;
				}
				glEnd();
				glBegin(GL_POLYGON);
			}
			glEnd();
		}
	}
	glPopAttrib();
}

void removeVertex(struct vertexNode** head)
{
	struct vertexNode* vnode = *head;
	if (vnode->next != NULL)
	{
		while (vnode->next != NULL)
		{
			struct vertexNode* tmp = vnode->next;
			vnode->next = NULL;
			vnode = NULL;
			free(vnode);
			vnode = tmp;
		}
		free(vnode);
		vnode = NULL;
	}
}

void redoInit()
{
	struct object* p = redo;
	if (redo != NULL)
	{
		while (p != NULL)
		{
			struct object* tmp = p->next;
			p->next = NULL;
			removeVertex(&p->vertex);
			p = NULL;
			free(p);
			p = tmp;
		}
		free(redo);
		redo = NULL;
	}
}

struct object* pop()
{
	struct object* p = redo;
	struct object* prior = NULL;
	if (redo != NULL)
	{
		while (p->next != NULL)
		{
			prior = p;
			p = p->next;
		}
		if (prior != NULL)
			prior->next = NULL;
		else
			redo = NULL;
		return p;
	}
	return NULL;
}

void push(struct object* node)
{
	struct object* p = redo;
	node->next = NULL;
	if (redo == NULL)
	{
		redo = node;
		return;
	}
	else
	{
		while (p->next != NULL)
		{
			p = p->next;
		}
		p->next = node;
	}
}

void Redo(struct object** head)
{
	struct object *p = *head;
	if ((redo != NULL) && (head != NULL))
	{
		while (p->next != tail)
		{
			p = p->next;
		}
		p->next = pop();
		p->next->next = tail;
	}
	glutPostRedisplay();
}

void undo(struct object** head)
{
	struct object* p = *head;
	if ((p->next != NULL) && (p->next != tail))
	{
		while (p->next->next != tail)
			p = p->next;
		push(p->next);
		p->next = tail;
		glutPostRedisplay();
	}
}

struct object* makeObj(int fill)
{
	struct object* p = (struct object*)malloc(sizeof(struct object));
	initObj(p);
	
	p->vertex = (struct vertexNode*)malloc(sizeof(struct vertexNode));
	initNode(p->vertex);
	
	p->fill = fill;
	p->mode = draw_mode;
	if (draw_mode != 8)
	{
		p->color[0] = r;
		p->color[1] = g;
		p->color[2] = b;
		p->color[3] = 1.0;
	}
	else
	{
		p->color[0] = bg_r;
		p->color[1] = bg_g;
		p->color[2] = bg_b;
		p->color[3] = 1.0;
	}
	return p;
}


void makeVertex(int x, int y, struct object* p)
{
	struct vertexNode* v = (struct vertexNode*)malloc(sizeof(struct vertexNode));
	initNode(v);
	v->vx = 500 * (x - 80) / drawsize;
	v->vy = 500 * (y - 20) / drawsize;
	addVertex(v, &p->vertex); //초기버전
}

void CreateCircle1(int centerx, int centery, int rad, int fill)
{
	struct object* p = makeObj(fill);
	for (int angle = 0; angle < 360; angle += 1)
	{
		int px = rad * cos(angle*0.017);
		int py = rad * sin(angle*0.017);

		makeVertex(centerx + px, wh - centery + py, p);
	}
	addObj(p, &head);
}

void CreateStar(int x1, int y1, int x2, int y2, struct object* p)
{
	int tmp = 0;
	if (x2 < x1)
	{
		tmp = x2;
		x2 = x1;
		x1 = tmp;
	}
	if (y2 < y1)
	{
		tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	int xlength = x2 - x1;
	int ylength = y2 - y1;
	
	if (p->fill == 0)
	{
		makeVertex(0.5*xlength + x1, y1, p);					//1
		makeVertex(0.38*xlength + x1, 0.38*ylength + y1, p);	//2
		makeVertex(x1, 0.4*ylength + y1, p);					//3
		makeVertex(0.3*xlength + x1, 0.62*ylength + y1, p);		//4
		makeVertex(0.19*xlength + x1, y2, p);					//5
		makeVertex(0.5*xlength + x1, 0.77*ylength + y1, p);		//6
		makeVertex(0.81*xlength + x1, y2, p);					//7
		makeVertex(0.7*xlength + x1, 0.62*ylength + y1, p);		//8
		makeVertex(x2, 0.4*ylength + y1, p);					//9
		makeVertex(0.62*xlength + x1, 0.38*ylength + y1, p);	//10
	}
	else
	{
		makeVertex(0.5*xlength + x1, y1, p);					//1
		makeVertex(0.19*xlength + x1, y2, p);					//5
		makeVertex(0.7*xlength + x1, 0.62*ylength + y1, p);		//8
		makeVertex(0.5*xlength + x1, y1, p);					//1
		makeVertex(0.3*xlength + x1, 0.62*ylength + y1, p);		//4
		makeVertex(0.81*xlength + x1, y2, p);					//7
		makeVertex(x1, 0.4*ylength + y1, p);					//3
		makeVertex(0.5*xlength + x1, 0.77*ylength + y1, p);		//6
		makeVertex(x2, 0.4*ylength + y1, p);					//9
	}
	
}

void drawSquare(int x, int y)
{
	y = wh - y;
	//glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256);
	// 오브젝트 리스트를 출력하는 형태로 구조가 바뀜
	/*	glBegin(GL_POINTS);
	glVertex2f(x+size, y+size);
	glVertex2f(x-size, y+size);
	glVertex2f(x-size, y-size);
	glVertex2f(x+size, y-size);
	glEnd(); */
	struct object *p = makeObj(1);

	makeVertex(x + size, wh - y + size, p);
	makeVertex(x - size, wh - y + size, p);
	makeVertex(x - size, wh - y - size, p);
	makeVertex(x + size, wh - y - size, p);

	addObj(p, &head);
}

void clearObject()
{
	struct object* p = head;

	while (p->next != tail)
	{
		undo(&head);
	}
	glutPostRedisplay();
}

/* rehaping routine called whenever window is resized
or moved */
//크기 변경 시 뷰포트랑 뭐 그런거 변경해주는거. wh랑 ww도 똑같이 바꿔줌.
void myReshape(GLsizei w, GLsizei h)
{

	/* adjust clipping box */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* adjust viewport and  clear */

	glViewport(0, 0, w, h);
	glClearColor(0.8, 0.8, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	display();
	glutSwapBuffers();

	/* set global size for use by drawing routine */

	ww = w;
	wh = h;
}

void myinit(void)
{

	glViewport(0, 0, ww, wh);


	/* Pick 2D clipping window to match size of X window
	This choice avoids having to scale object coordinates
	each time window is resized */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)ww, 0.0, (GLdouble)wh, -1.0, 1.0);

	/* set clear color to black and clear window */

	glClearColor(0.8, 0.8, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}


//마우스와 관련된 이벤트 함수다.
void mouse(int btn, int state, int x, int y)
{
	int where = draw_mode;

	struct object* p;
	double rad;
	//왼쪽 버튼을 누르면
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		if (menubarFuction(x, y))	where = pick(x, y);
		//COLOR메뉴에서 정한 값이 들어간다.
		glColor3f(r, g, b);

		if (where != 0)
		{
			count = 0;
			draw_mode = where;
		}//아무 버튼을 선택하지 않았으면 걍 스킵
		else
		{
			redoInit();
			switch (draw_mode)
			{
			case(DRAW):
				if (!fill) fill = 2;
				lineObj = makeObj(fill);
				break;
			case(LINE):
				if (count == 0)
				{
					count++;
					xp[0] = x;
					yp[0] = y;

					draw = makeObj(fill);
				}
				//시작점 설정
				/*
				else
				{
				p = makeObj(fill);
				makeVertex(x, y, p);
				makeVertex(xp[0], yp[0], p);

				addObj(p, &head);
				//draw_mode=0;
				count = 0;
				}//처음 점에서 count를 증가시켰으니 메뉴가 바뀌지 않은 이상 시행됨
				//두번째 선택 위치와 처음 선택 위치에 점을 찍고 선 연결, x는 그대로, y좌표는 wh-y로 한다. 윈도우 좌표와 그리는 좌표의 원점 위치가 달라서 그런듯 하다.
				*/
				break;
			case(RECTANGLE):

				if (count == 0)
				{
					count++;
					xp[0] = x;
					yp[0] = y;

					draw = makeObj(fill);
				}//시작점 설정 - 똑같음
				 /*
				 else
				 {
				 p = makeObj(fill);

				 makeVertex(x, y, p);
				 makeVertex(x, yp[0], p);
				 makeVertex(xp[0], yp[0], p);
				 makeVertex(xp[0], y, p);

				 addObj(p, &head);

				 //draw_mode=0;
				 count = 0;
				 }//사각형의 좌측 상단과 우측 하단점을 찍어서 표현
				 */
				break;
			case (TRIANGLE):
				switch (count)
				{
				case(0):
					count++;
					xp[0] = x;
					yp[0] = y;
					break;
				case(1):
					count++;
					xp[1] = x;
					yp[1] = y;

					draw = makeObj(fill);
					break;
				}//삼각형의 경우는 점이 3개가 필요하기에 스위치문 사용

				break;
			case(POINTS):
			{
				drawSquare(x, y);
				count++;
			}//걍 점 찍는거다
			break;
			case(STAR):
			{
				xp[0] = x;
				yp[0] = y;
				count++;

				draw = makeObj(fill);
			}
			case(CIRCLE):
			{
				if (count == 0)
				{
					count++;
					xp[0] = x;
					yp[0] = y;

					draw = makeObj(fill);
				}//시작점 설정 - 똑같음
				 /*
				 else
				 {

				 int rad;
				 if ((x - xp[0]) > (y - yp[0]))
				 rad = (x - xp[0]) / 2;
				 else
				 rad = (y - yp[0]) / 2;
				 if (rad < 0) rad = -rad;
				 int centerx = (x + xp[0]) / 2;
				 int centery = wh - (y + yp[0]) / 2;
				 CreateCircle1(centerx, centery, rad, fill);
				 //draw_mode = 0;
				 count = 0;
				 }
				 */
				break;
			}
			case(ERASE):
				if (count == 0)
				{
					count++;
					xp[0] = x;
					yp[0] = y;

					draw = makeObj(1);
				}
			}
		}
	}
	else if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (buttoncheck != 0) buttoncheck = 0;

		if (count > 0)
		{
			switch (draw_mode)
			{
			case LINE:
				p = makeObj(fill);
				makeVertex(x, y, p);
				makeVertex(xp[0], yp[0], p);

				addObj(p, &head); 
				count = 0;
				break;
			case RECTANGLE:
				p = makeObj(fill);

				makeVertex(x, y, p);
				makeVertex(x, yp[0], p);
				makeVertex(xp[0], yp[0], p);
				makeVertex(xp[0], y, p);

				addObj(p, &head);
				count = 0;
				break;
			case CIRCLE:
				rad = sqrt(pow(x - xp[0], 2) + pow(y - yp[0], 2));
				if (rad < 0) rad = -rad;
				CreateCircle1(xp[0], wh - yp[0], rad, fill);
				count = 0;
				break;
			case ERASE:
				p = makeObj(1);

				makeVertex(x, y, p);
				makeVertex(x, yp[0], p);
				makeVertex(xp[0], yp[0], p);
				makeVertex(xp[0], y, p);

				addObj(p, &head); 
				count = 0;
				break;
			case TRIANGLE:
				if (count == 2)
				{
					p = makeObj(fill);

					makeVertex(xp[0], yp[0], p);
					makeVertex(xp[1], yp[1], p);
					makeVertex(x, y, p);

					addObj(p, &head);

					//draw_mode = 0;
					count = 0;
				}
				break;
			case STAR:
				p = makeObj(fill);
				CreateStar(xp[0], yp[0], x, y, p);

				addObj(p, &head);
				count = 0;
				break;
			default:
				return;
			}
		}
	}
	glutPostRedisplay();
	glPopAttrib();
}
//클릭한 마우스의 좌표를 계산해서 출력하는듯함
int pick(int x, int y)
{
	/*
	//마우스 좌표와 그림 좌표는 다르다
	y = wh - y;
	if(y < wh-ww/10) return 0; //해당 y좌표 안에 버튼이 있음
	//line은 0-0.1비율의 지점에 있다.
	else if(x < ww/10) return LINE;
	//사각형 그리기 버튼은 0.1-0.2지점에 있다.
	else if(x < ww/5) return RECTANGLE;
	//삼각형은 0.2-0.3지점이다.
	else if(x < 3*ww/10) return TRIANGLE;
	//점은 0.3-0.4지점이다.
	else if(x < 2*ww/5) return POINTS;
	//0.5 이상부터는 텍스트다. 다음 버튼들을 넣는다면 적당히 계산해서 넣어라
	else if(x < ww/2) return TEXT;
	//나머지는 0
	else if (x < 3 * ww / 5) return CIRCLE;
	else if (x < 7 * ww / 10) return DRAW;
	else return 0;
	*/


	if (80 < x && x < 80 + drawsize && 20 < y && 20 + drawsize > y) return 0;
	else if (Button_locate(x, y, 5, 25))
	{
		buttoncheck = 1;
		return LINE;
	}
	else if (Button_locate(x, y, 45, 25))
	{
		buttoncheck = 2;
		return TRIANGLE;
	}
	else if (Button_locate(x, y, 5, 65))
	{
		buttoncheck = 3;
		return POINTS;
	}
	else if (Button_locate(x, y, 45, 65))
	{
		buttoncheck = 4;
		return CIRCLE;
	}
	else if (Button_locate(x, y, 5, 105))
	{
		buttoncheck = 5;
		return STAR;
	}
	else if (Button_locate(x, y, 45, 105))
	{
		buttoncheck = 6;
		return DRAW;
	}
	else if (Button_locate(x, y, 5, 145))
	{
		buttoncheck = 7;
		return RECTANGLE;
	}
	else if (Button_locate(x, y, 45, 145))
	{
		buttoncheck = 8;
		return ERASE;
	}
	else return draw_mode;
}
void drawInit()
{
	removeVertex(&draw->vertex);
	glClearColor(bg_r, bg_g, bg_b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawList();
	menubar();
	Functionbar();

	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_POLYGON);
	glVertex2i(80, wh - 20 - drawsize);
	glVertex2i(80, 0);
	glVertex2i(ww, 0);
	glVertex2i(ww, wh - 20 - drawsize);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2i(80 + drawsize, wh - 20);
	glVertex2i(ww, wh - 20);
	glVertex2i(ww, wh - 20 - drawsize);
	glVertex2i(80 + drawsize, wh - 20 - drawsize);
	glEnd();
}

void rever(struct object** head)
{
	bg_r = 1 - bg_r;
	bg_g = 1 - bg_g;
	bg_b = 1 - bg_b;

	struct object* p = *head;
	if (p->next != NULL)
	{
		while (p->next != tail)
		{
			p = p->next;
			p->color[0] = 1 - p->color[0];
			p->color[1] = 1 - p->color[1];
			p->color[2] = 1 - p->color[2];
		}
		glutPostRedisplay();
	}
}

void xInversion(struct object** head)
{
	struct object* p = *head;;
	if (p->next != NULL)
	{
		struct vertexNode* node;
		while (p->next != tail)
		{
			p = p->next;
			for (node = p->vertex; node != NULL; node = node->next)
			{
				node->vx = 500 - node->vx;
			}
		}
	}

}

void yInversion(struct object** head)
{
	struct object* p = *head;;
	if (p->next != NULL)
	{
		struct vertexNode* node;
		while (p->next != tail)
		{
			p = p->next;
			for (node = p->vertex; node != NULL; node = node->next)
			{
				node->vy = 500 - node->vy;
			}
		}
	}

}

void drawObj(struct object* drawnode)
{
	struct vertexNode* node = drawnode->vertex;
	glColor3f(drawnode->color[0], drawnode->color[1], drawnode->color[2]);
	if (drawnode->fill == 1) glBegin(GL_POLYGON);
	else if (drawnode->fill == 2) glBegin(GL_LINE_STRIP);
	else glBegin(GL_LINE_LOOP);
	if (draw_mode != STAR || (draw_mode == STAR && drawnode->fill == 0))
	{
		while (node->next != NULL)
		{
			node = node->next;
			glVertex2i((node->vx*drawsize) / 500 + 80, wh - (node->vy*drawsize) / 500 - 20);
		}
		glEnd();
		glutSwapBuffers();
	}
	else
	{
		while (node->next != NULL)
		{
			int count = 0;
			while (count < 3)
			{
				node = node->next;
				glVertex2i((node->vx*drawsize) / 500 + 80, wh - (node->vy*drawsize) / 500 - 20);
				count++;
			}
			glEnd();
			glBegin(GL_POLYGON);
		}
		glEnd();
		glutSwapBuffers();
	}
}

void motion(int x, int y) {
	if (80 < x && x < 80 + drawsize && 20 < y && 20 + drawsize > y)
	{
	if (draw_mode == DRAW && lineObj != NULL)
	{
		makeVertex(x, y, lineObj);
		dragCheck = 1;

		drawObj(lineObj);
	}
	else if (draw_mode == RECTANGLE && draw != NULL)
	{
		drawInit();

		makeVertex(x, y, draw);
		makeVertex(x, yp[0], draw);
		makeVertex(xp[0], yp[0], draw);
		makeVertex(xp[0], y, draw);

		drawObj(draw);
	}
	else if (draw_mode == CIRCLE && draw != NULL)
	{
		drawInit();

		double rad = sqrt(pow(x - xp[0], 2) + pow(y - yp[0], 2));
		if (rad < 0) rad = -rad;

		for (int angle = 0; angle < 360; angle += 1)
		{
			int px = rad * cos(angle*0.017);
			int py = rad * sin(angle*0.017);

			makeVertex(xp[0] + px, yp[0] + py, draw);
		}

		drawObj(draw);
	}
	else if (draw_mode == LINE && draw != NULL)
	{
		drawInit();

		makeVertex(x, y, draw);
		makeVertex(xp[0], yp[0], draw);

		drawObj(draw);
	}
	else if (draw_mode == ERASE && draw != NULL)
	{
		drawInit();

		makeVertex(x, y, draw);
		makeVertex(x, yp[0], draw);
		makeVertex(xp[0], yp[0], draw);
		makeVertex(xp[0], y, draw);

		drawObj(draw);
	}
	else if (draw_mode == TRIANGLE && draw != NULL)
	{
		drawInit();

		makeVertex(xp[0], yp[0], draw);
		makeVertex(xp[1], yp[1], draw);
		makeVertex(x, y, draw);

		drawObj(draw);
	}
	else if (draw_mode == STAR && draw != NULL)
	{
		drawInit();
		CreateStar(xp[0], yp[0], x, y, draw);
		drawObj(draw);
	}
	}
	//glutPostRedisplay();
}

void pointMove(int x, int y)
{
	if (draw_mode == DRAW && dragCheck)
	{
		addObj(lineObj, &head);
		dragCheck = 0;
		lineObj = '\0';
		if (fill == 2)
			fill = 0;
	}

	if (menubarcheck != 0)
	{
		menucheck = y / 20;
		glutPostRedisplay();
	}
	else menucheck = 0;
}

void screen_box(int x, int y, int s)
{
	glBegin(GL_POLYGON);
	glVertex2i(x, y);
	glVertex2i(x + s, y);
	glVertex2i(x + s, y + s);
	glVertex2i(x, y + s);

	glEnd();
}

void right_menu(int id)
{
	if (id == 1) exit(1);
	else if (id == 3) rever(&head);
	else clearObject();
}

void middle_menu(int id)
{

}

//ID의 값에 따라서 컬러 값을 변경
void color_menu(int id)
{
	if (id == 1) { r = 1.0; g = 0.0; b = 0.0; }
	else if (id == 2) { r = 0.0; g = 1.0; b = 0.0; }
	else if (id == 3) { r = 0.0; g = 0.0; b = 1.0; }
	else if (id == 4) { r = 0.0; g = 1.0; b = 1.0; }
	else if (id == 5) { r = 1.0; g = 0.0; b = 1.0; }
	else if (id == 6) { r = 1.0; g = 1.0; b = 0.0; }
	else if (id == 7) { r = 1.0; g = 1.0; b = 1.0; }
	else if (id == 8) { r = 0.0; g = 0.0; b = 0.0; }
}

//픽셀 값 조정
void pixel_menu(int id)
{
	if (id == 1)
		size = 2 * size;
	else if (size > 1)
		size = size / 2;
}

void fill_menu(int id)
{
	if (id == 1) fill = 1;
	else fill = 0;
}

void key(unsigned char k, int xx, int yy)
{
	if (k == 26)
	{
		int mod = glutGetModifiers();
		if (mod & GLUT_ACTIVE_SHIFT)
			Redo(&head);
		else
			undo(&head);
	}
	/*
	//text모드로 선택 시 수행된다. 
	if (draw_mode != TEXT) return;
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2i(rx, ry);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, k);
	glutStrokeCharacter(GLUT_STROKE_ROMAN,i);
	rx += glutBitmapWidth(GLUT_BITMAP_9_BY_15, k);
	*/
}

void display(void)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glClearColor(bg_r, bg_g, bg_b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	if (ww - 80  > wh - 20) drawsize = wh - 20;
	else drawsize = ww - 80;

	/*
	//line버튼 색깔, 위치 설정
	//wh-ww/10을 기본 y값으로 넣어준다. 이후 버튼들도 x값만 바꿔주고 나머지는 다 똑같이 넣는다.
	glColor3f(1.0, 1.0, 1.0);
	screen_box(0,wh-ww/10,ww/10);
	//여기까지 line

	glColor3f(1.0, 0.0, 0.0);
	screen_box(ww/10,wh-ww/10,ww/10);
	//여기까지 rect

	glColor3f(0.0, 1.0, 0.0);
	screen_box(ww/5,wh-ww/10,ww/10);
	//여기까지 tri

	glColor3f(0.0, 0.0, 1.0);
	screen_box(3*ww/10,wh-ww/10,ww/10);
	//여기까지 point

	glColor3f(1.0, 1.0, 0.0);
	screen_box(2*ww/5,wh-ww/10,ww/10);
	//여기까지 text

	//버튼은 여기서부터 추가해라
	glColor3f(0.0, 1.0, 1.0);
	screen_box(ww / 2, wh - ww / 10, ww / 10);

	glColor3f(1.0, 0.0, 1.0);
	screen_box(3 * ww / 5, wh - ww / 10, ww / 10);

	//이건 뭔지 모름
	glColor3f(0.0, 0.0, 0.0);
	screen_box(ww/10+ww/40,wh-ww/10+ww/40,ww/20);
	*/

	drawList();
	Functionbar();
	menubar(); //이렇게 해야 그림 위에 메뉴바가 그려진다.

	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_POLYGON);
	glVertex2i(80, wh - 20 - drawsize);
	glVertex2i(80, 0);
	glVertex2i(ww, 0);
	glVertex2i(ww, wh - 20 - drawsize);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2i(80 + drawsize, wh - 20);
	glVertex2i(ww, wh - 20);
	glVertex2i(ww, wh - 20 - drawsize);
	glVertex2i(80 + drawsize, wh - 20 - drawsize);
	glEnd();

	glLineWidth(1);

	//썸네일 그려준다고 생각해라
	/*
	glBegin(GL_LINES);
	glVertex2i(wh/40,wh-ww/20);
	glVertex2i(wh/40+ww/20,wh-ww/20);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2i(ww/5+ww/40,wh-ww/10+ww/40);
	glVertex2i(ww/5+ww/20,wh-ww/40);
	glVertex2i(ww/5+3*ww/40,wh-ww/10+ww/40);
	glEnd();

	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex2i(3*ww/10+ww/20, wh-ww/20);
	glEnd();
	glPointSize(1.0);

	glBegin(GL_LINE_LOOP);
	for (int angle = 0; angle < 360; angle += 1)
	{
	int px = ww/20 * cos(angle*0.017);
	int py = ww/20 * sin(angle*0.017);

	glVertex2f(11*ww/20 + px, wh - ww/20 + py);
	}
	glEnd();
	//
	glRasterPos2i(2*ww/5,wh-ww/20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
	shift=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'A');
	glRasterPos2i(2*ww/5+shift,wh-ww/20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'B');
	shift+=glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'B');
	glRasterPos2i(2*ww/5+shift,wh-ww/20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'C');

	glRasterPos2i(3 * ww / 5, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'D');
	shift = glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'D');
	glRasterPos2i((3 * ww / 5)+shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'R');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'R');
	glRasterPos2i(3 * ww / 5+shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'A');
	glRasterPos2i(3 * ww / 5+shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'W');
	*/

	glLineWidth(size);
	glutSwapBuffers();
	glPopAttrib();
}

void menubar() {
	glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_POLYGON);
	glVertex2i(0, wh);
	glVertex2i(0, wh - 20);
	glVertex2i(ww, wh - 20);
	glVertex2i(ww, wh);
	glEnd();

	if (menubarcheck != 0) {
		int x1 = -5 * pow(menubarcheck - 1, 4) + 40 * pow(menubarcheck - 1, 3) - 85 * pow(menubarcheck - 1, 2) + 95 * (menubarcheck - 1);
		int x2 = -5 * pow(menubarcheck, 4) + 40 * pow(menubarcheck, 3) - 85 * pow(menubarcheck, 2) + 95 * menubarcheck;

		int y2 = wh - 20; //y1은 wh -20이다.
		int tmp;
		if (menubarcheck == 4) tmp = 80;
		else tmp = 50 * menubarcheck*menubarcheck - 130 * menubarcheck + 120;

		//클릭시 색 바뀜
		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_POLYGON);
		glVertex2i(x1, wh);
		glVertex2i(x1, wh - 20);
		glVertex2i(x2, wh - 20);
		glVertex2i(x2, wh);
		glEnd();

		glColor3f(0.9, 0.9, 0.9); //팝업창
		y2 = wh - tmp;
		glBegin(GL_POLYGON);
		glVertex2i(x1, wh - 20);
		glVertex2i(x1, y2);
		glVertex2i(x2, y2);
		glVertex2i(x2, wh - 20);
		glEnd();

		//선택된 팝업창 내용
		if (0 < menucheck&&menucheck <= tmp / 20 - 1) {
			glColor3f(0.0, 0.0, 1.0);
			glBegin(GL_POLYGON);
			glVertex2i(x1, wh - menucheck * 20);
			glVertex2i(x1, wh - (menucheck + 1) * 20);
			glVertex2i(x2, wh - (menucheck + 1) * 20);
			glVertex2i(x2, wh - menucheck * 20);
			glEnd();
		}

		//팝업창 내용
		glColor3f(0.0, 0.0, 0.0);
		if (menubarcheck == 1)
		{
			StringText("Quit", 5, wh - 35);
		}
		else if (menubarcheck == 2)
		{
			StringText("On", 50, wh - 35);
			StringText("Off", 50, wh - 55);
		}
		else if (menubarcheck == 3)
		{
			StringText("Red", 95, wh - 35);
			StringText("Green", 95, wh - 55);
			StringText("Blue", 95, wh - 75);
			StringText("Cyan", 95, wh - 95);
			StringText("Magenta", 95, wh - 115);
			StringText("Yellow", 95, wh - 135);
			StringText("White", 95, wh - 155);
			StringText("Black", 95, wh - 175);

		}
		else if (menubarcheck == 4)
		{
			StringText("Color", 205, wh - 35);
			StringText("X-axis", 205, wh - 55);
			StringText("Y-axis", 205, wh - 75);
		}

		glBegin(GL_LINE_LOOP);//테두리
		glVertex2i(x1, wh - 20);
		glVertex2i(x1, y2);
		glVertex2i(x2, y2);
		glVertex2i(x2, wh - 20);
		glEnd();
	}

	glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_LINES);
	glVertex2i(0, wh - 20);
	glVertex2i(ww, wh - 20);
	glEnd();


	glBegin(GL_LINES); // 메뉴바 나누기
	glVertex2i(45, wh - 4);
	glVertex2i(45, wh - 16);

	glVertex2i(90, wh - 4);
	glVertex2i(90, wh - 16);

	glVertex2i(195, wh - 4);
	glVertex2i(195, wh - 15);

	glVertex2i(300, wh - 4);
	glVertex2i(300, wh - 15);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	StringText("File", 5, wh - 15);
	StringText("Fill", 50, wh - 15);
	StringText("BackGround", 95, wh - 15);
	StringText("Inversion", 200, wh - 15);
}

boolean menubarFuction(int x, int y)
{
	if (menubarcheck != 0) // 메뉴바 행동 설정
	{
		if (menubarcheck == 1) //File을 선택했을 때
		{
			if (menucheck == 1) //Quit 선택
			{
				exit(0);
			}
			menubarcheck = 0;
		}
		else if (menubarcheck == 2) //Fill을 선택했을 떄
		{
			if (menucheck == 1) fill = 1;
			else if (menucheck == 2) fill = 0;
			menubarcheck = 0;
		}
		else if (menubarcheck == 3) //BackGround를 선택했을때
		{
			if (menucheck == 1) { bg_r = 1.0; bg_g = 0.0; bg_b = 0.0; }
			else if (menucheck == 2) { bg_r = 0.0; bg_g = 1.0; bg_b = 0.0; }
			else if (menucheck == 3) { bg_r = 0.0; bg_g = 0.0; bg_b = 1.0; }
			else if (menucheck == 4) { bg_r = 0.0; bg_g = 1.0; bg_b = 1.0; }
			else if (menucheck == 5) { bg_r = 1.0; bg_g = 0.0; bg_b = 1.0; }
			else if (menucheck == 6) { bg_r = 1.0; bg_g = 1.0; bg_b = 0.0; }
			else if (menucheck == 7) { bg_r = 1.0; bg_g = 1.0; bg_b = 1.0; }
			else if (menucheck == 8) { bg_r = 0.0; bg_g = 0.0; bg_b = 0.0; }
			menubarcheck = 0;
		}
		else if (menubarcheck == 4) //Reverse를 선택시
		{
			if (menucheck == 1) rever(&head);
			else if (menucheck == 2) yInversion(&head);
			else if (menucheck == 3) xInversion(&head);
			menubarcheck = 0;
		}
		return false1;
	}
	else if (x > 5 && x < 75 && y > 185 && y < 265)
	{
		getRGB(x - 5, y - 185);
		return false1;
	}
	if (y <= 20)
	{
		if (x <= 45)	menubarcheck = 1;	//File 선택
		else if (x <= 90) menubarcheck = 2; //Fill 선택
		else if (x <= 195) menubarcheck = 3; //BG선택
		else if (x <= 295) menubarcheck = 4;
		else	menubarcheck = 0;
	}
	else menubarcheck = 0;


	return true1;
}
void Functionbar()
{
	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_POLYGON); //기능 아이콘들이 들어갈 곳
	glVertex2i(0, wh - 20);
	glVertex2i(0, 0);
	glVertex2i(80, 0);
	glVertex2i(80, wh - 20);
	glEnd();

	Button(5, wh - 25);
	Button(45, wh - 25);
	Button(5, wh - 65);
	Button(45, wh - 65);
	Button(5, wh - 105);
	Button(45, wh - 105);
	Button(5, wh - 145);
	Button(45, wh - 145);
	//클릭한 버튼 효과

	if (buttoncheck != 0)
	{
		int x;
		int y;

		if (buttoncheck % 2 == 0)
		{
			x = 45;
			y = 25 + 40 * (buttoncheck / 2 - 1);
		}
		else
		{
			x = 5;
			y = 25 + 40 * (buttoncheck / 2);
		}

		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_POLYGON);
		glVertex2i(x, wh - y);
		glVertex2i(x, wh - (y + 30));
		glVertex2i(x + 30, wh - (y + 30));
		glVertex2i(x + 30, wh - y);
		glEnd();

		glColor3f(0.7, 0.7, 0.7);
		glBegin(GL_POLYGON);
		glVertex2i(x, wh - y);
		glVertex2i(x, wh - (y + 30));
		glVertex2i(x + 2, wh - (y + 30));
		glVertex2i(x + 2, wh - y);
		glEnd();

		glBegin(GL_POLYGON);
		glVertex2i(x + 2, wh - y);
		glVertex2i(x + 30, wh - y);
		glVertex2i(x + 30, wh - (y + 2));
		glVertex2i(x + 2, wh - (y + 2));
		glEnd();
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_STRIP);
		glVertex2i(x, wh - (y + 30));
		glVertex2i(x + 30, wh - (y + 30));
		glVertex2i(x + 30, wh - y);
		glEnd();
	}
	//썸네일 그려 주는 곳
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(11, wh - 49);
	glVertex2i(29, wh - 31);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2i(60, wh - 31);
	glVertex2i(51, wh - 49);
	glVertex2i(69, wh - 49);
	glEnd();

	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex2i(20, wh - 80);
	glEnd();
	glPointSize(1.0);

	glBegin(GL_LINE_LOOP);
	for (int angle = 0; angle < 360; angle += 1)
	{
		int px = 10 * cos(angle*0.017);
		int py = 10 * sin(angle*0.017);

		glVertex2f(60 + px, wh - 80 + py);
	}
	glEnd();

	int x1 = 10;
	int x2 = 30;
	int y1 = 110;
	int y2 = 130;

	glBegin(GL_LINE_LOOP);
		glVertex2i(0.5*20 + x1, wh -y1);					//1
		glVertex2i(0.38*20 + x1, wh - 0.38*20 - y1);	//2
		glVertex2i(x1, wh - 0.4*20 - y1);					//3
		glVertex2i(0.3*20 + x1, wh - 0.62*20 - y1);		//4
		glVertex2i(0.19*20 + x1, wh - y2);					//5
		glVertex2i(0.5*20 + x1, wh -0.77*20 - y1);		//6
		glVertex2i(0.81*20 + x1, wh -y2);					//7
		glVertex2i(0.7*20 + x1, wh -0.62*20 - y1);		//8
		glVertex2i(x2, wh - 0.4*20 - y1);					//9
		glVertex2i(0.62*20 + x1, wh - 0.38*20 - y1);	//10
	glEnd();

	glRasterPos2i(55, wh - 125);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'D');

	glBegin(GL_LINE_LOOP);
	glVertex2i(10, wh - 150);
	glVertex2i(10, wh - 170);
	glVertex2i(30, wh - 170);
	glVertex2i(30, wh - 150);
	glEnd();

	eraser_thumnail();

	glBegin(GL_POLYGON); //색상창
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(5, wh - 185);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(5, wh - 255);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2i(75, wh - 255);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(75, wh - 185);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP); //색상창 테두리
	glVertex2i(5, wh - 185);
	glVertex2i(5, wh - 255);
	glVertex2i(75, wh - 255);
	glVertex2i(75, wh - 185);
	glEnd();

	glColor3f(r, g, b);
	glBegin(GL_POLYGON); //현재색상창
	glVertex2i(20, wh - 265);
	glVertex2i(20, wh - 305);
	glVertex2i(60, wh - 305);
	glVertex2i(60, wh - 265);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP); //현재 색상창 테두리
	glVertex2i(20, wh - 265);
	glVertex2i(20, wh - 305);
	glVertex2i(60, wh - 305);
	glVertex2i(60, wh - 265);
	glEnd();

	//경계선 표시
	glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_LINES);
	glVertex2i(80, wh - 20);
	glVertex2i(80, 0);
	glEnd();

}

void Button(int x, int y) //색은 어쩌피 같으니 음영으로 버튼처럼 표시만 한다.
{
	glColor3f(1.0, 1.0, 1.0); //빛 표시
	glBegin(GL_LINE_STRIP);
	glVertex2i(x + 1, y - 30);
	glVertex2i(x + 1, y - 1);
	glVertex2i(x + 30, y - 1);
	glEnd();

	glColor3f(0.6, 0.6, 0.6);

	glBegin(GL_POLYGON); //그림자 표시
	glVertex2i(x, y - 28);
	glVertex2i(x, y - 30);
	glVertex2i(x + 30, y - 30);
	glVertex2i(x + 30, y - 28);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2i(x + 28, y);
	glVertex2i(x + 30, y);
	glVertex2i(x + 30, y - 28);
	glVertex2i(x + 28, y - 28);
	glEnd();
}

boolean Button_locate(int x, int y, int button_x, int button_y)
{
	if (x > button_x + 30) return false1;
	if (x < button_x) return false1;

	if (y < button_y) return false1;
	if (y > button_y + 30) return false1;
	return true1;
}

void Button_click(int x)
{
	buttoncheck = x;
	glutPostRedisplay();
}

void getRGB(int x, int y)
{
	float RGBx = (float)x / 70;
	float RGBy = (float)y / 70;
	if(RGBx <= RGBy)
	{
		if(RGBx < 1 - RGBy)
		{
			r = (1 - RGBx)*(1 - RGBy);
			g = (1 - RGBx)*RGBy;
			b = 0;
		}
		else
		{
			r = 0;
			g = (1 - RGBx)*RGBy;
			b = RGBx*RGBy;

		}
	}
	else
	{
		r = (1 - RGBx)*(1 - RGBy);
		g = 0;
		b = RGBx*RGBy;
	
	}
	printf("R : %f G : %f B : %f\n", r, g, b);
	glutPostRedisplay();
}

void StringText(char String[], int x, int y) {
	int shift = x;
	for (int i = 0; String[i] != NULL; i++) {
		glRasterPos2i(shift, y);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, String[i]);
		shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, String[i]);
	}
}

void eraser_thumnail()
{
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	for (int angle = 0; angle < 180; angle += 1)
	{
		int px = 10 * cos(angle*0.017);
		int py = 10 * sin(angle*0.017);

		glVertex2f(60 + px, wh - 157 + py);
	}
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	for (int angle = 0; angle < 178; angle += 1)
	{
		int px = 10 * cos(angle*0.017);
		int py = 10 * sin(angle*0.017);

		glVertex2f(60 + px, wh - 157 + py);
	} //지우개 반원
	glEnd();

	glColor3f(0.4, 0.3, 0.7);
	glBegin(GL_POLYGON);
	glVertex2i(51, wh - 157);
	glVertex2i(69, wh - 157);
	glVertex2i(69, wh - 170);
	glVertex2i(51, wh - 170);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(51, wh - 157);
	glVertex2i(69, wh - 157);
	glVertex2i(69, wh - 170);
	glVertex2i(51, wh - 170);
	glEnd();
}


int main(int argc, char** argv)
{
	int c_menu, p_menu, f_menu;
	head = (struct object*)malloc(sizeof(struct object));
	initObj(head);
	tail = (struct object*)malloc(sizeof(struct object));
	initObj(tail);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("square");
	glutDisplayFunc(display);

	//c_menu는 color 메뉴다. 밑으로는 항목 추가
	c_menu = glutCreateMenu(color_menu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Cyan", 4);
	glutAddMenuEntry("Magenta", 5);
	glutAddMenuEntry("Yellow", 6);
	glutAddMenuEntry("White", 7);
	glutAddMenuEntry("Black", 8);

	//p_menu를 만든다. pixel의 크기를 변경 한다.
	p_menu = glutCreateMenu(pixel_menu);
	glutAddMenuEntry("increase pixel size", 1);
	glutAddMenuEntry("decrease pixel size", 2);

	//f_menu는 fill의 on off를 결정한다. on이면 도형의 내부가 채워진다. -> polygon으로 되는듯 하다 off일 땐 GL_LINE_LOOP
	f_menu = glutCreateMenu(fill_menu);
	glutAddMenuEntry("fill on", 1);
	glutAddMenuEntry("fill off", 2);
	glutCreateMenu(right_menu);
	glutAddMenuEntry("quit", 1);
	glutAddMenuEntry("clear", 2);
	glutAddMenuEntry("rever", 3);

	//rigth_menu를 오른쪽 버튼에 붙인다. 종료와 CLEAR가 있다.
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//middle_menu에 c_menu와 p_menu, f_menu를 붙인다. 이후 휠버튼에 부착된다.
	glutCreateMenu(middle_menu);
	glutAddSubMenu("Colors", c_menu);
	glutAddSubMenu("Pixel Size", p_menu);
	glutAddSubMenu("Fill", f_menu);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);

	//초기화 작업
	myinit();

	//이벤트 붙여줌
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(pointMove);

	glutMainLoop();
}
