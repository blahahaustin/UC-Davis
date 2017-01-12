#include "final.h"
using namespace std;

#define USER_SELECT 1
#define USER_ORBIT 2
#define USER_MOVEPT 3

//------VARIABLE LIST--------------------------

//Set integer variables for the windows.
int MainWindow;

//Set initial size of the display window.
GLsizei winWidth = 600, winHeight = 600;

//Global variables to change our looking angle.
double theta = 0.0, phi = 0.0, radius = 100.0;

//Variables for selection of control points.
const GLint pickBuffSize = 32;
GLint selectedPt;

//A global variable to hold our control net and knot sequences.
vector< vector<point> > ctrlNet;
vector< vector<point> > ctrlNetStar;
double* u;
double* v;

//The dimensions of our control net.
int m = 0, n = 0;

//The orders of our de Boor functions.
//k cannot be less than m, and l cannot be less than n
int k = 3, l = 3;

int uResolution = 40, vResolution = 40;

//Set the initial mode our program will be in.
int mode = 1;

char valueChange = 'x';

//---------------------------------------------

//Performs any initial actions we need for the program.
void init(void)
{
	/* Set color of display window to white. */
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST); //need to add so z-values are read correctly

	getControlNet();
	calculateKnots();
}

/* A function to plot points. */
void plotPoint(point pt)
{
	glBegin(GL_POINTS);
	glVertex3f(pt.x, pt.y, pt.z);
	glEnd();
}

void drawControlNet(GLenum mode)
{
	//Plot the points.
	for (unsigned int i = 0; i < ctrlNet.size(); i++)
	{
		for (unsigned int j = 0; j < ctrlNet[i].size(); j++)
		{
			if (mode == GL_SELECT)
				glPushName(i*m + j);
			//cout << "i*m + j = " << i*m + j << endl;
			glPointSize(6.0);
			glColor3f(0.0, 0.0, 0.0);
			plotPoint(ctrlNet[i][j]);
			//cout << "Plotting at: (" << ctrlNet[i][j].x << ", " << ctrlNet[i][j].y << "," << ctrlNet[i][j].z << ")" << endl;
		}
	}

	//Draw lines between the columns.
	for (unsigned int i = 0; i < ctrlNet.size(); i++)
	{
		for (unsigned int j = 0; j < ctrlNet[i].size() - 1; j++)
		{
			glLineWidth(2.0);
			glColor3f(0.0, 0.0, 0.0);
			glBegin(GL_LINES);
				glVertex3f(ctrlNet[i][j].x, ctrlNet[i][j].y, ctrlNet[i][j].z);
				glVertex3f(ctrlNet[i][j+1].x, ctrlNet[i][j+1].y, ctrlNet[i][j+1].z);
			glEnd();
		}
	}

	//Draw lines between the rows.
	for (unsigned int i = 0; i < ctrlNet.size() - 1; i++)
	{
		for (unsigned int j = 0; j < ctrlNet[i].size(); j++)
		{
			glLineWidth(2.0);
			glColor3f(0.0, 0.0, 0.0);
			glBegin(GL_LINES);
			glVertex3f(ctrlNet[i][j].x, ctrlNet[i][j].y, ctrlNet[i][j].z);
			glVertex3f(ctrlNet[i + 1][j].x, ctrlNet[i + 1][j].y, ctrlNet[i + 1][j].z);
			glEnd();
		}
	}
}

//Parses data from the file and places it into our ctrlNet vector!
void getControlNet()
{
	vector<point> temp;
	//vector<point> points;

	ifstream data("points.csv");
	string line;
	
	while (getline(data, line))
	{
		n++; //Iterate n.
		stringstream lineStream(line);
		string cell;
		while (getline(lineStream, cell, ','))
		{
			m++; //Iterate m, will divide by n later.

			point p;

			vector<string> tokens;
			stringstream cellStream(cell);
			string str;
			while (getline(cellStream, str, ' '))
			{
				//cout << str << endl;
				tokens.push_back(str);
			}

			p.x = stof(tokens[0]);
			p.y = stof(tokens[1]);
			p.z = stof(tokens[2]);
			p.w = stof(tokens[3]);

			temp.push_back(p);
		}
	}

	m = m / n;

	cout << "m = " << m << endl;
	cout << "n = " << n << endl;

	//Set up sizes.
	ctrlNet.resize(n);
	for (unsigned int i = 0; i < n; i++)
	{
		//cout << "i = " << i << endl;
		ctrlNet[i].resize(m);
	}

	for (unsigned int i = 0; i < ctrlNet.size(); i++)
	{
		for (unsigned int j = 0; j < ctrlNet[i].size(); j++)
		{
			ctrlNet[i][j] = temp[i*m + j];
		}
	}
}

void getControlNetStar()
{
	//Set up sizes.
	ctrlNetStar.resize(n);
	for (unsigned int i = 0; i < n; i++)
	{
		//cout << "i = " << i << endl;
		ctrlNetStar[i].resize(m);
	}

	for (unsigned int i = 0; i < ctrlNet.size(); i++)
	{
		for (unsigned int j = 0; j < ctrlNet[i].size(); j++)
		{
			ctrlNetStar[i][j].x = ctrlNet[i][j].x*ctrlNet[i][j].w;
			ctrlNetStar[i][j].y = ctrlNet[i][j].y*ctrlNet[i][j].w;
			ctrlNetStar[i][j].z = ctrlNet[i][j].z*ctrlNet[i][j].w;
			ctrlNetStar[i][j].w = ctrlNet[i][j].w;
		}
	}
}

void calculateKnots()
{
	int param, degree, numKnots;

	//CALCULATE U (m-direction)
	param = m - 1;
	degree = k;
	numKnots = param + degree + 1;
	u = new double[numKnots];
	for (unsigned int i = 0; i <= degree; i++)
		u[i] = 0.0;
	for (unsigned int j = 1; j <= param - degree; j++)
		u[j + degree] = (double)j / double(param - degree + 1);
	for (unsigned int i = numKnots - degree; i <= numKnots; i++)
		u[i] = 1.0;

	for (unsigned int i = 0; i < numKnots + 1; i++)
	{
		cout << "u[" << i << "] = " << u[i] << endl;
	}
	
	//CALCULATE V (n-direction)
	param = n - 1;
	degree = l;
	numKnots = param + degree + 1;
	v = new double[numKnots];
	for (unsigned int i = 0; i <= degree; i++)
		v[i] = 0.0;
	for (unsigned int j = 1; j <= param - degree; j++)
		v[j + degree] = (double)j / double(param - degree + 1);
	for (unsigned int i = numKnots - degree; i <= numKnots; i++)
		v[i] = 1.0;

	/*for (unsigned int i = 0; i < numKnots + 1; i++)
	{
	cout << "v[" << i << "] = " << v[i] << endl;
	}*/
}

point deBoor(int order, double* knots, vector<point> d, double uBar)
{
	//temp[r][i]
	point myd[30][30];

	//Need to find the interval I, such that the value of u is in [uI, uI+1)
	int I = 0;
	for (unsigned int i = 0; i < m + k; i++)
	{
		if (uBar >= u[i] && uBar < u[i + 1])
		{
			I = i;
		}
	}
	//cout << "I = " << I << endl;

	//Need to fill the first scheme in the array, is just the deBoor points.
	for (unsigned int i = 0; i < d.size(); i++)
	{
		//cout << "i = " << i << endl;
		myd[0][i] = d[i];
	}

	for (int r = 1; r <= order - 1; r++)
	{
		for (int i = I - (order - 1); i <= (I - r); i++)
		{
			point temp;
			temp.x = ((knots[i + order] - uBar) / (knots[i + order] - knots[i + r]))*myd[r - 1][i].x
				+ ((uBar - knots[i + r]) / (knots[i + order] - knots[i + r]))*myd[r - 1][i + 1].x;
			temp.y = ((knots[i + order] - uBar) / (knots[i + order] - knots[i + r]))*myd[r - 1][i].y
				+ ((uBar - knots[i + r]) / (knots[i + order] - knots[i + r]))*myd[r - 1][i + 1].y;
			temp.z = ((knots[i + order] - uBar) / (knots[i + order] - knots[i + r]))*myd[r - 1][i].z
				+ ((uBar - knots[i + r]) / (knots[i + order] - knots[i + r]))*myd[r - 1][i + 1].z;
			temp.w = ((knots[i + order] - uBar) / (knots[i + order] - knots[i + r]))*myd[r - 1][i].w
				+ ((uBar - knots[i + r]) / (knots[i + order] - knots[i + r]))*myd[r - 1][i + 1].w;
			myd[r][i] = temp;
		}
	}

	/*for (int r = 0; r < order; r++)
	{
		for (int i = I - (order - 1); i <= (I - r); i++)
		{
			cout << "myd[" << r << "][" << i << "] = (" <<
				myd[r][i].x << ", " << myd[r][i].y << ", " << myd[r][i].z <<
				", " << myd[r][i].w << ")\n";
		}
		cout << endl;
	}*/

	return myd[order - 1][I - (order - 1)];
}

void calcSurface(int uRes, int vRes)
{
	double uResInc = 1.0 / (double)uRes;
	double vResInc = 1.0 / (double)vRes;

	for (double inc = 0.0; inc < 1; inc += uResInc)
	{
		vector<point> temp;
		for (double j = 0.0; j < 1; j += vResInc)
		{
			//cout << "j = " << j << endl;
			//Calculate in the u direction.
			for (unsigned int i = 0; i < ctrlNetStar.size(); i++)
			{
				point p;
				p = deBoor(k, u, ctrlNetStar[i], inc);
				temp.push_back(p);
			}

			//Calculate in the v direction.
			point p;
			p = deBoor(l, v, temp, j);

			//Inhomogenize the point before plotting.
			p.x = p.x / p.w;
			p.y = p.y / p.w;
			p.z = p.z / p.w;

			glPointSize(2.0);
			glColor3f(1.0, 0.0, 0.0);
			plotPoint(p);
		}
	}
}

void winReshapeFcn(int newWidth, int newHeight)
{
	//set viewport to new height/width
	glViewport(0, 0, newWidth, newHeight);

	//set to projection mode for Frustum function
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 500.0);

	//set back to model view for lookat function to work later
	glMatrixMode(GL_MODELVIEW);
}

void keyboardFcn(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1': mode = USER_SELECT; cout << "mode is now USER_SELECT" << endl; break;
	case '2': mode = USER_ORBIT; cout << "mode is now USER_ORBIT" << endl; break;
	case '3': mode = USER_MOVEPT; cout << "mode is now USER_MOVEPT" << endl; break;

	case 'w': phi += 0.5; glutPostRedisplay(); break;
	case 's': phi -= 0.5; glutPostRedisplay(); break;
	case 'a': theta -= 0.1; glutPostRedisplay(); break;
	case 'd': theta += 0.1; glutPostRedisplay(); break;

	case 'u': valueChange = 'u'; cout << "EDIT U-RESOLUTION" << endl; break;
	case 'v': valueChange = 'v'; cout << "EDIT V-RESOLUTION" << endl; break;

	case 'x': valueChange = 'x'; cout << "EDIT X-VALUE" << endl; break;
	case 'y': valueChange = 'y'; cout << "EDIT Y-VALUE" << endl; break;
	case 'z': valueChange = 'z'; cout << "EDIT Z-VALUE" << endl; break;

	case 'k': valueChange = 'k'; cout << "EDIT ORDER IN U DIRECTION" << endl; break;
	case 'l': valueChange = 'l'; cout << "EDIT ORDER IN V DIRECTION" << endl; break;
	}
}

void specialFcn(int key, int x, int y)
{
	if (key == GLUT_KEY_DOWN)
	{
		if (valueChange == 'k' && k > 2)
			k--;
		else if (valueChange == 'l' && l > 2)
			l--;
		else if (valueChange == 'u')
			uResolution--;
		else if (valueChange == 'v')
			vResolution--;
		else if (valueChange == 'x') {
			if (selectedPt < 1)
				return;
			for (int i = 0; i < n; i++) {
				if (selectedPt - 1 < (m*i) + m && selectedPt - 1 >= m*i)
					ctrlNet[i][selectedPt - 1 - m*i].x -= 1;
			}
		}
		else if (valueChange == 'y') {
			if (selectedPt < 1)
				return;
			for (int i = 0; i < n; i++) {
				if (selectedPt - 1 < (m*i) + m && selectedPt - 1 >= m*i)
					ctrlNet[i][selectedPt - 1 - m*i].y -= 1;
			}
		}
		else if (valueChange == 'z') {
			if (selectedPt < 1)
				return;
			for (int i = 0; i < n; i++) {
				if (selectedPt - 1 < (m*i) + m && selectedPt - 1 >= m*i)
					ctrlNet[i][selectedPt - 1 - m*i].z -= 1;
			}
		}

	}
	else if (key == GLUT_KEY_UP)
	{
		if (valueChange == 'k' && k < m)
			k++;
		else if (valueChange == 'l' && l < n)
			l++;
		else if (valueChange == 'u')
			uResolution++;
		else if (valueChange == 'v')
			vResolution++;
		else if (valueChange == 'x') {
			if (selectedPt < 1)
				return;
			for (int i = 0; i < n; i++) {
				if (selectedPt - 1 < (m*i) + m && selectedPt - 1 >= m*i)
					ctrlNet[i][selectedPt - 1 - m*i].x += 1;
			}
		}
		else if (valueChange == 'y') {
			if (selectedPt < 1)
				return;
			for (int i = 0; i < n; i++) {
				if (selectedPt - 1 < (m*i) + m && selectedPt - 1 >= m*i)
					ctrlNet[i][selectedPt - 1 - m*i].y += 1;
			}
		}
		else if (valueChange == 'z') {
			if (selectedPt < 1)
				return;
			for (int i = 0; i < n; i++) {
				if (selectedPt - 1 < (m*i) + m && selectedPt - 1 >= m*i)
					ctrlNet[i][selectedPt - 1 - m*i].z += 1;
			}
		}
	}
	glutPostRedisplay();
}


void mouseFcn(int button, int state, int xMouse, int yMouse)
{
	GLuint pickBuffer[pickBuffSize];
	GLint nPicks, vpArray[4];

	if ((button == GLUT_LEFT_BUTTON) && state == GLUT_DOWN && mode == USER_SELECT)
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
		gluPickMatrix(GLdouble(xMouse), GLdouble(vpArray[3] - yMouse), 10.0, 10.0, vpArray);
		glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 500.0);

		drawControlNet(GL_SELECT);

		/*Restore original viewing matrix */
		//glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 500.0);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);

		glFlush();

		/* Determine the number of picked objects and return to the
		* normal rendering mode. */
		nPicks = glRenderMode(GL_RENDER);

		processPicks(nPicks, pickBuffer); //process picked objects

		glutPostRedisplay();
	}

	if ( (button == GLUT_LEFT_BUTTON) && (mode == USER_MOVEPT) && (state == GLUT_DOWN))
	{
		if (selectedPt < 1)
			return;

		for (int i = 0; i < n; i++)
		{
			if (selectedPt - 1 < (m*i) + m && selectedPt - 1 >= m*i)
			{
				ctrlNet[i][selectedPt - 1 - m*i].x += 1;
			}
		}

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

void displayFcn()
{
	//depth buffer for proper representation
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity(); //clear the matrix

	//Calculate the center of the controlNet so we can focus on it.
	double xMin = 500.0, xMax = -500,
		yMin = 500.0, yMax = -500,
		zMin = 500.0, zMax = -500;

	for (unsigned int i = 0; i < ctrlNet.size(); i++)
	{
		for (unsigned int j = 0; j < ctrlNet[i].size(); j++)
		{
			if (ctrlNet[i][j].x < xMin)
				xMin = ctrlNet[i][j].x;
			if (ctrlNet[i][j].x > xMax)
				xMax = ctrlNet[i][j].x;
			if (ctrlNet[i][j].y < yMin)
				yMin = ctrlNet[i][j].y;
			if (ctrlNet[i][j].y > yMax)
				yMax = ctrlNet[i][j].y;
			if (ctrlNet[i][j].z < zMin)
				zMin = ctrlNet[i][j].z;
			if (ctrlNet[i][j].z > zMax)
				zMax = ctrlNet[i][j].z;
		}
	}

	double lookX = (xMax - xMin)*0.5;
	double lookY = (yMax - yMin)*0.5;
	double lookZ = (zMax - zMin)*0.5;

	double eyeX = lookX + radius*cos(phi)*sin(theta);
	double eyeY = lookY + radius*sin(phi)*sin(theta);
	double eyeZ = lookZ + radius*cos(theta);

	//Orbit the center of the scene.
	gluLookAt(eyeX, eyeY, eyeZ,
		lookX, lookY, lookZ,
		0.0, 1.0, 0.0);
		
	//----OBJECTS TO DISPLAY HERE----------------------------------------------------------

	glColor3f(0.0, 0.0, 0.0);
	drawControlNet(GL_RENDER);
	getControlNetStar();
	calcSurface(uResolution, vResolution);

	//---------------------------------------------------------------

	//because we are using GLUT_DOUBLE
	glutSwapBuffers();
	glFlush();
}

/* Main, calls everything we need. */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	MainWindow = glutCreateWindow("ECS 178 - Final Project");

	init();
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
	glutKeyboardFunc(keyboardFcn);
	glutMouseFunc(mouseFcn);
	glutSpecialFunc(specialFcn);

	glutMainLoop();

	return 0;
}