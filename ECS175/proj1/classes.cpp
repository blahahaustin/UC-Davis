#include<iostream>
#include<cmath>
#include<algorithm>
using namespace std;

//a struct will keep all of the values together in memory
struct pixel {
	float red;
	float green;
	float blue;
};

class Point
{
public:

	//takes an x and y, directly corresponding to pixel location
	int x, y;

	Point::Point() {}; //default constructor
	Point::Point(int a, int b) //initialize the point
	{
		x = a;
		y = b;
	}

	//draw the point
	void Point::drawPoint(pixel *PixelBuffer)
	{
		int pos = 0;
		/* 200 pixels in a row, multiplied by the number of rows
		   you would like to move up, add the number of columns over */
		pos = 200 * (y - 1) + (x - 1);

		//makes a white pixel
		PixelBuffer[pos].red = 1.0;
		PixelBuffer[pos].green = 1.0;
		PixelBuffer[pos].blue = 1.0;
	}
};

class Line
{
public:
	Point p1;
	Point p2;
	float dy;
	float dx;
	float m;

	Line::Line() {}; //default constructor

	Line::Line(Point point1, Point point2)
	{
		//initialize the points, make sure they're in right order
		if (point1.x > point2.x)
		{
			p2 = point1;
			p1 = point2;
		}
			else
		{
			p1 = point1;
			p2 = point2;
		}

		//calculate slope of the line
		dy = p2.y - p1.y;
		dx = p2.x - p1.x;
		m = dy / dx;
		cout << "The slope is " << m << endl;
	}

void Line::Bresenham(pixel* PixelBuffer)
{
	if (m == 1)
	{
		int j = p1.y;

		for (int i = p1.x; i < p2.x; i++)
		{
			Point point(i, j);
			point.drawPoint(PixelBuffer);
			j++;
		}
	}
	if (m == -1)
	{
		int j = p1.y;

		for (int i = p1.x; i < p2.x; i++)
		{
			Point point(i, j);
			point.drawPoint(PixelBuffer);
			j--;
		}
	}

	int x0 = p1.x;
	int y0 = p1.y;

	int xEnd = p2.x;
	int yEnd = p2.y;

	const bool largeSlope = (fabs(yEnd - y0) > fabs(xEnd - x0));
	if (largeSlope)
	{
		swap(x0, y0);
		swap(xEnd, yEnd);
	}

	if (x0 > xEnd)
	{
		swap(x0, xEnd);
		swap(y0, yEnd);
	}

	const float dx = xEnd - x0;
	const float dy = fabs(yEnd - y0);

	float error = dx / 2.0f;
	const int ystep = (y0 < yEnd) ? 1 : -1;
	int y = y0;

	const int maxX = xEnd;

	for (int x = x0; x < maxX; x++)
	{
		if (largeSlope)
		{
			Point point(y, x);
			point.drawPoint(PixelBuffer);
		}
		else
		{
			Point point(x, y);
			point.drawPoint(PixelBuffer);
		}

		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}


};
