#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
using namespace std;

//class definition
class wcPt3D {
	public:
		GLfloat x, y, z;
};

//global variables
const GLint pickBuffSize = 32;
GLfloat *inputs;
wcPt3D *ctrlPts;
GLint selectedPt;
int MainWindow;
int SubMenu;

//function prototypes
wcPt3D GetOGLPos(int, int);
void processMouseClick(int, int, int, int); //call back function
void handlePoint(GLint x1, GLint y1, GLint x2, GLint y2);
void createMenu();	//a function that creates menu
void processMenu(int value); //----Call back function---// As explained during discussion, this process menu operations

/* Set initial size of the display window. */
GLsizei winWidth = 600, winHeight = 600;

/* Set size of world-coordinate clipping window. */
GLfloat xwcMin = -200.0, xwcMax = 200.0;
GLfloat ywcMin = -200.0, ywcMax = 200.0;

void init (void)
{
	/* Set color of display window to white. */
	glClearColor (1.0, 1.0, 1.0, 0.0);
}

void plotPoint (wcPt3D bezCurvePt)
{
	glBegin (GL_POINTS);
		glVertex2f (bezCurvePt.x, bezCurvePt.y);
	glEnd();
}

/* Compute binomial coefficients C for given value of n. */
void binomialCoeffs (GLint n, GLint *C)
{
	GLint k, j;
	
	for (k = 0; k <= n; k++)
	{
		/* compute n!/(k!(n - k)!). */
		C[k] = 1;
		for(j = n; j >= k + 1; j--)
			C[k] *= j;
		for(j = n - k; j >= 2; j--)
			C[k] /= j;
	}
}

void computeBezPt (GLfloat u, wcPt3D* bezPt, GLint nCtrlPts,
					wcPt3D* ctrlPts, GLint* C)
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

void bezier (wcPt3D* ctrlPts, GLint nCtrlPts, GLint nBezCurvePts)
{
	wcPt3D bezCurvePt;
	GLfloat u;
	GLint *C, k;
	
	/* Allocate space for binomial coefficients */
	C = new GLint[nCtrlPts];
	
	binomialCoeffs(nCtrlPts - 1, C);
	for(k = 0; k <= nBezCurvePts; k++)
	{
		u = GLfloat(k) / GLfloat(nBezCurvePts);
		computeBezPt(u, &bezCurvePt, nCtrlPts, ctrlPts, C);
		plotPoint(bezCurvePt);
	}
	delete[] C;
}

void plotCtrl(wcPt3D* ctrlPts, GLint nCtrlPts)
{
	glPointSize(5);
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	for(int k = 0; k <= nCtrlPts; k++)
	{
		plotPoint(ctrlPts[k]);
	}
}

/* Define 5 control points and associated IDs */
void curve(GLenum mode)
{	
	if(mode == GL_SELECT)
		glPushName(0);
	ctrlPts[0].x = inputs[0]; ctrlPts[0].y = inputs[1]; ctrlPts[0].z = inputs[2];
	glPointSize(5);
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	plotPoint(ctrlPts[0]);

	if(mode == GL_SELECT)
		glPushName(1);
	glPointSize(5);
	ctrlPts[1].x = inputs[3]; ctrlPts[1].y = inputs[4]; ctrlPts[1].z = inputs[5];
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	plotPoint(ctrlPts[1]);

	if(mode == GL_SELECT)
		glPushName(2);
	glPointSize(5);
	ctrlPts[2].x = inputs[6]; ctrlPts[2].y = inputs[7]; ctrlPts[2].z = inputs[8];
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	plotPoint(ctrlPts[2]);

	if(mode == GL_SELECT)
		glPushName(3);
	glPointSize(5);
	ctrlPts[3].x = inputs[9]; ctrlPts[3].y = inputs[10]; ctrlPts[3].z = inputs[11];
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	plotPoint(ctrlPts[3]);

	if(mode == GL_SELECT)
		glPushName(4);
	glPointSize(5);
	ctrlPts[4].x = inputs[12]; ctrlPts[4].y = inputs[13]; ctrlPts[4].z = inputs[14];
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	plotPoint(ctrlPts[4]);
}

void displayFcn (void)
{
	glClear(GL_COLOR_BUFFER_BIT); //Clear display window.
	curve(GL_RENDER); //sets control points
	
	//paint bezier curve
	GLint nCtrlPts = 5, nBezCurvePts = 1500;
	glPointSize(4);
	glColor3f(1.0, 0.0, 0.0); //Set point color to red.
	bezier(ctrlPts, nCtrlPts, nBezCurvePts);
	
	glFlush();
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	/* Maintain an aspect ratio of 1.0. */
	glViewport(0, 0, newHeight, newHeight);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluOrtho2D (xwcMin, xwcMax, ywcMin, ywcMax);
	
	glClear(GL_COLOR_BUFFER_BIT);
}

void processPicks(GLint nPicks, GLuint pickBuffer[])
{
	GLint j, k;
	GLuint objID, *ptr;
	
	cout << "Number of objects picked = " << nPicks << endl << endl;
	ptr = pickBuffer;
	
	//find the selected point and store it globally
	//MAKE SURE TO SUBTRACT ONE FOR INDEXING
	if (nPicks > 0)
	{
		objID = *ptr;
		selectedPt = objID;
	}
}

void movePoint(GLint nPicks, GLuint pickBuffer[])
{

}

//button can be either GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON.
//state can be either GLUT_UP or GLUT_DOWN
//x, y are window position, with (0,0) at top left and x is to the right, y is down (different from normal convention)
void processMouseClick(int button, int state, int xMouse, int yMouse)
{
	GLuint pickBuffer[pickBuffSize];
	GLint nPicks, vpArray[4];
	
	//here we select a point
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
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
		gluPickMatrix(GLdouble (xMouse), GLdouble(vpArray[3] - yMouse),
						5.0, 5.0, vpArray);
		gluOrtho2D (xwcMin, xwcMax, ywcMin, ywcMax);
		curve(GL_SELECT);
		
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
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{	
		//unwanted case
		if(selectedPt < 1)
			return;
		
		//create new point at the drag-dropped position
		//cout << "selected point = " << selectedPt << endl;
		wcPt3D newPoint = GetOGLPos(xMouse, yMouse);
		
		//update the inputs[] array, everything else will update!
		int pos = (selectedPt - 1) * 3;
		inputs[pos] = newPoint.x;
		inputs[pos + 1] = newPoint.y;
		inputs[pos + 2] = 0.0;
		cout << inputs[pos] << ", " << inputs[pos + 1] << " , " << inputs[pos + 2] << endl;
		
		//reset selected point!
		selectedPt = 0;

		glutPostRedisplay();
	}
	else if (button == GLUT_MIDDLE_BUTTON)
	{
		//move point to here!
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		//open menu to write file?
	}
	
	glutPostRedisplay();
}

wcPt3D GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	wcPt3D newPoint = {posX, posY, posZ};
 
    return newPoint;
}

void createMenu()
{
	//main menu, 
	//note, here we pass in the same function to process the menu actions as above
	glutCreateMenu(processMenu);
	glutAddMenuEntry("Write Current Points to File", 1);

	//attach submenu to main menu
	glutAddSubMenu("SubMenu", SubMenu);
	glutAddMenuEntry("Exit", 0);

	//attach menu to window which will be triggered by right button click
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//This function is called if user clicked a menu entry
//value is the value associated with a menu entry
void processMenu(int value)
{
	if (value == 0)
	{
		glutDestroyWindow(MainWindow);
		exit(0);
		//bye!
	}
	else if (value == 1)
	{
		ofstream outfile("output.txt");
		for(int i = 0; i < 15; i++)
		{
			outfile << inputs[i] << " ";
			if ((i + 1) % 3 == 0)
			{
				outfile << endl;
			}
		}
		outfile.close();
	}
}

int main (int argc, char** argv)
{
	inputs = new GLfloat[15];
	ctrlPts = new wcPt3D[5];

	ifstream myfile(argv[1]);
	if(!myfile.is_open())
		cout << "Could not open file!" << endl;
	else
	{
		for(int i = 0; i < 15; i++)
		{
			myfile >> inputs[i];
			//cout << "inputs[" << i << "] = " << inputs[i] << endl;
		}
	}
	
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition (50, 50);
	glutInitWindowSize (winWidth, winHeight);
	MainWindow = glutCreateWindow("Bezier Curve");

	init();
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
	//set call back function to the one we want
	glutMouseFunc(processMouseClick);
	//Create and attach menu to the window
	createMenu();

	glutMainLoop();
	
	return 0;
}
