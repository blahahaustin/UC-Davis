#include "ass3.h"

extern vector<point> ctrlPts;
extern point deriv;
extern point derivEnd;
extern int mode;
extern bool specified;
extern bool specifiedEnd;
extern bool dend;
int part = mode;

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

/* A function to plot points. */
void plotPoint(point pt)
{
	glBegin(GL_POINTS);
	glVertex2f(pt.x, pt.y);
	glEnd();
}

void drawControls(GLenum mode)
{
	int n = ctrlPts.size() - 1;

	/* Print out the control points specified by the user. */
	for (unsigned int i = 0; i < ctrlPts.size(); i++)
	{
		/* If in select mode, give the point a name! */
		if (mode == GL_SELECT)
			glPushName(i);
		glPointSize(5);
		glColor3f(0.0, 0.0, 0.0);
		plotPoint(ctrlPts[i]);
	}

	if (part == 3 && ctrlPts.size() > 0 && specified == true)
	{
		cout << "drawing the derivative" << endl;
		if (mode == GL_SELECT)
			glPushName(100);
		glPointSize(5);
		glColor3f(0.0, 0.0, 1.0);
		plotPoint(deriv);
		glBegin(GL_LINES);
		glVertex2f(ctrlPts[0].x, ctrlPts[0].y);
		glVertex2f(deriv.x, deriv.y);
		glEnd();
	}
	if (part == 3 && dend == true && specifiedEnd == true)
	{
		if (mode == GL_SELECT)
			glPushName(101);
		glPointSize(5);
		glColor3f(0.0, 0.0, 1.0);
		plotPoint(derivEnd);
		glBegin(GL_LINES);
		glVertex2f(ctrlPts[n].x, ctrlPts[n].y);
		glVertex2f(derivEnd.x, derivEnd.y);
		glEnd();
	}

	/* Draw control polygon between lines. */
	if (ctrlPts.size() > 1 && part == 2)
	{
		for (unsigned int i = 0; i < ctrlPts.size() - 1; i++)
		{
			glLineWidth(2);
			glColor3f(0.0, 0.0, 0.0);
			glBegin(GL_LINES);
			glVertex2f(ctrlPts[i].x, ctrlPts[i].y);
			glVertex2f(ctrlPts[i + 1].x, ctrlPts[i + 1].y);
			glEnd();
		}
	}

}