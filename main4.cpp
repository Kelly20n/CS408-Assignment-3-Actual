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
float calculateIncrement(float value1, float value2);
void initializeObject();
double B0(double uLine);
double B1(double uLine);
double B2(double uLine);
double B3(double uLine);


// add your global variables here
//float pointsX[100];
//float pointsY[100];
float age = 0;
float pointsX[11] = { 0.0f, 1.0f, 4.8f, 6.5f, 9.0f, 32.5f, 33.2f, 36.8f, 37.8f, 41.2f, 41.5f };
float pointsY[11] = { 0.0f, 3.5f, 1.8f, 7.0f, 3.5f, 4.8f, 2.6f, 7.0f, 5.0f, 20.5f, 21.5f };
float midPointsX[100];
float midPointsY[100];
int key = 0;
int numPoints = 11;
int iCounter = 0;
//int numPoints = 0;
//int pairNum = 1;
int pairNum = 6;
double linePoints = 100;
int p = 0;
int n = 0;
int restartO = 799;
float u = 0;
float objectX = 0;
float objectY = 0;
ObjModel Object;
Obj mainObject[800];


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
	initializeObject();
	Object.load("Tennis Ball.obj");
	initDisplay();
}

void initDisplay ()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
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
	cout << "We in here" << endl;
	cout << "Age: " << age << endl;
	//cout << glutGet(GLUT_ELAPSED_TIME) << endl;
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
	drawControlPoints();
	drawLine();
	

	cout << "I am here" << endl;

	//if (age < 60 * pairNum - 1)
	//{
		while (age > mainObject[n].frame)
		{
			p = n;
			n++;
		}

		cout << "Main Object: " << mainObject[p].frame << endl;
		u = (age - mainObject[p].frame) / (mainObject[n].frame - mainObject[p].frame);
		
		cout << "u: " << u << endl;

		/*objectX = (1 - u) * mainObject[p].valueX + u * mainObject[n].valueX;
		objectY = (1 - u) * mainObject[p].valueY + u * mainObject[n].valueY;*/

		/*objectX = B0(u) * pointsX[p] +
			B1(u) * pointsX[p + 1] +
			B2(u) * pointsX[p + 2] +
			B3(u) * pointsX[p + 3];

		objectY = B0(u) * pointsY[p] +
			B1(u) * pointsY[p + 1] +
			B2(u) * pointsY[p + 2] +
			B3(u) * pointsY[p + 3];*/
	//}
	glPushMatrix();
		glColor3d(1.0, 1.0, 0.0);
		glTranslated(mainObject[p].valueX, mainObject[p].valueY, 0.0);
		glRotated(0.0, 0.0, 0.0, 0.0);
		glScaled(0.01, 0.01, 0.01);
		Object.draw();
	glPopMatrix();



	if (age == restartO)
	{
		age = 0;
		p = 0;
		n = 0;
		cout << "WOWOWOWOWOWOW " << p << " " << n << endl;
	}
	
	// draw a purple wireframe cube
	/*glColor3d(1.0, 0.0, 1.0);
	glPushMatrix();
		glTranslated(0.0, 0.0, 0.0);
		glRotated(0.0, 0.0, 0.0, 0.0);
		glScaled(1.0, 1.0, 1.0);
		glutWireCube(1.0);
	glPopMatrix();*/

	

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
		glColor3d(1.0, 1.0, 1.0);
		glVertex3d(pointsX[i], pointsY[i], 0.0);
		//glVertex3d(midPointsX[i], midPointsY[i], 0.0);
	}
	glEnd();
}

//void drawLine()
//{
//	float x = 0;
//	float y = 0;
//	float incrementX = 0;
//	float incrementY = 0;
//
//	glBegin(GL_POINTS);
//	for (int i = 0; i < pairNum - 1; i++)
//	{
//		incrementX = calculateIncrement(midPointsX[i], midPointsX[i + 1]);
//		incrementY = calculateIncrement(midPointsY[i], midPointsY[i + 1]);
//		for(int j = 0; j < linePoints; j++)
//		{
//			x = midPointsX[i] + (j * incrementX);
//			y = midPointsY[i] + (j * incrementY);
//
//			glColor3d(0.0, 1.0, 0.0);
//			glVertex3d(x, y, 0.0);
//		}
//	}
//	glEnd();
//}

void calculateMiddlePoint()
{
	float temp = 0;
	int a = 1;

	midPointsX[0] = pointsX[0];
	midPointsY[0] = pointsY[0];

	for (int i = 1; i < numPoints; i++)
	{
		/*cout << pointsX[i + 1] << endl;
		cout << pointsX[i] << endl;*/
		temp = (pointsX[i + 1] - pointsX[i]) / 2;
		midPointsX[a] = pointsX[i + 1] - temp;
		cout << "ELSE" << endl;
		cout << temp << endl;
		cout << "Midpoint X: " << midPointsX[a] << endl;

		temp = (pointsY[i + 1] - pointsY[i]) / 2;
		midPointsY[a] = pointsY[i + 1] - temp;
		cout << "Midpoint Y: " << midPointsY[a] << endl;
		i++;
		a++;

		//cout << "A: " << a << endl;
	}
}

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

float calculateIncrement(float value1, float value2)
{
	float temp = 0;

	temp = (value2 - value1) / linePoints;

	return temp;
}

void initializeObject()
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

			mainObject[iCounter].frame = iCounter;
			mainObject[iCounter].valueX = x;
			mainObject[iCounter].valueY = y;

			/*cout << "Main Object X: " << mainObject[iCounter].valueX << endl;
			cout << "Main Object Y: " << mainObject[iCounter].valueY << endl;*/

			iCounter++;
		}
	}
}

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
