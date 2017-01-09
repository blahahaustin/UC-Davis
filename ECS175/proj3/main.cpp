#include <GL/glut.h>
#include<iostream>
using namespace std;

void display();
char color = 'w';

struct pixel {
	float red;
	float green;
	float blue;
};

pixel *PixelBuffer;
float *depthBuffer;

void setPixel(int x, int y, pixel *PixelBuffer)
{
	int pos = 0;
	/* 200 pixels in a row, multiplied by the number of rows
	you would like to move up, add the number of columns over */
	pos = 200 * (y - 1) + (x - 1);

	if (color == 'w')
	{
		//makes a white pixel
		PixelBuffer[pos].red = 1.0;
		PixelBuffer[pos].green = 1.0;
		PixelBuffer[pos].blue = 1.0;
	}
	else if(color == 'b')
	{
		//makes a white pixel
		PixelBuffer[pos].red = 0.0;
		PixelBuffer[pos].green = 0.0;
		PixelBuffer[pos].blue = 1.0;
	}
	else if (color == 'g')
	{
		//makes a white pixel
		PixelBuffer[pos].red = 0.0;
		PixelBuffer[pos].green = 1.0;
		PixelBuffer[pos].blue = 0.0;
	}
	else if (color == 'r')
	{
		//makes a white pixel
		PixelBuffer[pos].red = 1.0;
		PixelBuffer[pos].green = 0.0;
		PixelBuffer[pos].blue = 0.0;
	}
}

class point {
public:
	int x;
	int y;
	int z;

	point::point() {};
	point::point(int a, int b, int c) {
		x = a;
		y = b;
		z = c;
	}
};

class triangle {
public:
	point v[3];
	int r, s, t, k;

	triangle::triangle() {};//default constructor
	triangle::triangle(point a, point b, point c) {
		v[0] = a;
		v[1] = b;
		v[2] = c;

		//put the points in the right order every time
		for (int i = 1; i < 3; i++)
		{
			int j = i;
			while (j > 0 && v[j - 1].y > v[j].y)
			{
				swap(v[j], v[j - 1]);
				j--;
			}
		}

		point p1(v[0].x - v[1].x, v[0].y - v[1].y, v[0].z - v[1].z);
		point p2(v[0].x - v[2].x, v[0].y - v[2].y, v[0].z - v[2].z);
		r = p1.y*p2.z - p1.z*p2.y;
		s = p1.z*p2.x - p1.x*p2.z;
		t = p1.x*p2.y - p1.y*p2.x;
		k = r*v[0].x + s*v[0].y + t*v[0].z;
		cout << "r s t k = " << r << " " << s << " " << t << " " << k << endl;
		float invT = 1.0 / (float)t;
		int z = invT*(float)(k - r*v[1].x - s*v[1].y);
		cout << "invT = " << invT << endl;
		cout << "z = " << z << endl;

		/*/print y values 
		for (int i = 0; i < 3; i++)
		{
			cout << "v[" << i << "].x = " << v[i].x << endl;
			cout << "v[" << i << "].y = " << v[i].y << endl;
		}/*/
	}

	void scanFill()
	{
		if (v[0].y == v[1].y)
		{
			//cout << "flat bottom" << endl;
			scanFillBottomFlat();
		}
		else if (v[1].y == v[2].y)
		{ 
			//cout << "top flat" << endl;
			scanFillTopFlat();
		}
		else
		{
			//calculate new x and z values
			int x = (int)(v[2].x +
				((float)(v[1].y - v[2].y) / (float)(v[0].y - v[2].y))*
				(v[0].x - v[2].x));
			int z = (1 / t)*(k - r*x - s*v[1].y);

			//create new point to split triangles with
			point pNew(x, v[1].y, z);
			cout << "z = " << z << endl;

			//create two new triangles and fill them
			triangle bottomFlat(v[1], v[2], pNew);
			bottomFlat.scanFillBottomFlat();
			triangle topFlat(v[0], v[1], pNew);
			topFlat.scanFillTopFlat();
		}
	}

	void scanFillTopFlat()
	{
		if (v[1].x > v[2].x)
		{
			swap(v[1], v[2]);
		}

		float invM1 = (float)(v[0].x - v[1].x) / (float)(v[0].y - v[1].y);
		float invM2 = (float)(v[0].x - v[2].x) / (float)(v[0].y - v[2].y);
		float x1 = v[0].x;
		float x2 = v[0].x;

		for (int y = v[0].y; y <= v[2].y; y++)
		{
			setPixelScanLine((int)x1, (int)x2, y);
			x1 += invM1;
			x2 += invM2;
		}
	}

	void scanFillBottomFlat()
	{
		if (v[0].x > v[1].x)
		{
			swap(v[0], v[1]);
		}

		float invM1 = (float)(v[2].x - v[0].x)/(float)(v[2].y - v[0].y);
		float invM2 = (float)(v[2].x - v[1].x)/(float)(v[2].y - v[1].y);
		float x1 = v[0].x;
		float x2 = v[1].x;

		for (int y = v[0].y; y <= v[2].y; y++)
		{
			setPixelScanLine((int)x1, (int)x2, y);
			x1 += invM1;
			x2 += invM2;
		}
	}

	void setPixelScanLine(int x1, int x2, int y)
	{
		for (int x = x1; x <= x2; x++)
		{
			float invT = 1.0 / (float)t;
			int z = invT*(float)(k - r*x - s*y);
			int pos = 200 * (y - 1) + (x - 1);
			cout << "depthBuffer[" << pos << "] = " << z << endl;
			if (z > depthBuffer[pos])
			{
				setPixel(x, y, PixelBuffer);
				depthBuffer[pos] = z;
			}
		}
	}


};

int main(int argc, char *argv[])
{
	//allocate new pixel buffer and depth buffer!
	PixelBuffer = new pixel[200 * 200 * 3];
	depthBuffer = new float[200 * 200];
	for (int i = 0; i < 200; i++)
	{
		depthBuffer[i] = 0;
	}



	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	//set window size to 200*200
	glutInitWindowSize(200, 200);
	//set window position
	glutInitWindowPosition(100, 100);

	//create and set main window title
	int MainWindow = glutCreateWindow("ECS 175 - Assignment 2");
	glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL
							  //sets display function
	glutDisplayFunc(display);

	glutMainLoop();//main display loop, will display until terminate

	return 0;
}

//main display loop, this function will be called again and again by OpenGL
void display()
{
	//Misc.
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//draws pixel on screen, width and height must match pixel buffer dimension
	glDrawPixels(200, 200, GL_RGB, GL_FLOAT, PixelBuffer);

	//MY CODE GOES HERE
	color = 'b';
	point p1(25, 25, 100);
	point p2(175, 25, 50);
	point p3(100, 175, 200);
	triangle tri1(p1, p2, p3);
	tri1.scanFill();

	color = 'r';
	point p4(7, 50, 200);
	point p5(190, 50, 0);
	point p6(7, 10, 200);
	triangle tri2(p4, p5, p6);
	tri2.scanFill();





	//window refresh
	glFlush();
}