//lib includes
#include <glut.h> 
#include <math.h>
#include <vector>
#include <iostream>

//local includes
#include "Cloth.h"

#define TIME_STEPSIZE2 0.5f*0.5f

//creating a global cloth variable that can be assigned later
Cloth* cloth1;

//setting the ball position and size
Vec3 ball_pos(7, -5, 0);
float ball_radius = 2;

//setting the floor position and size
Vec3 floorPos(8, -70, 4);
float floorRadius = 55;

Vec3 windForceToApply= Vec3(0.5f, 0, 0.2f);

//various bool checks
bool ballCollision;
bool ballMove;
bool addForce;
bool addWind;

//variables to change parts of the cloth to make it larger or smaller
float widthOfCloth = 14;
float heightOfCloth = 10;
float numberOfParticlesWide = 55;
float numberOfParticlesHigh = 45;

float ball_time = 0;

void Render()
{
	//enable depth test so there is depth in the scene
	glEnable(GL_DEPTH_TEST);


	//moves the ball if enabled
	if (ballMove) {
		ball_time++;
		ball_pos.f[2] = cos(ball_time / 350.0) * 7;
	}
	//adds gravity if it is enabled
	if (addForce) {
		cloth1->addForce(Vec3(0, -0.2f, 0)*TIME_STEPSIZE2);
	}
	//adds wind if it is enabled
	if (addWind) {
		cloth1->windForce(windForceToApply*TIME_STEPSIZE2);
	}

	cloth1->timeStep();

	//checks the collision with the ball and the cloth if it is enabled
	if (ballCollision) {
		cloth1->ballCollision(ball_pos, ball_radius);
	}
	//checks the collision with the floor and the cloth
	cloth1->ballCollision(floorPos, floorRadius);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//load in an identity matrix
	glLoadIdentity();

	//move the camera
	glTranslatef(-6.5, 6, -9.0f);
	glRotatef(25, 0, 1, 0);

	//render the cloth
	cloth1->renderCloth();

	//create and render the floor using older opengl practices
	glPushMatrix();
	//set the floors position
	glTranslatef(floorPos.f[0], floorPos.f[1], floorPos.f[2]);
	//set the floors color
	glColor3f(0.4f, 0.8f, 0.5f);
	//create a sphere for the ball
	glutSolidSphere(floorRadius - 0.1, 50, 50);
	glPopMatrix();

	//create and render the ball using older opengl practices
	//set the balls position
	glTranslatef(ball_pos.f[0], ball_pos.f[1], ball_pos.f[2]);
	//set the balls color
	glColor3f(0.4f, 0.8f, 0.5f);
	//render a sphere with glut
	glutSolidSphere(ball_radius - 0.1, 50, 50);

	glutSwapBuffers();
	glutPostRedisplay();
}

//makes sure the screen viewport can be resized properly
void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h == 0)
		gluPerspective(80, (float)w, 1.0, 5000.0);
	else
		gluPerspective(80, (float)w / (float)h, 1.0, 5000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Input(unsigned char key, int x, int y)
{
	switch (key) {
	//escape quits the program
	case 27:
		exit(0);
		break;

#pragma region enable/disable forces

	//e makes the cloth drop
	case 101:
		cloth1->dropCloth();
		break;
		//c enables ball collision
	case 99:
		ballCollision = !ballCollision;
		break;
		//f makes it so the ball moves
	case 102:
		ballMove = !ballMove;
		break;
		//r resets the cloth to its starting point
	case 114:
		//deleting the values from cloth1
		delete cloth1;
		//creating a new cloth
		cloth1 = new Cloth(widthOfCloth, heightOfCloth, numberOfParticlesWide, numberOfParticlesHigh);
		windForceToApply = Vec3(0.5f, 0, 0.2f);
		break;
		//a adds gravity
	case 97:
		addForce = !addForce;
		break;
		//b adds wind
	case 98:
		addWind = !addWind;
		break;

#pragma endregion

#pragma region poly mode
		//q sets it to wireframe mode
	case 113:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
		//w sets it back to regular polygon mode
	case 119:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
#pragma endregion

#pragma region wind control
		//j decreases wind force on x axis
	case 106:
		windForceToApply = Vec3(windForceToApply.f[0] -= 0.1f, 0, windForceToApply.f[2]);
		break;
		//l increases wind force on x axis 
	case 108:
		windForceToApply = Vec3(windForceToApply.f[0] += 0.1f, 0, windForceToApply.f[2]);
		break;
		//k to increase z wind force
	case 107:
		windForceToApply = Vec3(windForceToApply.f[0], 0, windForceToApply.f[2] += 0.1f);
		break;
		//i to decrease z wind force
	case 105:
		windForceToApply = Vec3(windForceToApply.f[0], 0, windForceToApply.f[2] -= 0.1f);
		break;

#pragma endregion

#pragma region width and height of the cloth
	//1 to increase height
	case 49:
		heightOfCloth += 1;
		numberOfParticlesHigh += 1;
		delete cloth1;
		cloth1 = new Cloth(widthOfCloth, heightOfCloth, 55, 45);
		break;
		//3 to reduce width
	case 51:
		if (widthOfCloth > 0) {
			widthOfCloth -= 1;
			numberOfParticlesWide -= 1;
			delete cloth1;
			cloth1 = new Cloth(widthOfCloth, heightOfCloth, 55, 45);
		}
		break;
		//4 to increase width
	case 52:
		widthOfCloth += 1;
		numberOfParticlesWide += 1;
		delete cloth1;
		cloth1 = new Cloth(widthOfCloth, heightOfCloth, 55, 45);
		break;
		//2 to decrease height
	case 50:
		if (heightOfCloth > 0) {
			heightOfCloth -= 1;
			numberOfParticlesHigh -= 1;
			delete cloth1;
			cloth1 = new Cloth(widthOfCloth, heightOfCloth, 55, 45);
		}
		break;

#pragma endregion

	default:
		break;
	}
}

//start point
int main(int argc, char** argv)
{
	//assigning the cloth1 value
	cloth1 = new Cloth(14, 10, 55, 45);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//initialise opengl
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);

	//create the window
	glutCreateWindow("Cloth Simulation");
	//define the render function
	glutDisplayFunc(Render);
	//define the reshape function
	glutReshapeFunc(Reshape);
	//define the keyboard input function
	glutKeyboardFunc(Input);

	glutMainLoop();
}