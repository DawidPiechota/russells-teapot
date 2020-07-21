#pragma once

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>

class Planet
{
public:
	Planet(float red, float green, float blue,GLfloat size,GLfloat rotationSpeed,GLfloat orbitRadius, GLfloat startingTheta);
	~Planet();

	struct planetColor	//nieuzywane
	{
		float red;
		float green;
		float blue;
	};

	planetColor color;
	GLfloat size;
	GLfloat rotationSpeed;
	GLfloat orbitRadius;
	GLfloat posX;
	GLfloat posZ;
	GLfloat donutPosX;
	GLfloat donutPosZ;
	GLfloat theta;
	GLfloat mass;

	bool donutOn;

	void positionCalc();
	void donutPositionCalc();
	float getGravityForceX(float shipX, float shipZ, bool forGrid);
	float getGravityForceXTest(float shipX, float shipZ);
	float getGravityForceZ(float shipX, float shipZ, bool forGrid);
	float distance2(float x1, float y1, float x2, float y2);



};