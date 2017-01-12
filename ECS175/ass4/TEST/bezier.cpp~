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
wcPt3D *ctrlPts1;
wcPt3D *ctrlPts2;
wcPt3D *ctrlPts3;
wcPt3D *ctrlPts4;
wcPt3D *ctrlPts5;
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
	//CURVE 1 - FIRST POINT IS LAST OF CURVE 5, LAST POINT IS FIRST OF CURVE 2
	if(mode == GL_SELECT)
		glPushName(0);
	ctrlPts1[0].x = inputs[0]; ctrlPts1[0].y = inputs[1]; ctrlPts1[0].z = inputs[2];
	glPointSize(5);
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	plotPoint(ctrlPts1[0]);

	if(mode == GL_SELECT)
		glPushName(1);
	ctrlPts1[1].x = inputs[3]; ctrlPts1[1].y = inputs[4]; ctrlPts1[1].z = inputs[5];
	glPointSize(5);
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	plotPoint(ctrlPts1[1]);

	if(mode == GL_SELECT)
		glPushName(2);
	ctrlPts1[2].x = inputs[6]; ctrlPts1[2].y = inputs[7]; ctrlPts1[2].z = inputs[8];
	glPointSize(5);
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	plotPoint(ctrlPts1[2]);
	
	//CURVE 2 - FIRST POINT IS LAST OF CURVE 1, LAST POINT IS FIRST OF CURVE 3
	ctrlPts2[0] = ctrlPts1[2];

	if(mode == GL_SELECT)
		glPushName(4);
	ctrlPts2[1].x = inputs[9]; ctrlPts2[1].y = inputs[10]; ctrlPts2[1].z = inputs[11];
	glPointSize(5);
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	plotPoint(ctrlPts2[1]);

	if(mode == GL_SELECT)
		glPushName(5);
	ctrlPts2[2].x = inputs[12]; ctrlPts2[2].y = inputs[13]; ctrlPts2[2].z = inputs[14];
	glPointSize(5);
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	plotPoint(ctrlPts2[2]);
	

	//CURVE 3 - FIRST POINT IS LAST OF CURVE 2, LAST POINT IS FIRST OF CURVE 4
	ctrlPts3[0] = ctrlPts2[2];
	
	if(mode == GL_SELECT)
		glPushName(7);
	ctrlPts3[1].x = inputs[15]; ctrlPts3[1].y = inputs[16]; ctrlPts3[1].z = inputs[17];
	glPointSize(5);
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	plotPoint(ctrlPts3[1]);

	if(mode == GL_SELECT)
		glPushName(8);
	ctrlPts3[2].x = inputs[18]; ctrlPts3[2].y = inputs[19]; ctrlPts3[2].z = inputs[20];
	glPointSize(5);
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	plotPoint(ctrlPts3[2]);
	

	//CURVE 4 - FIRST POINT IS LAST OF CURVE 3, LAST POINT IS FIRST OF CURVE 5
	ctrlPts4[0] = ctrlPts3[2];
	
	if(mode == GL_SELECT)
		glPushName(10);
	ctrlPts4[1].x = inputs[21]; ctrlPts4[1].y = inputs[22]; ctrlPts4[1].z = inputs[23];
	glPointSize(5);
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	plotPoint(ctrlPts4[1]);

	if(mode == GL_SELECT)
		glPushName(11);
	ctrlPts4[2].x = inputs[24]; ctrlPts4[2].y = inputs[25]; ctrlPts4[2].z = inputs[26];
	glPointSize(5);
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	plotPoint(ctrlPts4[2]);
	

	//CURVE 5 - FIRST POINT IS LAST OF CURVE 4, LAST POINT IS FIRST OF CURVE 1
	ctrlPts5[0] = ctrlPts4[2];
	
	if(mode == GL_SELECT)
		glPushName(13);
	ctrlPts5[1].x = inputs[27]; ctrlPts5[1].y = inputs[28]; ctrlPts5[1].z = inputs[29];
	glPointSize(5);
	glColor3f(0.0, 0.0, 0.0); //Set point color to black.
	plotPoint(ctrlPts5[1]);

	ctrlPts5[2] = ctrlPts1[0];
}

void displayFcn (void)
{
	glClear(GL_COLOR_BUFFER_BIT); //Clear display window.
	curve(GL_RENDER); //sets control points
	
	//paint bezier curves
	GLint nCtrlPts = 3, nBezCurvePts = 1000;
	glPointSize(4);
	glColor3f(1.0, 0.0, 0.0); //Set point color to red.
	bezier(ctrlPts1, nCtrlPts, nBezCurvePts);
	bezier(ctrlPts2, nCtrlPts, nBezCurvePts);
	bezier(ctrlPts3, nCtrlPts, nBezCurvePts);
	bezier(ctrlPts4, nCtrlPts, nBezCurvePts);
	bezier(ctrlPts5, nCtrlPts, nBezCurvePts);
	
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
		for(int i = 0; i < 30; i++)
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
	inputs = new GLfloat[30];
	ctrlPts1 = new wcPt3D[3];
	ctrlPts2 = new wcPt3D[3];
	ctrlPts3 = new wcPt3D[3];
	ctrlPts4 = new wcPt3D[3];
	ctrlPts5 = new wcPt3D[3];

	ifstream myfile(argv[1]);
	if(!myfile.is_open())
		cout << "Could not open file!" << endl;
	else
	{
		for(int i = 0; i < 30; i++)
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
