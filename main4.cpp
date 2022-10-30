//
//  main4.cpp
//

#include <cassert>
#include <cmath>
#include <string>
#include <iostream>
#include <cmath>
#include "ObjModel.h"
#include "TextureManager.h"
#include "glut.h"
#include "Sleep.h"
#include "Obj.h"
#include "Table.h"

#define GL_PI 3.141592654

using namespace std;
using namespace ObjLibrary;

void init ();
void initDisplay ();
void keyboard (unsigned char key, int x, int y);
void update ();
void reshape (int w, int h);
void display ();
void drawAxes ();
void drawControlPoints();
void drawLine();
void calculateMiddlePoint();
void initializeObject();
double B0(double uLine);
double B1(double uLine);
double B2(double uLine);
double B3(double uLine);
float calculateTotalDistance();
void initializeTable();
void printTable();
float getParameters(float arcL, int& index);
void parabolicEase();
void sinEase();


// add your global variables here
float age = 0; // Keep track of the frames
float pointsX[11] = { 0.0f, 1.0f, 4.8f, 6.5f, 9.0f, 32.5f, 33.2f, 36.8f, 37.8f, 41.2f, 41.5f };
float pointsY[11] = { 0.0f, 3.5f, 1.8f, 7.0f, 3.5f, 4.8f, 2.6f, 7.0f, 5.0f, 20.5f, 21.5f };
float midPointsX[100]; // Initially used to store the mid points between two control points
float midPointsY[100];
int key = 0; // Used for entry into something only once
int numPoints = 11; // Number of points
//int iCounter = 0;
int counter = 0; // Counter used in initializing table function
int pairNum = 6; // Number of pairs of control points
double linePoints = 100; // Number of points per segment
int p = 0; // Used in algorithms for animation
int n = 0; // Used in algorithms for animation
int restartO = 300; // Variable that decided at what frame number the animation would restart
float totalDistance = 0; // The total distance using the difference in middle points as distances
float u = 0; // The variable used in the drawing of the curve
float objectX = 0; // Variable used to temporarily store values
float objectY = 0;
float baseArcL = 0;
double unitsPF = 0.183752; // How many units per frame at 60FPS for lengths
ObjModel Object; // Instance of a model for the object
Obj mainObject[800]; // User created class used to store information about the object
Table mainTable[100]; // Used to hold data from the curve to attempt to make constant motion work



int main (int argc, char* argv[])
{
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("Loading OBJ Models");
	glutKeyboardFunc(keyboard);
	glutIdleFunc(update);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	init();

	// Commented out section below allowed for user input early on in my coding. The user could input all the control points they wanted in pairs.

	/*while (key == 0)
	{
		cout << "Enter the number of control points you would like: ";
		cin >> numPoints;

		cout << "Enter the x-axis value for the starting control point, control point 1: ";
		cin >> pointsX[0];

		cout << "Enter the y-axis value for the starting control point, control point 1: ";
		cin >> pointsY[0];
		
		for (int i = 1; i < numPoints; i++)
		{
			cout << "For control point pair " << pairNum << " enter the x - axis value for control point " << i + 1 << ": ";
			cin >> pointsX[i];
			cout << "For control point pair " << pairNum << " enter the y - axis value for control point " << i + 1 << ": ";
			cin >> pointsY[i];
			if (i + 1 == numPoints)
			{
				calculateMiddlePoint();
				key++;
			}
			if (i % 2 == 0)
			{
				pairNum++;
			}
		}
	}*/

	calculateMiddlePoint();

	glutMainLoop();  // contains an infinite loop, so it never returns
	return 1;
}

void init ()
{
	initDisplay();
	Object.load("Tennis Ball.obj");

	// In an attempt to get motion control working, tried following the hint to create a table whenever I uncomment the code below the program runs but the window for animation crashes and the program stops for some
	// reason, ended up giving up on getting the constant motion.

	initializeTable();
}

void initDisplay ()
{
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glColor3f(0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // on [ESC]
		exit(0); // normal exit
		break;
	}
}

void update ()
{
	// update your variables here
	sleep(1.0 / 60.0);
	age++;
	glutPostRedisplay();
}

void reshape (int w, int h)
{
	glViewport (0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / (GLdouble)h, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void display ()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// clear the screen - any drawing before here will not display at all

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 70.0,  // (x, y, z) camera eye position
	          0.0, 0.0, 0.0,  // (x, y, z) camera look at position
	          0.0, 1.0, 0.0); // (x, y, z) camera up direction

	// camera is now set up - any drawing before here will display incorrectly

	// display positive X, Y, and Z axes near origin
	drawAxes();
	drawControlPoints(); // Call function to draw control points
	drawLine(); // Call function to draw curve

	// Code below uses the algorithms given to us

	/*while (age > mainObject[n].frame)
	{
		p = n;
		n++;
	}
	if (p == n)
	{
		u = 0;
	}
	else
	{
		u = (age - mainObject[p].frame) / (mainObject[n].frame - mainObject[p].frame);
	}*/
		
	// code commented out below allows for constant motion along a straight line but of course not a curved line.

	/*objectX = (1 - u) * mainObject[p].valueX + u * mainObject[n].valueX;
	objectY = (1 - u) * mainObject[p].valueY + u * mainObject[n].valueY;*/

	if (age == 300)
	{
		age = 0;
		baseArcL = 0;
	}

	u = getParameters(baseArcL, p);
	baseArcL += 0.1666;

	

	objectX = B0(u) * pointsX[p] +
		B1(u) * pointsX[p + 1] +
		B2(u) * pointsX[p + 2] +
		B3(u) * pointsX[p + 3];

	objectY = B0(u) * pointsY[p] +
		B1(u) * pointsY[p + 1] +
		B2(u) * pointsY[p + 2] +
		B3(u) * pointsY[p + 3];

	glPushMatrix();
		glColor3d(1.0, 1.0, 0.0);
		glTranslated(objectX, objectY, 0.0);
		glRotated(0.0, 0.0, 0.0, 0.0);
		glScaled(0.01, 0.01, 0.01);
		Object.draw();
	glPopMatrix();


	// If statement to restart animation
	if (age == restartO)
	{
		age = 0;
		p = 0;
		n = 0;
	}
	
	// send the current image to the screen - any drawing after here will not display
	glutSwapBuffers();
}

void drawAxes ()
{
	glBegin(GL_LINES);
		glColor3d(1.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(2.0, 0.0, 0.0);
		glColor3d(0.0, 1.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 2.0, 0.0);
		glColor3d(0.0, 0.0, 1.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 2.0);
	glEnd();
}

void drawControlPoints()
{
	glBegin(GL_POINTS);
	for (int i = 0; i < numPoints; i++)
	{
		glColor3d(0.0, 0.0, 0.0);
		glVertex3d(pointsX[i], pointsY[i], 0.0);
	}
	glEnd();
}
// Used when drawing a straight line through the middle of the control points
void calculateMiddlePoint()
{
	float temp = 0;
	int a = 1;

	midPointsX[0] = pointsX[0];
	midPointsY[0] = pointsY[0];

	for (int i = 1; i < numPoints; i++)
	{
		temp = (pointsX[i + 1] - pointsX[i]) / 2;
		midPointsX[a] = pointsX[i + 1] - temp;

		temp = (pointsY[i + 1] - pointsY[i]) / 2;
		midPointsY[a] = pointsY[i + 1] - temp;
		i++;
		a++;
	}
	totalDistance = calculateTotalDistance();
}

// Used to Draw the curve algorithm given to us
void drawLine()
{
	double x = 0;
	double y = 0;
	
	
	for (int i = 0; i < numPoints - 3; i++)
	{
		for (double j = 0; j < linePoints; j++)
		{
			double uLine = j / linePoints;
			x = B0(uLine) * pointsX[i] +
				B1(uLine) * pointsX[i + 1] +
				B2(uLine) * pointsX[i + 2] +
				B3(uLine) * pointsX[i + 3];

			y = B0(uLine) * pointsY[i] +
				B1(uLine) * pointsY[i + 1] +
				B2(uLine) * pointsY[i + 2] +
				B3(uLine) * pointsY[i + 3];


			glBegin(GL_POINTS);
			glColor3d(0.0, 1.0, 0.0);
			glVertex3d(x, y, 0.0);
			glEnd();
		}
	}
	
}

// Attempted to get motion control to work by storing a movement every animation so interpolation  but didn't work so it was scrapped

//void initializeObject()
//{
//	double x = 0;
//	double y = 0;
//	
//	for (int i = 0; i < numPoints - 3; i++)
//	{
//		for (double j = 0; j < linePoints; j++)
//		{
//			double uLine = j / linePoints;
//			x = B0(uLine) * pointsX[i] +
//				B1(uLine) * pointsX[i + 1] +
//				B2(uLine) * pointsX[i + 2] +
//				B3(uLine) * pointsX[i + 3];
//
//			y = B0(uLine) * pointsY[i] +
//				B1(uLine) * pointsY[i + 1] +
//				B2(uLine) * pointsY[i + 2] +
//				B3(uLine) * pointsY[i + 3];
//
//			mainObject[iCounter].frame = iCounter;
//			mainObject[iCounter].valueX = x;
//			mainObject[iCounter].valueY = y;
//
//			/*cout << "Main Object X: " << mainObject[iCounter].valueX << endl;
//			cout << "Main Object Y: " << mainObject[iCounter].valueY << endl;*/
//
//			iCounter++;
//		}
//	}
//}

// Initialize animation object
void initializeObject()
{
	double x = 0;
	double y = 0;

	float temp = 0;
	
	for (int i = 0; i < pairNum; i++)
	{
		temp += mainObject[i].distance / unitsPF; // Calculate the frame at which we would reach the one middle point if going a constant motion
		mainObject[i].frame = temp;
		mainObject[i].valueX = midPointsX[i];
		mainObject[i].valueY = midPointsY[i];

	}
}


// Functions given to us to create the curve
double B0(double uLine)
{
	
	return pow((1-uLine), 3.0) / 6;
}

double B1(double uLine)
{
	return (3 * pow(uLine, 3.0) - 6 * pow(uLine, 2.0) + 4) / 6;
}

double B2(double uLine)
{
	return (-3 * pow(uLine, 3.0) + 3 * pow(uLine, 2.0) + 3 * uLine + 1) / 6;
}

double B3(double uLine)
{
	return pow(uLine, 3.0) / 6;
}


// Used to store the distance of each segment using a straight line instead of curve though
float calculateTotalDistance()
{
	float temp = 0;


	for (int i = 0; i < pairNum - 1; i++)
	{
		mainObject[i+1].distance = sqrt(pow(midPointsX[i] - midPointsX[i + 1], 2.0) + pow(midPointsY[i] - midPointsY[i + 1], 2.0));
		temp += sqrt(pow(midPointsX[i] - midPointsX[i+1], 2.0)  + pow(midPointsY[i] - midPointsY[i+1], 2.0));
	}

	initializeObject();
	return temp;
}


// Function to create table to implement constant motion, was able to store the points at equal spaced u values and store all the values properly but once attempting to run the program after initializing
// Program would crash if above attempting to do 20 evenly spaced intervals or else a smoother constant motion could've been achieved.
void initializeTable()
{
	double x = 0;
	double y = 0;
	double tempX = 0;
	double tempY = 0;

	tempX = B0(0) * pointsX[0] +
		B1(0) * pointsX[0 + 1] +
		B2(0) * pointsX[0 + 2] +
		B3(0) * pointsX[0 + 3];

	tempY = B0(0) * pointsY[0] +
		B1(0) * pointsY[0 + 1] +
		B2(0) * pointsY[0 + 2] +
		B3(0) * pointsY[0 + 3];

	for (int i = 0; i < numPoints - 3; i++)
	{
		for (int j = 0; j < linePoints; j++)
		{
			double uLine = j / linePoints;
			x = B0(uLine) * pointsX[i] +
				B1(uLine) * pointsX[i + 1] +
				B2(uLine) * pointsX[i + 2] +
				B3(uLine) * pointsX[i + 3];

			y = B0(uLine) * pointsY[i] +
				B1(uLine) * pointsY[i + 1] +
				B2(uLine) * pointsY[i + 2] +
				B3(uLine) * pointsY[i + 3];
			if (j % 10 == 0)
			{
				mainTable[counter].segNum = i;
				mainTable[counter].u = uLine;
				mainTable[counter].pointX = x;
				mainTable[counter].pointY = y;
				if (counter == 0)
				{
					mainTable[counter].length = sqrt(pow(tempX - x, 2.0) + pow(tempY - y, 2.0));
					mainTable[counter].arcLength = mainTable[counter].length;
				}
				else
				{
					mainTable[counter].length = sqrt(pow(mainTable[counter - 1].pointX - x, 2.0) + pow(mainTable[counter - 1].pointY - y, 2.0));
					mainTable[counter].arcLength = mainTable[counter].length + mainTable[counter - 1].arcLength;
				}
				counter++;
				cout << "Counter: " << counter << endl;
			}
		}
	}
	printTable();
}

// Program to print all the values from the table
void printTable()
{
	for (int i = 0; i < counter; i++)
	{
		cout << "Seg Num: " << mainTable[i].segNum << " U value: " << mainTable[i].u << " Point X: " << mainTable[i].pointX << " Point Y " << mainTable[i].pointY << " Length: " << mainTable[i].length << " Arc Length " << mainTable[i].arcLength << endl;
	}
}

// Function to find the parameters to do constant motion, motion is choppy because not enough points to interpret from also couldn't derivec a formula where I could estimate the value of u with a given arclength
// Lots of research and time to try and estimate u given arclength but no success
float getParameters(float arcL, int& index)
{
	float tempU = 0;
	for (int i = 0; i < counter; i++)
	{
		if (i + 1 != counter)
		{
			if (arcL >= mainTable[i].arcLength && arcL <= mainTable[i + 1].arcLength)
			{
				if (abs(arcL - mainTable[i].arcLength > arcL - mainTable[i + 1].arcLength))
				{
					index = mainTable[i+1].segNum;
					tempU = mainTable[i + 1].u;


				}
				else
				{
					index = mainTable[i].segNum;
					tempU = mainTable[i].u;
				}
			}
		}
	}

	

	return tempU;
}

// Function for parabolic ease-in/ease-out I was never able to implement part c 
void parabolicEase()
{

}

void sinEase()
{

}