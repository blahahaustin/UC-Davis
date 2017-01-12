#include "ass3.h"

//Set size of the world-coordinate clipping window.
GLfloat xwcMin = -200.0, xwcMax = 200.0;
GLfloat ywcMin = -200.0, ywcMax = 200.0;

//Global variable for our control points!
vector<point> ctrlPts;
point deriv;
point derivEnd;
bool specified = false;
bool specifiedEnd = false;
bool dend = false;
bool rev = false;
int order = 3;

//Set the mode of the program based on keyboard press.
int mode = 3;

//Set the parameterization mode for the program based on keyboard press.
string param = "uni";

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
	case 'q': param = "uni"; cout << "UNIFORM PARAMETERIZATION" << endl; glutPostRedisplay(); break;
	case 'w': param = "chord"; cout << "CHORD LENGTH PARAMETERIZATION" << endl; glutPostRedisplay(); break;
	case 'e': param = "cent"; cout << "CENTRIPEDAL PARAMETERIZATION" << endl; glutPostRedisplay(); break;
	case 'd': dend = true; cout << "Now specify the ending derivative." << endl; glutPostRedisplay(); break;
	}

	/* Press space to draw the picture and again to reset it. */
	if (key == ' ' && execute == false)
	{
		cout << "EXECUTE" << endl;
		execute = true;
		glutPostRedisplay();
	}
	else if (key == ' ' && execute == true)
	{
		cout << "RESET" << endl;
		ctrlPts.clear();
		t = 0.5;
		execute = false;
		specified = false;
		dend = false;
		specifiedEnd = false;
		glutPostRedisplay();
	}
}

void specialFcn(int key, int x, int y)
{
	if (key == GLUT_KEY_DOWN && order > 3)
	{
		order--;
		cout << "Order is now " << order << endl;
		glutPostRedisplay();
	}
	else if (key == GLUT_KEY_UP && order < ctrlPts.size() - 1)
	{
		order++;
		cout << "Order is now " << order << endl;
		glutPostRedisplay();
	}
}

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
		if (selectedPt <= ctrlPts.size())
		{
			ctrlPts[selectedPt - 1] = newPoint;
		}

		if (selectedPt == ctrlPts.size() + 1)
		{
			deriv = newPoint;
		}

		if (selectedPt == ctrlPts.size() + 2)
		{
			derivEnd = newPoint;
		}

		/* Reset the selected point! */
		selectedPt = 0;

		glutPostRedisplay();
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		/* The unwanted case where nothing is selected. */
		if (selectedPt < 1 || selectedPt > ctrlPts.size())
			return;

		/* Delete the point! */
		int pos = selectedPt - 1;
		ctrlPts.erase(ctrlPts.begin() + pos);

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
		if (mode == 3 && ctrlPts.size() == 1 && specified == false)
		{
			deriv = clicked;
			specified = true;
		}
		else if (mode == 3 && dend == true)
		{
			derivEnd = clicked;
			specifiedEnd = true;
		}
		else
		{
			ctrlPts.push_back(clicked);
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
		cout << "selectedPt = " << selectedPt << endl;
	}
}

void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT); //Clear display window.

	drawControls(GL_RENDER);

	vector<double> parameter;
	vector<point> d;

	if (mode == 1 && execute == true && ctrlPts.size() >= 3)
	{
		//Pick the parameterization!
		if (param == "uni")
			parameter = computeUni();
		else if (param == "chord")
			parameter = computeChord();
		else if (param == "cent")
			parameter = computeCent();

		//Calculate and return the deBoor points.
		d = calculateDeBoor(parameter);
		deBoorToBez(d, parameter);
	}
	else if (mode == 2 && execute == true && ctrlPts.size() >= 3)
	{
		for (double i = 0.001; i < 1; i = i + .001)
		{
			deBoor(order, ctrlPts, i);
		}
			
	}
	else if (mode == 3 && execute == true && ctrlPts.size() >= 2)
	{
		vector<point> bezForward;
		vector<point> bezBackward;

		//Calculate the bezierPoints in the forward direction.
		if (param == "uni")
			parameter = computeUni();
		else if (param == "chord")
			parameter = computeChord();
		else if (param == "cent")
			parameter = computeCent();

		bezForward = computeQuad(parameter);

		//Calculate the bezierPoints in the backward direction.
		reverse(ctrlPts.begin(), ctrlPts.end());
		rev = true;
		
		if (param == "uni")
			parameter = computeUni();
		else if (param == "chord")
			parameter = computeChord();
		else if (param == "cent")
			parameter = computeCent();

		bezBackward = computeQuad(parameter);
		rev = false;
		reverse(ctrlPts.begin(), ctrlPts.end());

		average(bezForward, bezBackward);


	}

	glFlush();
}