#include "Spaceship.h"

#define PI  3.14159265358979323846 

Spaceship::Spaceship(GLfloat posX, GLfloat posY, GLfloat posZ, float thrustPower)
{
	this->startingX = this->position.X = posX;
	this->startingZ = this->position.Z = posY;
	this->position.Z = posZ;
	this->thrustPower = thrustPower;
	this->orientation.X = 0;
	this->orientation.Y = 90;
	this->orientation.Z = 0;
	this->velocity.X = 0;
	this->velocity.Y = 0;
	this->velocity.Z = 0.03;
	this->isPowered = 0;
	this->isAlive = 1;
	this->color.red = 1.0;
	this->color.blue = 0.0;
	this->color.red = 0.5;

}

void Spaceship::reset()
{
	this->position.X = this->startingX;
	this->position.Z = this->startingZ;
	this->orientation.X = 0;
	this->orientation.Y = 270;
	this->orientation.Z = 0;
	this->velocity.X = 0;
	this->velocity.Y = 0;
	this->velocity.Z = 0.022;
	this->isPowered = 0;
	this->isAlive = 1;
}

Spaceship::~Spaceship()
{
}

void Spaceship::propulsion(float timeSpeed)
{
	velocity.X += timeSpeed * thrustPower * 0.000005 * cos(orientation.Y * PI / 180);
	velocity.Z -= timeSpeed * thrustPower * 0.000005 * sin(orientation.Y * PI / 180);
}

void Spaceship::propulsionBackwards(float timeSpeed)
{
	velocity.X -= timeSpeed * thrustPower * 0.000005 * cos(orientation.Y * PI / 180);
	velocity.Z += timeSpeed * thrustPower * 0.000005 * sin(orientation.Y * PI / 180);
}

void Spaceship::propulsionRight(float timeSpeed)
{
	velocity.X -= timeSpeed * thrustPower *  0.000005 * cos(orientation.Y * PI / 180 + PI / 2);
	velocity.Z -= timeSpeed * thrustPower * 0.000005 * sin(orientation.Y * PI / 180 + 3 * PI / 2);

}

void Spaceship::propulsionLeft(float timeSpeed)
{
	velocity.X += timeSpeed * thrustPower * 0.000005 * cos(orientation.Y * PI / 180 + PI / 2);
	velocity.Z += timeSpeed * thrustPower * 0.000005 * sin(orientation.Y * PI / 180 +  3 * PI / 2);
}

void Spaceship::idleDrifting(float timeSpeed)
{
	position.X += timeSpeed * velocity.X;
	position.Z += timeSpeed * velocity.Z;
}

void Spaceship::gravityEffect(float gravityForceX, float gravityForceZ)
{
	velocity.X -= 0.00002 * gravityForceX;
	velocity.Z -= 0.00002 * gravityForceZ;
}

