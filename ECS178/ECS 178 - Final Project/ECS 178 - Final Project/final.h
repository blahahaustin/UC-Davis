#pragma once
#ifndef FINAL_H
#define FINAL_H

#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class point
{
public:
	GLfloat x, y, z, w;
};


void init(void);
void getControlNet(void);
void getControlNetStar(void);
void drawControlNet(GLenum mode);
void calculateKnots(void);
point deBoor(int order, double* knots, vector<point> d, double uBar);
void calcSurface(int uRes, int vRes);
void processPicks(GLint nPicks, GLuint pickBuffer[]);



#endif