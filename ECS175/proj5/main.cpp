#include <GL/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

#define PI 3.14
#define twicePI 2*PI

GLint winWidth = 600, winHeight = 600; // Initial display-window size

GLfloat angle;
GLfloat carX = 0;

//declare our model lists!
GLint tire;
GLint rim;
GLint carSide;
GLint carTop;
GLint car;
GLint tireLeft, tireRight;
GLint allTires;
GLint env;

//function definitions
void modelEnv();
void modelRims();
void modelTires();
void modelBody();
void modelCar();
void f(int a);

void init()
{
	glClearColor(0.0, 0.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST); //need to add so z-values are read correctly
	//glShadeModel(GL_SMOOTH);
}

void display()
{
	//depth buffer for proper representation
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glLoadIdentity(); //clear the matrix

	
	modelCar(); //called here to keep rotation

	//viewing transformation, set to follow the car
	gluLookAt(carX + 4.0, 5.0, 15.0, carX, 0, 0, 0.0, 1.0, 0.0);

	//create the environment
	glCallList(env);

	//car transformation
	glPushMatrix();
		glTranslatef(carX, 0.0, 0.0);
		glCallList(car);
	glPopMatrix();

	//because we are using GLUT_DOUBLE
	glutSwapBuffers();
	glFlush();
}

void reshape(int newWidth, int newHeight)
{
	//set viewport to new height/width
	glViewport(0, 0, newWidth, newHeight);
	
	//set to projection mode for Frustum function
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 100.0);

	//set back to model view for lookat function to work later
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Moving Car");

	modelBody();
	modelRims();
	modelTires();
	modelEnv();
	init();
	glutDisplayFunc(display);
	glutTimerFunc(25, f, 0); //timer function, in milliseconds
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

void modelEnv()
{
	env = glGenLists(1);
	glNewList(env, GL_COMPILE);
		//rightGreen
		glColor3f(0, 1, 0);
		glBegin(GL_QUADS);
			glVertex3f(-1000, -4.2, -100.0);
			glVertex3f(-1000, -4.2, -5.0);
			glVertex3f(100, -4.2, -5.0);
			glVertex3f(100, -4.2, -100.0);
		glEnd();
		//Road
		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_QUADS);
			glVertex3f(-1000, -4.2, -5.0);
			glVertex3f(-1000, -4.2, 20.0);
			glVertex3f(100, -4.2, 20.0);
			glVertex3f(100, -4.2, -5.0);
		glEnd();
		//yellow dashed line
		glColor3f(1, 1, 0);
		for (int i = -100; i <= 1000; i += 4)
		{
			cout << i << endl;
			glBegin(GL_QUADS);
				glVertex3f(-i, -4, 5);
				glVertex3f(-i, -4, 4.5);
				glVertex3f(-i - 1, -4, 4.5);
				glVertex3f(-i - 1, -4, 5);
			glEnd();
		}
		//model an easy tree, with triangle top
		
		for (int i = -100; i <= 1000; i += 25)
		{
			cout << i << endl;
			glColor3f(0.4, 0.2, 0);
			glBegin(GL_QUADS);
				glVertex3f(-i + 1, -4, -20);
				glVertex3f(-i + 1, 4, -20);
				glVertex3f(-i - 1, 4, -20);
				glVertex3f(-i - 1, -4, -20);
			glEnd();
			glColor3f(0, 0.5, 0);
			glBegin(GL_TRIANGLES);
				glVertex3f(-i, 12, -19.9);
				glVertex3f(-i - 3, 2, -19.9);
				glVertex3f(-i + 3, 2, -19.9);
			glEnd();
		}

	glEndList();
}

void modelRims()
{
	//variables for easy editing
	GLfloat x = 0;
	GLfloat y = 0;
	GLint numTriangles = 14;
	GLfloat radius = 1;

	rim = glGenLists(1);
	glNewList(rim, GL_COMPILE);
		glBegin(GL_TRIANGLE_FAN);
		//glTexCoord2f(0.0, 0.0); 
		glColor3f(1, 1, 1);
		glVertex2f(x, y);
			for (int i = 0; i <= numTriangles + 1; i++)
			{
				if (i % 2)
					glColor3f(1, 1, 1);
				else
					glColor3f(0.2, 0.2, 0.2);
				glVertex2f(
					x + (radius * cos(i * twicePI / numTriangles)),
					y + (radius * sin(i * twicePI / numTriangles))
					);
			}
		glEnd();
	glEndList();
}

void modelTires()
{
	tire = glGenLists(1);
	glNewList(tire, GL_COMPILE);
	glColor3f(0.1, 0.1, 0.1);
	glutSolidTorus(0.3, 1.0, 360, 360);
	glEndList();
}

void modelBody()
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
}

void modelCar()
{
	//TIRE RIGHT
	tireRight = glGenLists(1);
	glNewList(tireRight, GL_COMPILE);
		glPushMatrix();
			glRotatef(angle, 0.0, 0.0, 1.0);
			glCallList(tire);
			glTranslatef(0, 0, -0.01);
			glCallList(rim);
		glPopMatrix();
	glEndList();

	//TIRE LEFT
	tireLeft = glGenLists(1);
	glNewList(tireLeft, GL_COMPILE);
		glPushMatrix();
			glRotatef(angle, 0.0, 0.0, 1.0);
			glCallList(tire);
			glTranslatef(0, 0, 0.01);
			glCallList(rim);
		glPopMatrix();
	glEndList();

	car = glGenLists(1);
	glNewList(car, GL_COMPILE);
		//BODY OF CAR
		glCallList(carSide);
		glCallList(carTop);
		glPushMatrix();
			glScalef(1, 1, -1);
			glCallList(carSide);
		glPopMatrix();

		//TIRES WITH ANIMATION
		//TIRE FRONT RIGHT
		glPushMatrix();
			glTranslatef(-4, -3, -2);
			glCallList(tireRight);
		glPopMatrix();
		//TIRE BACK RIGHT
		glPushMatrix();
			glTranslatef(4, -3, -2);
			glCallList(tireRight);
		glPopMatrix();
		//TIRE FRONT LEFT
		glPushMatrix();
			glTranslatef(-4, -3, 2);
			glCallList(tireLeft);
		glPopMatrix();
		//TIRE BACK LEFT
		glPushMatrix();
			glTranslatef(4, -3, 2);
			glCallList(tireLeft);
		glPopMatrix();

	glEndList();
}

void f(int a)
{
	//what's happening here? 
	glutTimerFunc(25, f, 0);

	//cout << "idle function";
	//a simple rotation angle increment routine
	angle += 2.3;
	if (angle > 360)
	{
		angle -= 360;
	}

	carX -= 0.2;
	if (carX < -1000)
	{
		carX += 1000;
	}

	glutPostRedisplay();
}