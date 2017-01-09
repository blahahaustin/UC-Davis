#include <GL/glut.h>
#include "classes.cpp"

pixel *PixelBuffer;
void display();

int main(int argc, char *argv[])
{
	//allocate new pixel buffer, need initialization!!
	PixelBuffer = new pixel[200 * 200 * 3];

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	//set window size to 200*200
	glutInitWindowSize(200, 200);
	//set window position
	glutInitWindowPosition(100, 100);

	//create and set main window title
	int MainWindow = glutCreateWindow("Hello Graphics!!");
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

	//points and stuff for the A
	Point p1(20, 70); Point p2(60, 180); Point p3(90, 70); Point p4(30, 100); Point p5(80, 100);
	Line line1(p1, p2); Line line2(p2, p3); Line line3(p4, p5);
	line1.Bresenham(PixelBuffer); line2.Bresenham(PixelBuffer); line3.Bresenham(PixelBuffer);

	//points and stuff for the B
	Point p6(100, 70); Point p7(100, 180); Point p8(150, 70);  Point p9(150, 180); Point p10(100, 120);
	Point p11(130, 120); Point p12(150, 100); Point p13(130, 120); Point p14(150, 140);
	Line line4(p6, p7); Line line5(p6, p8); Line line6(p7, p9); Line line7(p10, p11); Line line8(p11, p12);
	Line line9(p13, p14); Line line10(p12, p8); Line line11(p14, p9);
	line4.Bresenham(PixelBuffer); line5.Bresenham(PixelBuffer); line6.Bresenham(PixelBuffer);
	line7.Bresenham(PixelBuffer); line8.Bresenham(PixelBuffer); line9.Bresenham(PixelBuffer);
	line10.Bresenham(PixelBuffer); line11.Bresenham(PixelBuffer);

	//window refresh
	glFlush();
}