#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <random>
#include <iostream>

static int refreshmill = 30;
const int screenWidth = 800;
const int screenHeight = 600;
const int maxNumberOfCubes = 30;
	
// [0][0] is xspeed and [0][1] is xposition of cube 1, rest are rgb colors
GLfloat xValues[maxNumberOfCubes][2];
// [0][0] is yspeed and [0][1] is yposition of cube 1
GLfloat yValues[maxNumberOfCubes][2];
// Array that stores r g b values for all cubes
GLfloat rgbValues[maxNumberOfCubes][3];

int numOfCreatedCubes = 0;
int numOfDeadCubes = 0;
GLfloat r, g, b;

// Function that updates the position of a specified cube and negates speed when the boarder of the screen is hit.
void move(int index) {
	xValues[index][1] += xValues[index][0]; yValues[index][1] += yValues[index][0];
	if (xValues[index][1] + 4.5 >= 80 || xValues[index][1] - 4.5 <= -80) {
		xValues[index][0] = -xValues[index][0];
		if (abs(xValues[index][0]) < 9) { xValues[index][0] = xValues[index][0] * 1.1; yValues[index][0] = yValues[index][0] * 1.1; }
		rgbValues[index][0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); rgbValues[index][1] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); rgbValues[index][2] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}
	if (yValues[index][1] + 4.5 >= 60 || yValues[index][1] - 4.5 <= -60) {
		yValues[index][0] = -yValues[index][0];
		if (abs(yValues[index][0]) < 9) { yValues[index][0] = yValues[index][0] * 1.1; xValues[index][0] = xValues[index][0] * 1.1; }
		rgbValues[index][0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); rgbValues[index][1] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); rgbValues[index][2] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}
}

// Function that generates a new cube and adds its information to the 3 two dimensional arrays
void addCube() {
	float xpos = -65.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (65.0 - (-65.0)))); float ypos = -55.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (55.0 - (-55.0))));

	for (int i = 0; i < maxNumberOfCubes; i++) {
		if (xValues[i][0] == 0) {
			xValues[i][0] = 1; xValues[i][1] = xpos;
			yValues[i][0] = 1; yValues[i][1] = ypos;
			numOfCreatedCubes++;
			// std::cout << "Cube Added to Array! (xpos:  " << xpos << " ypos: " << ypos << ")" << std::endl;
			return;
		}
	}
}
void draw() {
	//draw 80 x 60 rectangle at (x,y)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	// Adds a cube at the begining of the animation
	if (numOfCreatedCubes == 0) addCube();

	// Loop that renders all cubes that have not reached max speed and keeps track of all cubes that have reached max speed
	for (int i = 0; i < maxNumberOfCubes; i++) {
		if (xValues[i][0] != 0) {
			move(i);
			if (abs(xValues[i][0]) < 9) {
				glPushMatrix();
				glTranslatef(xValues[i][1], yValues[i][1], 0.0f);
				glColor3f(rgbValues[i][0], rgbValues[i][1], rgbValues[i][2]);
				glutSolidCube(9.0f);
				glPopMatrix();
			}
			else
			{
				if (abs(xValues[i][0]) != 10) {
					// std::cout << "CUBE DIED" << std::endl;
					numOfDeadCubes++;
					xValues[i][0] = 10;
				}
			}
		}
	}

	// If statement that spawns more cubes for all cubes that died in the previous for loop
	if (numOfCreatedCubes != maxNumberOfCubes && numOfDeadCubes != 0) {
		// std::cout << "TRYING TO SPAWN MORE CUBES" << std::endl;
		for (int i = 0; i < numOfDeadCubes; i++) { addCube(); addCube(); }
		numOfDeadCubes = 0;
	}
	glutSwapBuffers();
}

// Resize function that gets the screen created to the correct size
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-screenWidth / 10, screenWidth / 10, -screenHeight / 10, screenHeight / 10, 0.0, 6.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
// Timer function to refresh active window
void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(refreshmill, timer, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("DVD Bounce");
	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	return 0;
}