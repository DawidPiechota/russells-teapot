#pragma once

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>

class Spaceship
{
public:

	Spaceship(GLfloat posX, GLfloat posY, GLfloat posZ, float thrustPower);
	~Spaceship();

	struct velocityStruct
	{
		GLfloat X;
		GLfloat Y;		//nieuzywane
		GLfloat Z;
	};
	struct positionStruct
	{
		GLfloat X;
		GLfloat Y;		//nieuzywane
		GLfloat Z;
	};
	struct orientationStruct
	{
		GLfloat X;
		GLfloat Y;		//nieuzywane
		GLfloat Z;
	};
	struct colorStruct	//nieuzywane
	{
		GLfloat red;
		GLfloat green;
		GLfloat blue;
	};
	colorStruct color;
	velocityStruct velocity;
	positionStruct position;
	orientationStruct orientation;

	GLfloat startingX;
	GLfloat startingZ;

	bool isPowered;
	bool isAlive;

	float thrustPower;
	void propulsion(float timeSpeed);
	void propulsionBackwards(float timeSpeed);
	void propulsionLeft(float timeSpeed);
	void propulsionRight(float timeSpeed);
	void idleDrifting(float timeSpeed);
	void gravityEffect(float gravityForceX, float gravityForceZ);
	void reset();
	
};