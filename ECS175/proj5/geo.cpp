#include <GL/glut.h>

GLint carSide;
GLint carTop;
GLint car;

void modelCar()
{
	//FAR SIDE OF CAR
	carSide = glGenLists(1);
	glNewList(carSide, GL_COMPILE);
	//BODY OF CAR
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(-3.0, 3.0, -2.0);
	glVertex3f(3.0, 3.0, -2.0);
	glVertex3f(3.0, 0.0, -2.0);
	glVertex3f(-3.0, 0.0, -2.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(7.0, 0.0, -2.0);
	glVertex3f(7.0, -3.0, -2.0);
	glVertex3f(-7.0, -3.0, -2.0);
	glVertex3f(-7.0, 0.0, -2.0);
	glEnd();
	//WINDSHIELD AND WINDOWS
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(-5.0, 0.0, -2.0);
	glVertex3f(-3.0, 0.0, -2.0);
	glVertex3f(-3.0, 3.0, -2.0);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex3f(5.0, 0.0, -2.0);
	glVertex3f(3.0, 0.0, -2.0);
	glVertex3f(3.0, 3.0, -2.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-0.2, 0.0, -2.01);
	glVertex3f(-2.8, 0.0, -2.01);
	glVertex3f(-2.8, 2.8, -2.01);
	glVertex3f(-0.2, 2.8, -2.01);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.2, 0.0, -2.01);
	glVertex3f(2.8, 0.0, -2.01);
	glVertex3f(2.8, 2.8, -2.01);
	glVertex3f(0.2, 2.8, -2.01);
	glEnd();
	glEndList();

	carTop = glGenLists(1);
	glNewList(carTop, GL_COMPILE);
	//CAR BODY
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(-7.0, 0.0, 2.0);
	glVertex3f(-7.0, 0.0, -2.0);
	glVertex3f(-7.0, -3.0, -2.0);
	glVertex3f(-7.0, -3.0, 2.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(7.0, 0.0, 2.0);
	glVertex3f(7.0, 0.0, -2.0);
	glVertex3f(7.0, -3.0, -2.0);
	glVertex3f(7.0, -3.0, 2.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-7.0, 0.0, -2.0);
	glVertex3f(-7.0, 0.0, 2.0);
	glVertex3f(-5.0, 0.0, 2.0);
	glVertex3f(-5.0, 0.0, -2.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(7.0, 0.0, -2.0);
	glVertex3f(7.0, 0.0, 2.0);
	glVertex3f(5.0, 0.0, 2.0);
	glVertex3f(5.0, 0.0, -2.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-3.0, 3.0, -2.0);
	glVertex3f(-3.0, 3.0, 2.0);
	glVertex3f(3.0, 3.0, 2.0);
	glVertex3f(3.0, 3.0, -2.0);
	glEnd();
	//WINDSHIELD
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(-5.0, 0.0, 2.0);
	glVertex3f(-3.0, 3.0, 2.0);
	glVertex3f(-3.0, 3.0, -2.0);
	glVertex3f(-5.0, 0.0, -2.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(5.0, 0.0, 2.0);
	glVertex3f(3.0, 3.0, 2.0);
	glVertex3f(3.0, 3.0, -2.0);
	glVertex3f(5.0, 0.0, -2.0);
	glEnd();
	glEndList();

	car = glGenLists(1);
	glNewList(car, GL_COMPILE);
	glCallList(carSide);
	glCallList(carTop);
	glPushMatrix();
	glScalef(1, 1, -1);
	glCallList(carSide);
	glPopMatrix();
	glEndList();
}