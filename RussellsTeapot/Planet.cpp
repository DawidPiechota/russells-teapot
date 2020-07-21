#include "Planet.h"

Planet::Planet(float red, float green, float blue, GLfloat size, GLfloat rotationSpeed, GLfloat orbitRadius, GLfloat startingTheta)
{
	this->color.red = red;
	this->color.green = green;
	this->color.blue = blue;
	this->size = size;
	this->rotationSpeed = rotationSpeed;
	this->orbitRadius = orbitRadius;
	posX = 0;
	posZ = 0;
	theta = startingTheta;
	mass = size * size * size;
	donutOn = 1;
}

Planet::~Planet()
{
}

void Planet::positionCalc()
{
	posX = orbitRadius * sin(theta);
	posZ = orbitRadius * cos(theta);
}

void Planet::donutPositionCalc()
{
	donutPosX = (orbitRadius + size + 3) * sin(theta);
	donutPosZ = (orbitRadius + size + 3) * cos(theta);
}

float Planet::getGravityForceX(float shipX, float shipZ, bool forGrid)
{
	if((forGrid == false && distance2(posX, posZ, shipX, shipZ) > size*size) || forGrid == true)  // if forGrid == 1 return gravity, if forGrid == 0 and ship is inside planet return 0
	return mass / (distance2(posX, posZ, shipX, shipZ)) * (shipX - posX) / (fabs(shipX - posX)*fabs(shipX - posX) + fabs(shipZ - posZ)*fabs(shipZ - posZ));	//pozycja statku relatywna do planety to (xs-xp,ys-yp)
	else return 0;
}

float Planet::getGravityForceZ(float shipX, float shipZ, bool forGrid)
{
	if ((forGrid == false && distance2(posX, posZ, shipX, shipZ) > size*size) || forGrid == true)
	return mass / (distance2(posX, posZ, shipX, shipZ)) * (shipZ - posZ) / (fabs(shipX - posX)*fabs(shipX - posX) + fabs(shipZ - posZ)*fabs(shipZ - posZ));
	else return 0;
}

float Planet::distance2(float x1, float y1, float x2, float y2)
{
	return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

float Planet::getGravityForceXTest(float shipX, float shipZ)
{
	if (distance2(posX, posZ, shipX, shipZ) > size*size)
		return mass / (distance2(posX, posZ, shipX, shipZ)) * (shipX - posX) / (fabs(shipX - posX) + fabs(shipZ - posZ));	//pozycja statku relatywna do planety to (xs-xp,ys-yp)
	else return 0;
}
