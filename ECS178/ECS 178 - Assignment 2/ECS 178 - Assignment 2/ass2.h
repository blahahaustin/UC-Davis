#pragma once
#ifndef ASS2_H
#define ASS2_H

#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class point
{
public:
	GLfloat x, y, z;
};


//implemented in bezier.cpp
point GetOGLPos(int x, int y);
void bezier(point* ctrlPts, GLint nCtrlPts, GLint nBezCurvePts);
void computeBezPt(GLfloat u, point* bezPt, GLint nCtrlPts, point* ctrlPts, GLint* C);
void binomialCoeffs(GLint n, GLint *C);
void plotPoint(point pt);
void drawControls(GLenum mode);

//implemented in degree.cpp
void raise();
void lower();
vector<point> lowerLeft(int n);
vector<point> lowerRight(int n);

//implemented in OpenGL.cpp
void init(void);
void winReshapeFcn(GLint newWidth, GLint newHeight);
void keyboardFcn(unsigned char key, int x, int y);
void specialFcn(int key, int x, int y);
void mouseFcn(int button, int state, int xMouse, int yMouse);
void processPicks(GLint nPicks, GLuint pickBuffer[]);
void displayFcn(void);

//implemented in aiken.cpp
vector<double> chord();
void aitken(double myT, vector<double> chords);

//implemented in hermite.cpp
void drawHermite(GLenum mode);
void vectorConvert();
void hermite();
void calcDerivative(vector<double> chords);





#endif