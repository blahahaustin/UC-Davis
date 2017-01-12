#include "ass2.h"

extern vector<point> ctrlPts;

void raise()
{
	//temporary vector to hold the new control points
	vector<point> temp;

	//Set n to be one less than the size of the vector.
	int n = ctrlPts.size() - 1;

	//Add the first point to the vector.
	temp.push_back(ctrlPts.front());

	//Calculate the new points.
	for (unsigned int i = 1; i < ctrlPts.size(); i++)
	{
		point p;
		p.x = ((float)i / (float)(n + 1))*ctrlPts[i - 1].x + ((float)(n + 1 - i )/ (float)(n + 1))*ctrlPts[i].x;
		p.y = ((float)i / (float)(n + 1))*ctrlPts[i - 1].y + ((float)(n + 1 - i) / (float)(n + 1))*ctrlPts[i].y;
		//p.z = (i / (n + 1))*ctrlPts[i - 1].z + (1 - i / (n + 1))*ctrlPts[i].z;
		temp.push_back(p);
	}

	//Add the last point to the vector.
	temp.push_back(ctrlPts.back());
	
	//Clear and then update the vector of control points.
	ctrlPts.clear();
	ctrlPts = temp;
}

void lower()
{
	//Set n to be one less than the size of the vector.
	int n = ctrlPts.size() - 2;

	vector<point> temp;

	//Call the helper functions to calculate each side and put them in two temp vectors.
	vector<point> tempLeft = lowerLeft(n);
	vector<point> tempRight = lowerRight(n);

	for (int i = 0; i <= n; i++)
	{
		point p;
		p.x = 0.5 * (tempLeft[i].x + tempRight[n - i].x);
		p.y = 0.5 * (tempLeft[i].y + tempRight[n - i].y);
		temp.push_back(p);
	}

	//Clear and then update the vector of control points.
	ctrlPts.clear();
	ctrlPts = temp;

}

vector<point> lowerLeft(int n)
{
	//temporary vector to hold the new control points
	vector<point> temp;

	//add the first point to the vector, because they are the same
	temp.push_back(ctrlPts.front());

	//approximates the degree reduction from the left side
	for (unsigned int i = 1; i <= n; i++)
	{
		point p;
		p.x = ((float)(n + 1)) / ((float)(n + 1 - i))*ctrlPts[i].x - ((float)(i)) / ((float)(n + 1 - i))*temp[i - 1].x;
		p.y = ((float)(n + 1)) / ((float)(n + 1 - i))*ctrlPts[i].y - ((float)(i)) / ((float)(n + 1 - i))*temp[i - 1].y;
		temp.push_back(p);
	}

	return temp;
}

vector<point> lowerRight(int n)
{
	//temporary vector to hold the new control points
	vector<point> temp;

	//add the last point to the vector, because they are the same
	temp.push_back(ctrlPts.back());

	//need to keep track of numbering for the array
	int j = 0;

	//approximates the degree reduction from the right side
	for (unsigned int i = n; i > 0; i--)
	{
		point p;
		p.x = ((float)(n + 1)) / ((float)(i))*ctrlPts[i].x - ((float)(n + 1 - i)) / ((float)(i))*temp[j].x;
		p.y = ((float)(n + 1)) / ((float)(i))*ctrlPts[i].y - ((float)(n + 1 - i)) / ((float)(i))*temp[j].y;
		temp.push_back(p);

		//increase the index for the new temp vector
		j++;
	}

	return temp;
}