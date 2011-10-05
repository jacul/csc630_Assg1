/**
 *You can press Enter key to switch between fullscreen mode and normal mode.
 *author @xiangdongzhu
 */
#include <stdlib.h>			// standard definitions
#include <stdio.h>			// C I/O (for sprintf) 
#include <math.h>			// standard definitions

#ifdef __WIN32
#include <gl/gl.h>
#include <gl/glut.h>
#else
#include <GLUT/glut.h>			// GLUT
#include <OpenGL/glu.h>			// GLU
#include <OpenGL/gl.h>			// OpenGL
#endif

#define TRUE 1
#define FALSE 0

// global variable declarations

GLint TOGGLE_INTERVAL = 10000;
GLfloat windowHeight, windowWidth;
GLfloat triangleColor[] = {0,1.0,0};

GLint LPRESSED = FALSE;
GLint RPRESSED = FALSE;

GLint FULLSCREEN = FALSE;

GLfloat LINE_SEGMENT = 20.0;
GLfloat CIRCLE_X =0.0;
GLfloat CIRCLE_Y =0.0;
GLfloat DIAMOND_X = 0.0;
GLfloat DIAMOND_Y = 0.0;

//	Sets up some default OpenGL values.

void myInit()
{
    glClearColor(0, 0, 0, 1.0);		// background color
    glShadeModel(GL_SMOOTH);		// smooth shading
}

// reshape callback function

void myReshape(int winWidth, int winHeight) 
{
	
	windowHeight = winHeight;
	windowWidth = winWidth;
	
	// update projection
	glViewport (0, 0, winWidth,  winHeight);  // update the viewport
	glMatrixMode(GL_PROJECTION);              // update the projection
	glLoadIdentity();               
	
	gluOrtho2D(-4.0, 4.0, -4.0, 4.0);           // map unit square to viewport
	glMatrixMode(GL_MODELVIEW);
	
	glutPostRedisplay();                      // request redisplay
	
	
}

//draw a circle based on its radius and position, indeed, we are going to draw a bunch of lines.

void drawCircle(GLfloat r, GLfloat x, GLfloat y)
{
	const float DR = 3.14159/180;
	glBegin(GL_POLYGON);
	
	for (int i=0; i < 360; i+=(360/LINE_SEGMENT))
	{
		GLfloat deg = i * DR;
		glVertex2f(x + cos(deg)*r, y + sin(deg)*r);//calculate the position
	}
	
	glEnd();
}

// display callback function

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);       // clear the window
	
	if(LPRESSED){						//When left button is pressed, draw circle below the diamond
		glColor3f(0.0, 0.0, 1.0);       // set color to blue
		drawCircle(1.5,CIRCLE_X,CIRCLE_Y);				// draw a circle
	}
	
	/**
	 *	Draw diamond here.
	 */
    glColor3f(1.0, 0, 0);			// set color to red
    glBegin(GL_POLYGON);            // list the vertices to draw a diamond
	glVertex2f(DIAMOND_X + 0.0, DIAMOND_Y + 2.0);
	glVertex2f(DIAMOND_X + 1.5, DIAMOND_Y + 0.0);
	glVertex2f(DIAMOND_X + 0.0, DIAMOND_Y - 2.0);
	glVertex2f(DIAMOND_X - 1.5, DIAMOND_Y + 0.0);
	glEnd();
	
	if(!LPRESSED){
		glColor3f(0.0, 0.0, 1.0);       // set color to blue
		drawCircle(1.5, CIRCLE_X, CIRCLE_Y);// draw a circle
	}	
	
	/**
	 *  Draw triangle here
	 */
	glColor3f(0, triangleColor[1], 0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0.8);
	glVertex2f(0.8, -0.46);
	glVertex2f(-0.8, -0.46);
	glEnd();
	glFlush();				// force OpenGL to render now
	
	glutSwapBuffers();			// swap buffers
}

// keyboard callback function

void myKeyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
		case 'q':
		case 'Q':
			exit(0);			// exit
			break;
		case 'C':
		case 'c':
			if(LINE_SEGMENT==20.0)
				LINE_SEGMENT=100.0;
			else {
				LINE_SEGMENT=20.0;
			}
			break;
		case 13:
			if (FULLSCREEN) {
				glutReshapeWindow(400, 400);
				glutPositionWindow(100, 100);
				FULLSCREEN=FALSE;
			}else {
				glutFullScreen();
				FULLSCREEN=TRUE;
			}
			break;

	}
	
    glutPostRedisplay();		// request redisplay
}

void specialFunc(int key, int x, int y){
	GLfloat* pfloatx, *pfloaty;
	//get the address ahead so we don't need to do multiple comparisons.
	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
		pfloatx = &DIAMOND_X;
		pfloaty = &DIAMOND_Y;
	}else {
		pfloatx = &CIRCLE_X;
		pfloaty = &CIRCLE_Y;
	}
	
	switch (key) {
		case GLUT_KEY_LEFT:
			*pfloatx -= 0.1;
			break;
		case GLUT_KEY_RIGHT:
			*pfloatx += 0.1;
			break;
		case GLUT_KEY_UP:
			*pfloaty += 0.1;
			break;			
		case GLUT_KEY_DOWN:
			*pfloaty -= 0.1;
			break;
			
		default:
			break;
	}
	glutPostRedisplay();
}

// called if mouse clicks
void myMouse(int b, int s, int x, int y) {
	switch  ( b ) {    // b indicates the button
		case GLUT_LEFT_BUTTON:
			if (s == GLUT_DOWN)      // button pressed
				LPRESSED=TRUE;
			else if (s == GLUT_UP)   // button released
				LPRESSED=FALSE;
			break;
			
		case GLUT_RIGHT_BUTTON:
			if (s == GLUT_DOWN) {
				RPRESSED=TRUE;
			}else if (s == GLUT_UP) {
				RPRESSED=FALSE;
			}
			break;
			
	}
	glutPostRedisplay();
}

// called if mouse moves
void motionFunc(int x, int y){
	if (LPRESSED) {
		CIRCLE_X = x / windowWidth * 8.0 - 4.0;
		CIRCLE_Y = (windowHeight - y) / windowHeight * 8.0 - 4.0;
		glutPostRedisplay();
	}
	if (RPRESSED) {
		DIAMOND_X = x / windowWidth * 8.0 - 4.0;
		DIAMOND_Y = (windowHeight - y) / windowHeight * 8.0 - 4.0;
		glutPostRedisplay();
	}
}

// called if timer event occurs
void myTimeOut(int id) {
	// advance the state of animation incrementally
	triangleColor[1] = 1.0 - triangleColor[1];
	glutPostRedisplay();			  // request redisplay
	glutTimerFunc(TOGGLE_INTERVAL, myTimeOut, 0);  // request next timer event
}

// main program
// set up callbacks

int main(int argc, char **argv)
{
	
    glutInit(&argc, argv);   //initialize glut and gl
    glutInitDisplayMode(GLUT_DOUBLE |		// double buffering
						GLUT_RGB );		    // RGB color mode
	
    glutInitWindowSize(400, 400);   // 400x400 window
    glutInitWindowPosition(200,100);    // upper left
    glutCreateWindow("xiangdong zhu Assign1"); 	// create the window
	
	//register callbacks
    glutDisplayFunc(myDisplay);		//  call myDisplay to redraw window
    glutKeyboardFunc(myKeyboard);   // call myKeyboard when key is hit
	glutSpecialFunc(specialFunc);	//	special function key
    glutReshapeFunc(myReshape);     // call myReshape if window is resized
    glutMouseFunc(myMouse);         // call in mouse event 
	glutMotionFunc(motionFunc);		//	motion function
    glutTimerFunc(TOGGLE_INTERVAL, myTimeOut, 0);// call when this timer is triggered
	
    myInit();				// our own initializations
	
    glutMainLoop();			// pass control to GLUT, start it running
    return 0;               // ANSI C expects this
}
