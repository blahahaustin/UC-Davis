#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

#define FLOATERROR 0.000001

/*Class Definitions-----------------------------------------------------*/

class point
{
public:
	GLfloat x, y, z;
};

class boundBox
{
public:
	// Initialize to opposite extreme values for comparison purposes.
	GLdouble xMin = 200.0, xMax = -200, yMin = 200, yMax = -200;
	GLdouble area;
	vector<point> control;
};

/*----------------------------------------------------------------------*/

/*Variables-------------------------------------------------------------*/

/* Set integer variables for the windows. */
int MainWindow;

/* Set initial size of the display window. */
GLsizei winWidth = 600, winHeight = 600;

/* Set size of the world-coordinate clipping window. */
GLfloat xwcMin = -200.0, xwcMax = 200.0;
GLfloat ywcMin = -200.0, ywcMax = 200.0;

/* Set the mode of the program based on keyboard press. */
int mode = 1;
int curve = 1;

/* Create a vector of control points that is easy to add to. */
vector<point> ctrlPts;
vector<point> ctrlPts2;
vector<point> ctrlLeft1;
vector<point> ctrlRight1;
vector<point> ctrlLeft2;
vector<point> ctrlRight2;
boundBox B1a, B1b, B2a, B2b;

/* A variable so that we will execute the current algorithm. */
bool execute = false;

/* Set the t variable globally so that we can edit it. */
float t = 0.5;

/* Variables for selection of control points. */
const GLint pickBuffSize = 32;
GLint selectedPt;

/* Variables for clock times. */
clock_t bez;
clock_t deCast;

/*----------------------------------------------------------------------*/

/*Function Declarations-------------------------------------------------*/

void init(void);
void displayFcn(void);
void winReshapeFcn(GLint, GLint);
void keyboardFcn(unsigned char, int, int);
void specialFcn(int, int, int);
void mouseFcn(int, int, int, int);
point GetOGLPos(int, int);
void plotPoint(point);
void renderString(float, float, const char*);
void deCasteljau(vector<point>, float);
point interpolate(point, point, float);
void bezier(point*, GLint, GLint);
void computeBezPt(GLfloat, point*, GLint, point*, GLint*);
void binomialCoeffs(GLint, GLint*);
void drawControls(GLenum mode);
void processPicks(GLint, GLuint pickBuffer[]);
void subdivide(vector<point> p, float myT);
void intersect(vector<point> p1, vector<point> p2);
boundBox computeBound(vector<point> p);
void split1(vector<point>);
void split2(vector<point>);
bool overlap(boundBox a, boundBox b);
double area(boundBox box);
void reset();

/*----------------------------------------------------------------------*/

/* Performs any initial actions we need for the program. */
void init(void)
{
	/* Set color of display window to white. */
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

/* Callback function for display, is called over and over again,
we will use to change between modes and call appropriate functions. */
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT); //Clear display window.

		drawControls(GL_RENDER);
	
		/* If execute is true, do the de Casteljau algorithm for the given points!*/
	if (execute && mode == 1 && ctrlPts.size() > 1)
	{
		deCast = clock();
		/* Call the de Casteljau algorithm 1000 times. */
		for (double i = 0.0; i <= 1.0; i = i + 0.001)
		{
			//cout << "i: " << i << endl;
			deCasteljau(ctrlPts, i);
		}
		deCast = clock() - deCast;
		string time1 = to_string(deCast);
		renderString(-180, -160, time1.c_str());
	}
	else if (execute && mode == 2 && ctrlPts.size() > 1)
	{
		bez = clock();
		bezier(ctrlPts.data(), ctrlPts.size(), 1000);
		bez = clock() - bez;
		string time2 = to_string(bez);
		renderString(-180, -160, time2.c_str());
	}
	else if (execute && mode == 3 && ctrlPts.size() > 1)
	{
		bezier(ctrlPts.data(), ctrlPts.size(), 1000);
		subdivide(ctrlPts, t);
	}
	else if (execute && mode == 4)
	{
		if (ctrlPts.size() > 1)
		{
				bezier(ctrlPts.data(), ctrlPts.size(), 1000);
		}
		if (ctrlPts2.size() > 1)
		{
				bezier(ctrlPts2.data(), ctrlPts2.size(), 1000);
		}
		if (ctrlPts.size() > 1 && ctrlPts2.size())
		{
			intersect(ctrlPts, ctrlPts2);
		}
	}
	if (ctrlPts.size() == 1)
	{
		glPointSize(5);
		glColor3f(0.0, 0.0, 0.0);
		plotPoint(ctrlPts[0]);
	}
	if (ctrlPts2.size() == 1)
	{
		glPointSize(5);
		glColor3f(0.0, 0.0, 0.0);
		plotPoint(ctrlPts2[0]);
	}

	glFlush();
}

/* Keeps the window ratio to 1.0 and sets clipping window. */
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	/* Maintain an aspect ratio of 1.0. */
	glViewport(0, 0, newHeight, newHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* Set a 2d orthographic viewpoint clipped by our 200x200 bounds. */
	gluOrtho2D(xwcMin, xwcMax, ywcMin, ywcMax);

	glClear(GL_COLOR_BUFFER_BIT);
}

/* Keyboard callback function. */
void keyboardFcn(unsigned char key, int x, int y)
{
	/* Changes between parts a, b, c, and d of the project. */
	switch (key)
	{
	case '1': mode = 1; cout << "Mode is now 1." << endl; glutPostRedisplay(); break;
	case '2': mode = 2; cout << "Mode is now 2." << endl; glutPostRedisplay(); break;
	case '3': mode = 3; cout << "Mode is now 3." << endl; glutPostRedisplay(); break;
	case '4': mode = 4; cout << "Mode is now 4." << endl; glutPostRedisplay(); break;
	}

	/* Press enter to draw the picture and again to reset it. */
	if (key == ' ' && execute == false)
	{
		cout << "EXECUTE" << endl;
		execute = true;
		glutPostRedisplay();
	}
	else if (key == ' ' && execute == true)
	{
		ctrlPts.clear();
		ctrlPts2.clear();
		t = 0.5;
		execute = false;
		glutPostRedisplay();
	}
}

/* Special keyboard character callback function, to handle arrow keys. */
void specialFcn(int key, int x, int y)
{
	if (t < 0.99 && t > 0.0)
	{
		switch (key)
		{
			case GLUT_KEY_DOWN: t = t + 0.01; cout << "t = " << t << endl; glutPostRedisplay(); break;
			case GLUT_KEY_UP: t = t - 0.01; cout << "t = " << t << endl; glutPostRedisplay(); break;
			case GLUT_KEY_F1: curve = 1; cout << "curve = 1" << endl; glutPostRedisplay(); break;
			case GLUT_KEY_F2: curve = 2; cout << "curve = 2" << endl; glutPostRedisplay(); break;
		}
	}
}

/* Mouse callback function, handles screen clicks from the user. */
void mouseFcn(int button, int state, int xMouse, int yMouse)
{
	GLuint pickBuffer[pickBuffSize];
	GLint nPicks, vpArray[4];

	/* If statement to handle moving points. */
	if ((button == GLUT_MIDDLE_BUTTON || button == GLUT_RIGHT_BUTTON) && state == GLUT_DOWN)
	{
		glSelectBuffer(pickBuffSize, pickBuffer); //designate pick buffer
		glRenderMode(GL_SELECT);
		glInitNames();
		
		/* Save current viewing matrix */
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		
		/* Obtain the parameters for the current viewport. Set up
		 * a 5 x 5 pick window, and invert the input yMouse value
		 * using the height of the viewPort, which is the fourth
		 * element of vpArray. */
		glGetIntegerv(GL_VIEWPORT, vpArray);
		gluPickMatrix(GLdouble (xMouse), GLdouble(vpArray[3] - yMouse), 5.0, 5.0, vpArray);
		gluOrtho2D (xwcMin, xwcMax, ywcMin, ywcMax);
		drawControls(GL_SELECT);
		
		/*Restore original viewing matrix */
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		
		glFlush();
		
		/* Determine the number of picked objects and return to the 
		 * normal rendering mode. */
		nPicks = glRenderMode(GL_RENDER);
		
		processPicks(nPicks, pickBuffer); //process picked objects

		glutPostRedisplay();
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
	{
		/* The unwanted case where nothing is selected. */
		if (selectedPt < 1)
			return;

		/* Create new point at the drag-dropped position. */
		point newPoint = GetOGLPos(xMouse, yMouse);

		/* Update the vector of control points with the new spot. */

		//cout << "ctrlPts.size() = " << ctrlPts.size() << endl;
		//cout << "The selected point is: " << selectedPt << endl;

		if (selectedPt <= ctrlPts.size())
		{
			//cout << "CURVE 1" << endl;
			ctrlPts[selectedPt - 1] = newPoint;
		}
		else
		{
			//cout << "CURVE 2" << endl;
			ctrlPts2[selectedPt - ctrlPts.size() - 1] = newPoint;
		}
			
		/* Reset the selected point! */
		selectedPt = 0;

		glutPostRedisplay();
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		/* The unwanted case where nothing is selected. */
		if (selectedPt < 1)
			return;

		/* Delete the point! */
		int pos = selectedPt - 1;
		ctrlPts.erase(ctrlPts.begin() + pos);

		/* Reset the selected point! */
		selectedPt = 0;

		glutPostRedisplay();
	}

	if ( curve == 1)
	{
		/* If the left button is clicked. */
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			//cout << "Add to curve 1" << endl;
			/* Convert clicked point to world coordinates. */
			point clicked = GetOGLPos(xMouse, yMouse);

			/* Push points into a vector of control points. */
			ctrlPts.push_back(clicked);

			/* Update the display. */
			glutPostRedisplay();
		}
	}
	else if ( curve == 2)
	{
		/* If the left button is clicked. */
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			//cout << "Add to curve 2" << endl;

			/* Convert clicked point to world coordinates. */
			point clicked = GetOGLPos(xMouse, yMouse);

			/* Push points into a vector of control points. */
			ctrlPts2.push_back(clicked);

			/* Update the display. */
			glutPostRedisplay();
		}
	}
}

void processPicks(GLint nPicks, GLuint pickBuffer[])
{
	GLuint objID, *ptr;

	//cout << "Number of objects picked = " << nPicks << endl << endl;
	ptr = pickBuffer;

	//find the selected point and store it globally
	if (nPicks > 0)
	{
		objID = *ptr;
		selectedPt = objID;
	}
}

void drawControls(GLenum mode)
{
	/* Print out the control points specified by the user. */
	for (unsigned int i = 0; i < ctrlPts.size(); i++)
	{
		/* If in select mode, give the point a name! */
		if (mode == GL_SELECT)
			glPushName(i);
		//cout << " CURVE 1 glPushName(" << i << ")" << endl;
		glPointSize(5);
		glColor3f(0.0, 0.0, 0.0);
		plotPoint(ctrlPts[i]);
	}

	for (unsigned int i = 0; i < ctrlPts2.size(); i++)
	{
		/* If in select mode, give the point a name! */
		if (mode == GL_SELECT)
			glPushName(i + ctrlPts.size());
		//cout << "CURVE 2 glPushName(" << i + ctrlPts.size() << ")" << endl;
		glPointSize(5);
		glColor3f(0.0, 0.0, 0.0);
		plotPoint(ctrlPts2[i]);
	}
}

/* Function to convert screen coordinates into world coordinates. */
point GetOGLPos(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	point newPoint = { posX, posY, posZ };

	return newPoint;
}

/* A function to plot points. */
void plotPoint(point pt)
{
	glBegin(GL_POINTS);
	glVertex2f(pt.x, pt.y);
	glEnd();
}

/* A function to render a string. */
void renderString(float x, float y, const char* string)
{
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2f(x, y);

	glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)string);
}

/* A function that performs the de Casteljau algorithm. */
void deCasteljau(vector<point> p, float myT)
{
	/* Create a temporary vector that we will use to recurse. */
	vector<point> temp;

	for (unsigned int i = 0; i < p.size() - 1; i++)
	{
		/* Call the interpolate function and push that new point to our temp vector. */
		point newPoint = interpolate(p[i], p[i + 1], myT);
		temp.push_back(newPoint);

		/* Draw the interpolated points for the user's T. */
		if ((myT - t) < FLOATERROR && (t - myT) < FLOATERROR)
		{
			glPointSize(3.0);
			glColor3f(0.0, 0.0, 0.0);
			plotPoint(newPoint);
			
			/* Draw a line in between the two points being processed. */
			//glLineWidth(1.0);
			//glColor3f(0.0, 0.0, 1.0);
			glPushMatrix();
			glBegin(GL_LINES);
				glVertex3f(p[i].x, p[i].y, 0.0);
				glVertex3f(p[i + 1].x, p[i + 1].y, 0.0);
			glEnd();
			glPopMatrix();
		}

		/* Draw the terminating point in the algorithm! */
		if (p.size() == 2)
		{
			glPointSize(3.0);
			glColor3f(1.0, 0.0, 0.0);
			plotPoint(newPoint);
		}
	}
	
	/* Make the recursive call. */
	if (temp.size() != 1)
	{
		deCasteljau(temp, myT);
	}
}

point interpolate(point p0, point p1, float myT)
{
	point newPoint;
	
	/* Apply interpolation formula to each member of the point. */
	newPoint.x = (1.0 - myT) * p0.x + (myT * p1.x);
	newPoint.y = (1.0 - myT) * p0.y + (myT * p1.y);
	
	//cout << "The interpolated point is " << newPoint.x << ", " << newPoint.y << endl;

	return newPoint;
}

/* Compute binomial coefficients C for given value of n. */
void binomialCoeffs(GLint n, GLint *C)
{
	GLint k, j;

	for (k = 0; k <= n; k++)
	{
		/* compute n!/(k!(n - k)!). */
		C[k] = 1;
		for (j = n; j >= k + 1; j--)
			C[k] *= j;
		for (j = n - k; j >= 2; j--)
			C[k] /= j;
	}
}

void computeBezPt(GLfloat u, point* bezPt, GLint nCtrlPts, point* ctrlPts, GLint* C)
{
	GLint k, n = nCtrlPts - 1;
	GLfloat bezBlendFcn;

	bezPt->x = bezPt->y = bezPt->z = 0.0;

	/* Compute blending functions and blend control points. */
	for (k = 0; k < nCtrlPts; k++)
	{
		bezBlendFcn = C[k] * pow(u, k) * pow(1 - u, n - k);
		bezPt->x += ctrlPts[k].x * bezBlendFcn;
		bezPt->y += ctrlPts[k].y * bezBlendFcn;
		bezPt->z += ctrlPts[k].z * bezBlendFcn;
	}
}

void bezier(point* ctrlPts, GLint nCtrlPts, GLint nBezCurvePts)
{
	point bezCurvePt;
	GLfloat u;
	GLint *C, k;

	/* Allocate space for binomial coefficients */
	C = new GLint[nCtrlPts];

	binomialCoeffs(nCtrlPts - 1, C);
	for (k = 0; k <= nBezCurvePts; k++)
	{
		u = GLfloat(k) / GLfloat(nBezCurvePts);
		computeBezPt(u, &bezCurvePt, nCtrlPts, ctrlPts, C);
		glColor3f(1.0, 0.0, 0.0);
		glPointSize(3.0);
		plotPoint(bezCurvePt);
	}
	delete[] C;
}

void subdivide(vector<point> p, float myT)
{
	/* Create a temporary vector that we will use to recurse. */
	vector<point> temp;

	for (unsigned int i = 0; i < p.size() - 1; i++)
	{
		/* Call the interpolate function and push that new point to our temp vector. */
		point newPoint = interpolate(p[i], p[i + 1], myT);
		temp.push_back(newPoint);

		/* Need to add the last point to both control polygons!*/
		if (p.size() == 2)
		{
			glPointSize(5.0);
			glColor3f(0.0, 1.0, 1.0);
			plotPoint(newPoint);
			//glutPostRedisplay();

			/*for (unsigned int i = 0; i < ctrlLeft.size(); i++)
			{
				cout << "ctrlLeft[" << i << "] = " << ctrlLeft[i].x << ", " << ctrlLeft[i].y << endl;
			}
			for (unsigned int i = 0; i < ctrlRight.size(); i++)
			{
				cout << "ctrlRight[" << i << "] = " << ctrlRight[i].x << ", " << ctrlRight[i].y << endl;
			}*/

		}
	}

	/* Make the recursive call until the size of the vector is 1. */
	if (p.size() != 1)
	{
		//cout << "p.size() = " << endl;
		int pos = p.size() - 1;

		/* If the first point in p, add to the left control polygon. */
		glPointSize(5.0);
		glColor3f(0.0, 1.0, 0.0);
		plotPoint(p.front());

		/* Draw the lines for the left control polygon. */
		point left = interpolate(p[0], p[1], myT);
		glBegin(GL_LINES);
		glVertex3f(p[0].x, p[0].y, 0.0);
		glVertex3f(left.x, left.y, 0.0);
		glEnd();
		
		/* If the last point in p, add to the right control polygon. */
		glPointSize(5.0);
		glColor3f(0.0, 0.0, 1.0);
		plotPoint(p.back());

		/* Draw the lines for the right control polygon. */
		point right = interpolate(p[pos - 1], p[pos], myT);
		glBegin(GL_LINES);
		glVertex3f(p[pos].x, p[pos].y, 0.0);
		glVertex3f(right.x, right.y, 0.0);
		glEnd();

		subdivide(temp, myT);
	}

}

void split1(vector<point> p)
{
	/* Create a temporary vector that we will use to recurse. */
	vector<point> temp;

	for (unsigned int i = 0; i < p.size() - 1; i++)
	{
		/* Call the interpolate function and push that new point to our temp vector. */
		point newPoint = interpolate(p[i], p[i + 1], 0.5);
		temp.push_back(newPoint);

		/* Need to add the last point to both control polygons!*/
		if (p.size() == 2)
		{
			ctrlLeft1.push_back(newPoint);
			ctrlRight1.push_back(newPoint);

			B1a = computeBound(ctrlLeft1);
			B1b = computeBound(ctrlRight1);

			ctrlLeft1.clear();
			ctrlRight1.clear();
		}
	}

	/* Make the recursive call until the size of the vector is 1. */
	if (p.size() != 1)
	{
		//If the first point in p, add to the left control polygon.
		ctrlLeft1.push_back(p.front());
		//If the last point in p, add to the right control polygon.
		ctrlRight1.push_back(p.back());

		split1(temp);
	}

}

void split2(vector<point> p)
{
	/* Create a temporary vector that we will use to recurse. */
	vector<point> temp;

	for (unsigned int i = 0; i < p.size() - 1; i++)
	{
		/* Call the interpolate function and push that new point to our temp vector. */
		point newPoint = interpolate(p[i], p[i + 1], 0.5);
		temp.push_back(newPoint);

		/* Need to add the last point to both control polygons!*/
		if (p.size() == 2)
		{
			ctrlLeft2.push_back(newPoint);
			ctrlRight2.push_back(newPoint);

			B2a = computeBound(ctrlLeft2);
			B2b = computeBound(ctrlRight2);

			ctrlLeft2.clear();
			ctrlRight2.clear();
		}
	}

	/* Make the recursive call until the size of the vector is 1. */
	if (p.size() != 1)
	{
		//If the first point in p, add to the left control polygon.
		ctrlLeft2.push_back(p.front());
		//If the last point in p, add to the right control polygon.
		ctrlRight2.push_back(p.back());

		split2(temp);
	}

}

void intersect(vector<point> p1, vector<point> p2)
{
	boundBox b1 = computeBound(p1);
	//cout << "b1.xMin = " << b1.xMin << endl;
	boundBox b2 = computeBound(p2);

	if (overlap(b1, b2))
	{
		//cout << "They overlap!" << endl;
		if (area(b1) + area(b2) < 0.000001)
		{
			cout << "We have an intersection!" << endl;
			point intersection;
			intersection.x = b1.xMax;
			intersection.y = b1.yMax;
			intersection.z = 0.0;
			plotPoint(intersection);
			return;
		}
		else
		{
			split1(p1);
			split2(p2);
			intersect(B1a.control, B2a.control);
			intersect(B1a.control, B2b.control);
			intersect(B1b.control, B2a.control);
			intersect(B1b.control, B2b.control);
		}
	}
	else
	{
		return;
	}

}

boundBox computeBound(vector<point> p)
{
	// Create bounding box for the the control points.
	boundBox pBox;
	//cout << pBox.xMin << endl;

	// Loop through the control box comparing x and y values to see what the minimum bounding box is.
	for (unsigned int i = 0; i < p.size(); i++)
	{
		if (p[i].x < pBox.xMin)
			pBox.xMin = p[i].x;
		if (p[i].x > pBox.xMax)
			pBox.xMax = p[i].x;
		if (p[i].y < pBox.yMin)
			pBox.yMin = p[i].y;
		if (p[i].y > pBox.yMax)
			pBox.yMax = p[i].y;

		//Add points to our control vector for later use.
		pBox.control.push_back(p[i]);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(pBox.xMin, pBox.yMin, pBox.xMax, pBox.yMax);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return pBox;
}

bool overlap(boundBox a, boundBox b)
{
	//cout << "Overlap was called." << endl;
	if (a.xMax < b.xMax) return false;
	if (a.xMin > b.xMax) return false;
	if (a.yMax < b.yMin) return false;
	if (a.yMin > b.yMax) return false;
	return true;
}

double area(boundBox box)
{
	double x = box.xMax - box.xMin;
	double y = box.yMax - box.yMin;;

	return(x*y);
}

/* Main, calls everything we need. */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	MainWindow = glutCreateWindow("Bezier Curves");

	init();
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
	glutKeyboardFunc(keyboardFcn);
	glutMouseFunc(mouseFcn);
	glutSpecialFunc(specialFcn);

	glutMainLoop();

	return 0;
}









