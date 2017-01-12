#include "ass2.h"
using namespace std;

//Set integer variables for the windows.
int MainWindow;

//Set initial size of the display window.
GLsizei winWidth = 600, winHeight = 600;


/* Main, calls everything we need. */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	MainWindow = glutCreateWindow("Bezier Curves");

	init();
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
	glutKeyboardFunc(keyboardFcn);
	glutMouseFunc(mouseFcn);
	glutSpecialFunc(specialFcn);

	glutMainLoop();

	return 0;
}