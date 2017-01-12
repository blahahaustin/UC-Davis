#pragma once
#ifndef ASS3_H
#define ASS3_H

#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;

class point
{
public:
	GLfloat x, y, z;
};

//implemented in OpenGL.cpp
void init(void);
void winReshapeFcn(GLint newWidth, GLint newHeight);
void keyboardFcn(unsigned char key, int x, int y);
void specialFcn(int key, int x, int y);
void mouseFcn(int button, int state, int xMouse, int yMouse);
void processPicks(GLint nPicks, GLuint pickBuffer[]);
void displayFcn(void);

//implemented in bezier.cpp
point GetOGLPos(int x, int y);
void bezier(point* ctrlPts, GLint nCtrlPts, GLint nBezCurvePts);
void computeBezPt(GLfloat u, point* bezPt, GLint nCtrlPts, point* ctrlPts, GLint* C);
void binomialCoeffs(GLint n, GLint *C);
void plotPoint(point pt);
void drawControls(GLenum mode);

//implemented in parameterize.cpp
vector<double> computeUni(void);
vector<double> computeChord(void);
vector<double> computeCent(void);

//implemented in c2cubic.cpp
vector<point> calculateDeBoor(vector<double> t);
point forwardDiff(vector<double> t);
point backwardDiff(vector<double> t);
void tridiagonal(double* a, double* b, double* c, point* d, int n);
void deBoorToBez(vector<point> d, vector<double> t);

//implemented in c1quadratic.cpp
vector<point> computeQuad(vector<double> t);
void average(vector<point> bFor, vector<point> bBack);

//implemented in deBoor.cpp
void deBoor(int k, vector<point> d, double u);

#endif