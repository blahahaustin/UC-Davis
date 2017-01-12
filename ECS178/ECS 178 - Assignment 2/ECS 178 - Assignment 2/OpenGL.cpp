#include "ass2.h"

//Set size of the world-coordinate clipping window.
GLfloat xwcMin = -200.0, xwcMax = 200.0;
GLfloat ywcMin = -200.0, ywcMax = 200.0;

//Global variable for our control points!
vector<point> ctrlPts;

//Global variables for points and derivatives!
vector<point> c1p;
vector<point> c1m;
vector<point> norm;

//Set the mode of the program based on keyboard press.
int mode = 1;

//A variable so that we will execute the current algorithm.
bool execute = false;

//Set the t variable globally so that we can edit it.
double t = 0.500;

//Variables for selection of control points.
const GLint pickBuffSize = 32;
GLint selectedPt;

//Performs any initial actions we need for the program.
void init(void)
{
	/* Set color of display window to white. */
	glClearColor(1.0, 1.0, 1.0, 0.0);
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
		c1p.clear();
		c1m.clear();
		norm.clear();
		t = 0.5;
		execute = false;
		glutPostRedisplay();
	}
}

void specialFcn(int key, int x, int y)
{
	if (key == GLUT_KEY_UP && mode == 1)
	{
		raise();
		glutPostRedisplay();
	}
	else if (key == GLUT_KEY_DOWN && mode == 1 && ctrlPts.size() > 1)
	{
		lower();
		glutPostRedisplay();
	}

	if (key == GLUT_KEY_UP && mode == 2 && t < 0.99)
	{
		t = t + 0.01;
		cout << "t = " << t << endl;
		glutPostRedisplay();
	}
	if (key == GLUT_KEY_DOWN && mode == 2 && t > 0.01)
	{
		t = t - 0.01;
		cout << "t = " << t << endl;
		glutPostRedisplay();
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
		gluPickMatrix(GLdouble(xMouse), GLdouble(vpArray[3] - yMouse), 5.0, 5.0, vpArray);
		gluOrtho2D(xwcMin, xwcMax, ywcMin, ywcMax);

		if (mode == 1 || mode == 2 || mode == 4)
		{
			drawControls(GL_SELECT);
		}
		else if (mode == 3)
		{
			drawHermite(GL_SELECT);
		}
		

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
		if (mode == 1 || mode == 2 || mode == 4)
		{
			if (selectedPt <= ctrlPts.size())
			{
				ctrlPts[selectedPt - 1] = newPoint;
			}
		}
		else if (mode == 3)
		{
			if (selectedPt <= c1p.size())
			{
				c1p[selectedPt - 1] = newPoint;
			}
			else
			{
				c1m[selectedPt - c1p.size() - 1] = newPoint;
			}
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
		if (mode == 1 || mode == 2 || mode == 4)
		{
			int pos = selectedPt - 1;
			ctrlPts.erase(ctrlPts.begin() + pos);
		}
		else if (mode == 3)
		{
			if (selectedPt <= c1p.size())
			{
				int pos = selectedPt - 1;
				c1p.erase(c1p.begin() + pos);
				c1m.erase(c1m.begin() + pos);
			}
			else
			{
				int pos = selectedPt - c1p.size() - 1;
				c1p.erase(c1p.begin() + pos);
				c1m.erase(c1m.begin() + pos);
			}
		}


		/* Reset the selected point! */
		selectedPt = 0;

		glutPostRedisplay();
	}
	/* If the left button is clicked. */
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		/* Convert clicked point to world coordinates. */
		point clicked = GetOGLPos(xMouse, yMouse);

		/* Push points into a vector of control points. */
		if (mode == 1 || mode == 2 || mode == 4)
		{
			ctrlPts.push_back(clicked);
		}
		/*Push points into the vector of c1 points and derivatives. */
		if (mode == 3)
		{
			if (c1p.size() == 0)
			{
				c1p.push_back(clicked);
			}
			else if (c1m.size() < c1p.size())
			{
				c1m.push_back(clicked);
			}
			else
			{
				c1p.push_back(clicked);
			}
		}
		

		/* Update the display. */
		glutPostRedisplay();
	}
}

void processPicks(GLint nPicks, GLuint pickBuffer[])
{
	GLuint objID, *ptr;

	ptr = pickBuffer;

	//find the selected point and store it globally
	if (nPicks > 0)
	{
		objID = *ptr;
		selectedPt = objID;
	}
}


void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT); //Clear display window.

	//Draw the correct points for the mode we are in.
	if (mode == 1 || mode == 2)
		drawControls(GL_RENDER);
	else if (mode == 3 || mode == 4)
		drawHermite(GL_RENDER);

	/* If execute is true, do degree raising and lowering depending on user input!*/
	if (execute && mode == 1 && ctrlPts.size() > 1)
	{
		bezier(ctrlPts.data(), ctrlPts.size(), 1000);
	}
	if (execute && mode == 2 && ctrlPts.size() > 1)
	{
		vector<double> chords = chord();
		/* Call the Aitken algorithm 1000 times. */
		for (double i = 0.0; i <= 1.0; i = i + 0.001)
		{
			aitken(i, chords);
		}
	}
	if (execute && mode == 3)
	{
		vectorConvert();
		hermite();
	}
	if (execute && mode == 4)
	{
		vector<double> chords = chord();
		calcDerivative(chords);
		drawHermite(GL_RENDER);
		vectorConvert();
		hermite();
	}

	glFlush();
}
