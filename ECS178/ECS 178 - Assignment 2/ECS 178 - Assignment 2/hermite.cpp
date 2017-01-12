#include "ass2.h"

extern vector<point> ctrlPts;
extern vector<point> c1p;
extern vector<point> c1m;
extern vector<point> norm;
extern int mode;

void drawHermite(GLenum mode)
{
	/* Draw the points specified by the user. */
	for (unsigned int i = 0; i < c1p.size(); i++)
	{
		/* If in select mode, give the point a name! */
		if (mode == GL_SELECT)
			glPushName(i);
		glPointSize(5);
		glColor3f(0.0, 0.0, 0.0);
		plotPoint(c1p[i]);
	}

	/* Draw the derivative points specified by the user. */
	for (unsigned int i = 0; i < c1m.size(); i++)
	{
		/* If in select mode, give the point a name! */
		if (mode == GL_SELECT)
			glPushName(i + c1p.size());
		glPointSize(5);
		glColor3f(0.0, 0.0, 1.0);
		plotPoint(c1m[i]);
	}

	/* Draw the tangent vector, if the points are the same. */
	if (c1p.size() > 0 && c1m.size() > 0 && c1p.size() == c1m.size())
	{
		for (unsigned int i = 0; i < c1p.size(); i++)
		{
			glLineWidth(2);
			glColor3f(0.0, 0.0, 1.0);
			glBegin(GL_LINES);
			glVertex2f(c1p[i].x, c1p[i].y);
			glVertex2f(c1m[i].x, c1m[i].y);
			glEnd();
		}
	}
	//glutPostRedisplay();
}

void vectorConvert()
{
	vector<point> v;

	//Calculate the difference between the point and the vector.
	for (unsigned int i = 0; i < c1p.size(); i++)
	{
		point p;

		p.x = c1m[i].x - c1p[i].x;
		p.y = c1m[i].y - c1p[i].y;

		v.push_back(p);
	}

	norm = v;
}

void hermite()
{
	//For each pair of Hermite points and their derivatives.
	for (unsigned int i = 0; i < c1p.size() - 1; i++)
	{
		vector<point> b;
		point b0, b1, b2, b3;

		b0 = c1p[i];
		b.push_back(b0);
		
		b1.x = c1p[i].x + 0.33*norm[i].x;
		b1.y = c1p[i].y + 0.33*norm[i].y;
		b.push_back(b1);

		b2.x = c1p[i + 1].x + 0.33*norm[i + 1].x;
		b2.y = c1p[i + 1].y + 0.33*norm[i + 1].y;
		b.push_back(b2);

		b3 = c1p[i + 1];
		b.push_back(b3);

		bezier(b.data(), 4, 500);
	}
	if (mode == 4)
	{
		c1p.clear();
		c1m.clear();
		norm.clear();
	}
}

void calcDerivative(vector<double> chords)
{
	c1p = ctrlPts;
	
	for (unsigned int i = 0; i < c1p.size(); i++)
	{
		point p;
		//Get the forward difference.
		if (i == 0)
		{
			p.x = (double)(c1p[i + 1].x - c1p[i].x) / (double)(chords[i + 1] - chords[i]);
			p.y = (double)(c1p[i + 1].y - c1p[i].y) / (double)(chords[i + 1] - chords[i]);
		}
		else if (i == c1p.size() - 1)
		{
			p.x = (double)(c1p[i].x - c1p[i - 1].x) / (double)(chords[i] - chords[i - 1]);
			p.y = (double)(c1p[i].y - c1p[i - 1].y) / (double)(chords[i] - chords[i - 1]);
		}
		else
		{
			p.x = ((c1p[i].x - c1p[i - 1].x)*(chords[i + 1] - chords[i]))
				/ ((chords[i] - chords[i - 1]) / (chords[i + 1] - chords[i - 1]))
				+ ((c1p[i + 1].x - c1p[i].x)*(chords[i] - chords[i - 1]))
				/ ((chords[i + 1] - chords[i])*(chords[i + 1] - chords[i - 1]));
			p.y = ((c1p[i].y - c1p[i - 1].y)*(chords[i + 1] - chords[i]))
				/ ((chords[i] - chords[i - 1]) / (chords[i + 1] - chords[i - 1]))
				+ ((c1p[i + 1].y - c1p[i].y)*(chords[i] - chords[i - 1]))
				/ ((chords[i + 1] - chords[i])*(chords[i + 1] - chords[i - 1]));
		}
		c1m.push_back(p);
	}


}