#include "ass3.h"

extern vector<point> ctrlPts;

vector<point> calculateDeBoor(vector<double> t)
{
	vector<double> alpha;
	vector<double> beta;
	vector<double> gamma;
	vector<point> points;

	int n = ctrlPts.size() - 1;

	double dMinus2, dMinus1, d0, d1, dnMinus1, dn, dnPlus1;
	dMinus2 = (t[0] - (t[1] - t[0])) - (t[0] - 2*(t[1] - t[0]));
	dMinus1 = t[0] - (t[0] - (t[1] - t[0]));
	d0 = t[1] - t[0]; 
	d1 = t[2] - t[1];
	dnMinus1 = t[n - 1] - t[n - 2];
	dn = t[n] - t[n - 1];
	dnPlus1 = t[n] + (t[n] - t[n - 1]);

	//Insert the initial values to the matrix.
	points.push_back(forwardDiff(t));
	alpha.push_back(0.0);
	beta.push_back(d0 / (dMinus1 + d0 + d1));
	gamma.push_back((dMinus2 + dMinus1) / (dMinus2 + dMinus1 + d0));

	//Loop that calculates the inner coefficients of the matrix.
	for (unsigned int i = 0; i <= n; i++)
	{
		double ti, tiPlus1, tiPlus2, tiMinus1, tiMinus2;
		double di, diPlus1, diMinus1, diMinus2;

		if (i == 0)
		{
			ti = t[0];
			tiPlus1 = t[1];
			tiPlus2 = t[2];
			tiMinus1 = t[0] - (t[1] - t[0]);
			tiMinus2 = t[0] - 2*(t[1] - t[0]);
		}
		else if (i == 1)
		{
			ti = t[1];
			tiPlus1 = t[2];
			tiMinus1 = t[0];
			tiMinus2 = t[0] - (t[1] - t[0]);
			if (n == 2)
			{
				tiPlus2 = t[2] + (t[2] - t[1]);
			}
			else
			{
				tiPlus2 = t[3];
			}
		}
		else if (i == n - 1)
		{
			ti = t[n - 1];
			tiPlus1 = t[n];
			tiPlus2 = t[n] + (t[n] - t[n - 1]);
			tiMinus1 = t[n - 2];
			if (n == 2)
			{
				tiMinus2 = t[0] - (t[1] - t[0]);
			}
			else
			{
				tiMinus2 = t[n - 3];
			}
		}
		else if (i == n)
		{
			ti = t[n];
			tiPlus1 = t[n] + (t[n] - t[n - 1]);
			tiPlus2 = t[n] + 2*(t[n] - t[n - 1]);
			tiMinus1 = t[n - 1];
			tiMinus2 = t[n - 2];
		}
		else
		{
			ti = t[i];
			tiPlus1 = t[i + 1];
			tiPlus2 = t[i + 2];
			tiMinus1 = t[i - 1];
			tiMinus2 = t[i - 2];
		}

		di = tiPlus1 - ti;
		diPlus1 = tiPlus2 - tiPlus1;
		diMinus1 = ti - tiMinus1;
		diMinus2 = tiMinus1 - tiMinus2;

		points.push_back(ctrlPts[i]);

		double a = pow(di, 2) / (diMinus2 + diMinus1 + di);
		a /= (diMinus1 + di);
		alpha.push_back(a);

		double b = ((di *(diMinus2 + diMinus1)) / (diMinus2 + diMinus1 + di))
			+ ((diMinus1 * (di + diPlus1)) / (diMinus1 + di + diPlus1));
		b /= (diMinus1 + di);
		beta.push_back(b);

		double c = pow(diMinus1, 2) / (diMinus1 + di + diPlus1);
		c /= (diMinus1 + di);
		gamma.push_back(c);
	}

	//Insert the last values to the matrix.
	points.push_back(backwardDiff(t));
	alpha.push_back((dn + dnPlus1) / (dnMinus1 + dn + dnPlus1));
	beta.push_back(dnPlus1 / (dnMinus1 + dn + dnPlus1 ));
	gamma.push_back(0.0);

	//Call function to solve tridiagonal matrix.
	tridiagonal(alpha.data(), beta.data(), gamma.data(), points.data(), n + 2);

	for (unsigned int i = 0; i < points.size(); i++)
	{
		glColor3f(1.0, 0.0, 1.0);
		plotPoint(points[i]);
	}

	for (unsigned int i = 0; i < points.size() - 1; i++)
	{
		glLineWidth(2);
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_LINES);
			glVertex2f(points[i].x, points[i].y);
			glVertex2f(points[i + 1].x, points[i + 1].y);
		glEnd();
	}

	return points;
}

void deBoorToBez(vector<point> d, vector<double> t)
{
	int n = ctrlPts.size() - 1;
	point b0, b1, b2, b3;

	for (unsigned int i = 0; i < n; i++)
	{
		point b0, b1, b2, b3;
		vector<point> bez;
		double ti, tiPlus1, tiPlus2, tiMinus1;
		double di, diPlus1, diMinus1;

		if (i == 0)
		{
			ti = t[0];
			tiPlus1 = t[1];
			tiPlus2 = t[2];
			tiMinus1 = t[0] - (t[1] - t[0]);
		}
		else if (i == 1)
		{
			ti = t[1];
			tiPlus1 = t[2];
			tiMinus1 = t[0];
			if (n == 2)
			{
				tiPlus2 = t[2] + (t[2] - t[1]);
			}
			else
			{
				tiPlus2 = t[3];
			}
		}
		else if (i == n - 1)
		{
			ti = t[n - 1];
			tiPlus1 = t[n];
			tiPlus2 = t[n] + (t[n] - t[n - 1]);
			tiMinus1 = t[n - 2];
		}
		else if (i == n)
		{
			ti = t[n];
			tiPlus1 = t[n] + (t[n] - t[n - 1]);
			tiPlus2 = t[n] + 2 * (t[n] - t[n - 1]);
			tiMinus1 = t[n - 1];
		}
		else
		{
			ti = t[i];
			tiPlus1 = t[i + 1];
			tiPlus2 = t[i + 2];
			tiMinus1 = t[i - 1];
		}

		di = tiPlus1 - ti;
		diPlus1 = tiPlus2 - tiPlus1;
		diMinus1 = ti - tiMinus1;


		b0 = ctrlPts[i];
		bez.push_back(b0);

		b1.x = ((di + diPlus1) / (diMinus1 + di + diPlus1))*d[i + 1].x
			+ ((diMinus1) / (diMinus1 + di + diPlus1))*d[i + 2].x;
		b1.y = ((di + diPlus1) / (diMinus1 + di + diPlus1))*d[i + 1].y
			+ ((diMinus1) / (diMinus1 + di + diPlus1))*d[i + 2].y;
		glColor3f(0.0, 0.0, 0.0);
		plotPoint(b1);
		bez.push_back(b1);

		b2.x = ((diPlus1) / (diMinus1 + di + diPlus1))*d[i + 1].x
			+ ((diMinus1 + di) / (diMinus1 + di + diPlus1))*d[i + 2].x;
		b2.y = ((diPlus1) / (diMinus1 + di + diPlus1))*d[i + 1].y
			+ ((diMinus1 + di) / (diMinus1 + di + diPlus1))*d[i + 2].y;
		glColor3f(0.0, 0.0, 0.0);
		plotPoint(b2);
		bez.push_back(b2);

		b3 = ctrlPts[i + 1];
		bez.push_back(b3);

		for (unsigned int i = 0; i < bez.size() - 1; i++)
		{
			glLineWidth(2);
			glColor3f(0.0, 0.0, 1.0);
			glBegin(GL_LINES);
			glVertex2f(bez[i].x, bez[i].y);
			glVertex2f(bez[i + 1].x, bez[i + 1].y);
			glEnd();
		}

		bezier(bez.data(), 4, 500);

	}
}

//Function calculates and returns the derivative for p0.
point forwardDiff(vector<double> t)
{
	point deriv, p;
	double diMinus1, ti, tiMinus1;

	ti = t[0];
	tiMinus1 = t[0] - (t[1] - t[0]);
	diMinus1 = ti - tiMinus1;

	deriv.x = ((double)(ctrlPts[1].x - ctrlPts[0].x) / (double)(t[1] - t[0]));
	deriv.y = ((double)(ctrlPts[1].y - ctrlPts[0].y) / (double)(t[1] - t[0]));
	//cout << p.x << ", " << p.y << endl;

	p.x = ctrlPts[0].x - (diMinus1 / 3.0)*deriv.x;
	p.y = ctrlPts[0].y - (diMinus1 / 3.0)*deriv.y;
	//glColor3f(1.0, 0.0, 0.0);
	//plotPoint(p);


	return p;
}

//Function calculates and returns the derivative for pn.
point backwardDiff(vector<double> t)
{
	int n = t.size() - 1;
	point deriv, p;
	double di, tiPlus1, ti;

	tiPlus1 = t[n] + (t[n] - t[n - 1]);
	ti = t[n];
	di = tiPlus1 - ti;

	deriv.x = ((double)(ctrlPts[n].x - ctrlPts[n - 1].x) / (double)(t[n] - t[n - 1]));
	deriv.y = ((double)(ctrlPts[n].y - ctrlPts[n - 1].y) / (double)(t[n] - t[n - 1]));

	p.x = ctrlPts[n].x + (di / 3.0)*deriv.x;
	p.y = ctrlPts[n].y + (di / 3.0)*deriv.y;
	//glColor3f(1.0, 0.0, 0.0);
	//plotPoint(p);

	return p;
}

void tridiagonal(double* a, double* b, double* c, point* d, int n)
{
	c[0] /= b[0];

	d[0].x /= b[0];
	d[0].y /= b[0];

	//cout << "d[0].x = " << d[0].x  << endl;
	//cout << "d[0].y = " << d[0].y << endl;

	for (int i = 1; i < n; i++)
	{
		c[i] /= b[i] - a[i] * c[i - 1];

		d[i].x = (d[i].x - a[i] * d[i - 1].x) / (b[i] - a[i] * c[i - 1]);
		d[i].y = (d[i].y - a[i] * d[i - 1].y) / (b[i] - a[i] * c[i - 1]);

		//cout << "d[" << i << "].x = " << d[0].x  << endl;
		//cout << "d[" << i << "].y = " << d[0].y << endl;
	}

	d[n].x = (d[n].x - a[n] * d[n - 1].x) / (b[n] - a[n] * c[n - 1]);
	d[n].y = (d[n].y - a[n] * d[n - 1].y) / (b[n] - a[n] * c[n - 1]);

	//cout << "d[n].x = " << d[n].x << endl;
	//cout << "d[n].y = " << d[n].y << endl;

	for (int i = n; i-- > 0;)
	{
		d[i].x -= c[i] * d[i + 1].x;
		d[i].y -= c[i] * d[i + 1].y;
	}

}