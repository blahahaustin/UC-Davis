#include "ass3.h"

void deBoor(int k, vector<point> d, double u)
{
	//Determine the number of knots!
	int n = d.size() - 1;
	int p = k;
	int m = n + p + 1;

	double* knots;
	knots = new double[m];

	//Calculate the knot bar so that the algorithm works!
	for (unsigned int i = 0; i <= p; i++)
	{
		knots[i] = 0.0;
	}
	for (unsigned int j = 1; j <= n - p; j++)
	{
		knots[j + p] = (double)j / (double)(n - p + 1);
	}
	for (unsigned int i = m - p; i <= m; i++)
	{
		knots[i] = 1.0;
	}

	//Need to find the interval I, such that the value of u
	//is in [uI, uI+1)
	int I = 0;
	for (unsigned int i = 0; i <= m; i++)
	{
		//cout << "In the loop." << endl;
		if (u > knots[i] && u <= knots[i + 1])
		{
			I = i;
		}
	}
	//cout << "I = " << I << endl;

	//2-dimensional array to hold our values.
	point myd[30][30];

	//Fill the first scheme in the array.
	for (unsigned int i = 0; i < d.size(); i++)
	{
		myd[0][i] = d[i];
	}

	for (unsigned int r = 1; r <= k - 1; r++)
	{
		//cout << "r = " << r << endl;
		for (unsigned int i = I - (k - 1); i <= (I - r); i++)
		{
			//cout << "i = " << i << endl;
			point temp;
			temp.x = ((knots[i + k] - u) / (knots[i + k] - knots[i + r]))*myd[r - 1][i].x
				+ ((u - knots[i + r]) / (knots[i + k] - knots[i + r]))*myd[r - 1][i + 1].x;
			temp.y = ((knots[i + k] - u) / (knots[i + k] - knots[i + r]))*myd[r - 1][i].y
				+ ((u - knots[i + r]) / (knots[i + k] - knots[i + r]))*myd[r - 1][i + 1].y;
			myd[r][i] = temp;
		}
	}
	glColor3f(1.0, 0.0, 0.0);
	plotPoint(myd[k - 1][I - (k - 1)]);
}