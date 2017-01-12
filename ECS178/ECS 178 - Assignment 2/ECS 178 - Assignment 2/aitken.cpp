#include "ass2.h"

extern vector<point> ctrlPts;
extern double t;

vector<double> chord()
{
	vector<double> chords;
	vector<double> chordsNormalized;
	double L = 0.0;

	//Get inital chord position.
	chords.push_back(L);

	//Calulate the total length of our polygon, while taking positions of the chords.
	for (unsigned int i = 1; i < ctrlPts.size(); i++)
	{
		//Calculate distances between points.
		double D;
		D = sqrt(pow((ctrlPts[i].x - ctrlPts[i - 1].x), 2) + pow((ctrlPts[i].y - ctrlPts[i - 1].y), 2));

		//Add distances to total.
		L = L + D;

		//Push chord position to our vector.
		chords.push_back(L);
	}

	//Get normalized value for chord position.
	for (unsigned int i = 0; i < chords.size(); i++)
	{
		chordsNormalized.push_back(chords[i] / L);
	}

	return chordsNormalized;

}

void aitken(double myT, vector<double> chords)
{
	int n = chords.size() - 1;

	//Temp vector to hold all of our new points.
	vector<point> temp = ctrlPts;

	//Double for-loop to calculate the needed points.
	for (unsigned int r = 1; r <= n; r++)
	{
		vector<point> v;
		for (unsigned int i = 0; i <= n - r; i++)
		{
			point p;
			p.x = (chords[i + r] - myT) / (chords[i + r] - chords[i])*temp[i].x
				+ (myT - chords[i]) / (chords[i + r] - chords[i])*temp[i + 1].x;
			p.y = (chords[i + r] - myT) / (chords[i + r] - chords[i])*temp[i].y
				+ (myT - chords[i]) / (chords[i + r] - chords[i])*temp[i + 1].y;
			v.push_back(p);
		}

		if (abs(myT - t) < 0.0001)
		{
			//Draw all the lines on the wanted T value.
			for (unsigned int i = 0; i < v.size() - 1; i++)
				{
					glLineWidth(1);
					glColor3f(0.0, 0.0, 1.0);
					glBegin(GL_LINES);
					glVertex3f(v[i].x, v[i].y, 0.0);
					glVertex3f(v[i + 1].x, v[i + 1].y, 0.0);
					glEnd();
				}
		}
		if (v.size() == 1)
		{
			glColor3f(1.0, 0.0, 0.0);
			plotPoint(v[0]);
		}

		temp.clear();
		temp = v;
	}

}