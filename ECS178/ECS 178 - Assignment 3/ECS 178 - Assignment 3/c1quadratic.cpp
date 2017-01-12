#include "ass3.h"

extern vector<point> ctrlPts;
extern point deriv;
extern point derivEnd;
extern bool rev;

vector<point> computeQuad(vector<double> t)
{
	//Fix the derivative so that it is in vector form.
	point p0prime;
	if (rev)
	{
		p0prime.x = derivEnd.x - ctrlPts[0].x;
		p0prime.y = derivEnd.y - ctrlPts[0].y;
	}
	else
	{
		p0prime.x = deriv.x - ctrlPts[0].x;
		p0prime.y = deriv.y - ctrlPts[0].y;
	}
	
	//Create vector to store the 2n bezier points.
	vector<point> bez;

	//Initialize the first three values in the vector.
	//b0
	bez.push_back(ctrlPts[0]);
	
	//b1
	point b1;
	b1.x = ctrlPts[0].x + ((t[1] - t[0]) /*/ 2.0*/)*p0prime.x;
	b1.y = ctrlPts[0].y + ((t[1] - t[0]) /*/ 2.0*/)*p0prime.y;
	//glColor3f(0.0, 1.0, 0.0);
	//plotPoint(b1);
	bez.push_back(b1);

	//b2
	bez.push_back(ctrlPts[1]);

	//For loop that adds all of the necessary points.
	for (unsigned int i = 1; i < ctrlPts.size() - 1; i++)
	{
		//b0
		//bez.push_back(ctrlPts[i]);

		//b1
		point p;
		p.x = ctrlPts[i].x + (t[i + 1] - t[i])*(bez[2 * i].x - bez[2 * i - 1].x);
		p.y = ctrlPts[i].y + (t[i + 1] - t[i])*(bez[2 * i].y - bez[2 * i - 1].y);
		//plotPoint(p);
		bez.push_back(p);

		//b2
		bez.push_back(ctrlPts[i + 1]);
	}

	/*for (unsigned int i = 0; i < bez.size() - 1; i++)
	{
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINES);
			glVertex2f(bez[i].x, bez[i].y);
			glVertex2f(bez[i + 1].x, bez[i + 1].y);
		glEnd();
	}*/

	for (unsigned int i = 0; i <= bez.size() - 2; i = i + 2)
	{	
		glColor3f(0.0, 1.0, 0.0);
		point b[3];
		b[0] = bez[i];
		b[1] = bez[i + 1];
		b[2] = bez[i + 2];
		bezier(b, 3, 500);
	}/**/
	return bez;
}

void average(vector<point> bFor, vector<point> bBack)
{
	vector<point> b;

	//Reverse the backward points for easier averaging.
	reverse(bBack.begin(), bBack.end());

	for (unsigned int i = 0; i < bFor.size(); i++)
	{
		point p;
		p.x = (bFor[i].x + bBack[i].x) / 2.0;
		p.y = (bFor[i].y + bBack[i].y) / 2.0;
		b.push_back(p);
		glColor3f(0.0, 1.0, 0.0);
		plotPoint(p);
	}

	for (unsigned int i = 0; i < b.size() - 1; i++)
	{
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINES);
			glVertex2f(b[i].x, b[i].y);
			glVertex2f(b[i + 1].x, b[i + 1].y);
		glEnd();
	}

	for (unsigned int i = 0; i <= b.size() - 2; i = i + 2)
	{
		point bez[3];
		bez[0] = b[i];
		bez[1] = b[i + 1];
		bez[2] = b[i + 2];
		bezier(bez, 3, 500);
	}

}