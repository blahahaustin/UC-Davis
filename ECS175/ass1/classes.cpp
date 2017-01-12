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

	Point() {}; //default constructor
	Point(int a, int b) //initialize the point
	{
		x = a;
		y = b;
	}

	//draw the point
	void drawPoint(pixel *PixelBuffer)
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

	Line() {}; //default constructor

	Line(Point point1, Point point2)
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

void Bresenham(pixel* PixelBuffer)
{
	if (m == 1) //handles slope of 1
	{
		int j = p1.y;

		for (int i = p1.x; i < p2.x; i++)
		{
			Point point(i, j);
			point.drawPoint(PixelBuffer);
			j++;
		}
	}
	if (m == -1) //handles slope of -1
	{
		int j = p1.y;

		for (int i = p1.x; i < p2.x; i++)
		{
			Point point(i, j);
			point.drawPoint(PixelBuffer);
			j--;
		}
	}

	//set points so that we can modify them
	int x0 = p1.x;
	int y0 = p1.y;

	int xEnd = p2.x;
	int yEnd = p2.y;

	//if slope is extreme, we want to
	//swap x and y when we plot them
	if (m > 1 | m < -1)
	{
		swap(x0, y0);
		swap(xEnd, yEnd);
	}

	//make sure that we go left -> right
	if (x0 > xEnd)
	{
		swap(x0, xEnd);
		swap(y0, yEnd);
	}

	//calculate dx and dy
	int dx = xEnd - x0;
	int dy = fabs(yEnd - y0);

	//calculate our error
	float p = dx / 2.0f;

	//decision structure for going up or down
	int ystep;
	if (y0 < yEnd)
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}

	int y = y0;

	//step through the row
	for (int x = x0; x < xEnd; x++)
	{
		//switch x and y for extreme slope
		if (m > 1 | m < -1)
		{
			Point point(y, x);
			point.drawPoint(PixelBuffer);
		}
		else //plot normally
		{
			Point point(x, y);
			point.drawPoint(PixelBuffer);
		}
		
		//subtract change in y from error
		p -= dy;
		if (p < 0) //want to add change in x if error is negative
		{
			y += ystep;
			p += dx;
		}
	}
}


};
