#pragma comment(lib, "irrKlang.lib")
#include "Planet.h"
#include "Spaceship.h"
#include "main.h"
#include "Image.h"
#include <iostream>
#include <deque>
#include <ctime>
#include <windows.h>
#include "include/irrKlang.h"

using namespace std;
using namespace irrklang;

#define PI  3.14159265358979323846

const int N = 6;

struct point2D
{
	GLfloat posX;
	GLfloat posZ;
};

deque<point2D> lines;
deque<deque<point2D>> planetLines;
int linesCapacity = 1000;
int planetLinesCapacity = 2000;
Spaceship spaceship(120.f, 0.0f, 10.0f, 0.5f);
int gameActive = 0;


typedef float point3[3];
int planetChoice = 0;

float galaxySize = 2;
int visualMode = 0; //wylaczenie kolizji
float gravityStrenghtSun = 5;
float gravityStrenghtPlanets = 10;
float timeSpeed = 2;
const int gravityHalfGrid = 300;
int gravityGridInc = 3;
int gridOn = 1;

bool bouncing = 0;
bool inBorderX = 1;
bool inBorderZ = 1;
float gravityX[2 * gravityHalfGrid][2 * gravityHalfGrid];
float gravityZ[2 * gravityHalfGrid][2 * gravityHalfGrid];
const float stala = 0.017453;		 // PI/180
float gForceX, gForceZ;
float sunMass = 1700/1;
float sunRadius = 10;
float cameraHeight = 50;
float cameraX = 0;
float cameraZ = 0;
bool done = 0;
int linesTime = 0;
int planetLinesFrequency = 100;
int linesFrequency = 1;
int lightSwitch = 0;
int donutBaseLightning = 16386; // = 0x4002 = GL_LIGHT2
//int halfGridSize = 800;
int flyingThroughDonut = 0;
int points = 0;
int currentDonut;
ISoundEngine* engine = createIrrKlangDevice();
ISound* music;
ISound* dead;
ISound* point;
float teapotSize = 1;
int lastDonut = 0;
int earthPosition = 2;
int deadPlaying = 0;

Planet planets[N]{
	{ 1.0,0.0,0.0,  3.0f, 0.39f, 20.0f* galaxySize, -1 },
{ 0.0,1.0,0.0,  1.8f, 0.31f, 35.0f* galaxySize, 2 },
{ 0.0,0.0,1.0,  3.5f, 0.2f, 100.0f* galaxySize, 3 },
{ 1.0,1.0,0.0,  5.0f, 0.17f, 50.0f* galaxySize, 4.1f },
{ 0.0,1.0,1.0,  7.0f, 0.1f, 70.0f* galaxySize, 5 },
{ 1.0,0.0,1.0,  9.0f, 0.08f, 125.0f* galaxySize, 6 },
// R,G,B,     size, rSpeed, radius, theta
};


GLUquadricObj *sphere = NULL;

Image a;
Image* img[N];
Image* sunImage;
Image* universeImage;
GLuint _textureId[N];
GLuint _textureIdSun;
GLuint _textureIdUniverse;

static GLfloat viewer[] = { 5.0, 5.0, 5.0 };

float viewerMultiplier = 20;
float upCorrection = 1;

int check = 0;

int checkColor;
float color1 = 0.0;
float color2 = 1.0;

//static GLfloat thetaMoon[N] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

static GLfloat theta = 0.0;   // k¹t obrotu obiektu
static GLfloat phi = 0.0;   // k¹t obrotu obiektu
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 2;       // stan klawiszy myszy 
							   // 0 - nie naciœniêto ¿adnego klawisza
							   // 1 - naciœniêty zostaæ lewy klawisz


void MyInit(void)
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat mat_ambient[] = { 139 / 225,69 / 225,19 / 225 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_specular[] = { 0.5, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess = { 3.0 };

	/*************************************************************************************/
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0};
	GLfloat light_position1[] = { spaceship.position.X, 1 , spaceship.position.Z, 1.0 };
	GLfloat donutLightPosition[] = { 0,0,0,1.0 };
	// po³o¿enie Ÿród³a 

	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat light_ambient_zero[] = { 0.0, 0.0, 0.0 };

	GLfloat light_diffuse[] = { 0.9, 0.5, 0.5, 1.0 };
	GLfloat light_diffuse1[] = { spaceship.color.red, spaceship.color.green, spaceship.color.blue, 1.0 };

	GLfloat light_specular[] = { 0.9, 0.5, 0.5, 1.0 };
	GLfloat light_specular1[] = { spaceship.color.red, spaceship.color.green, spaceship.color.blue, 1.0 };
	GLfloat donutLightSpecular[] = { 0.0, 1.0, 1.0, 1.0 };

	GLfloat att_constant = { 1.0 };
	GLfloat att_constant1 = { 0.0 };

	GLfloat att_linear = { 0.005f };
	GLfloat att_linear1 = { 0.05f };
	GLfloat donutAttLinear = { 0.0f };

	GLfloat att_quadratic = { 0.00f };
	GLfloat att_quadratic1 = { 0.2f };
	GLfloat donutAttQuadratic = { 0.2f };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	/*************************************************************************************/

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	/*************************************************************************************/

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient_zero);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant1);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear1);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic1);

	for (int i = 0; i < N; i++)
	{
		if (planets[i].donutOn)
		{
			if (!lastDonut && i == earthPosition) continue;
			donutLightPosition[0] = planets[i].donutPosX;
			donutLightPosition[1] = 0.0f;
			donutLightPosition[2] = planets[i].donutPosZ;
			donutLightPosition[3] = 1.0f;
			glLightfv(i + donutBaseLightning, GL_AMBIENT, light_ambient_zero);
			glLightfv(i + donutBaseLightning, GL_DIFFUSE, light_diffuse);
			glLightfv(i + donutBaseLightning, GL_SPECULAR, donutLightSpecular);
			glLightfv(i + donutBaseLightning, GL_POSITION, donutLightPosition);

			glLightf(i + donutBaseLightning, GL_CONSTANT_ATTENUATION, att_constant1);
			glLightf(i + donutBaseLightning, GL_LINEAR_ATTENUATION, donutAttLinear);
			glLightf(i + donutBaseLightning, GL_QUADRATIC_ATTENUATION, donutAttQuadratic);
			glEnable(i + donutBaseLightning);
		}
		else glDisable(i + donutBaseLightning);
		
	}

	// Ustawienie opcji systemu oœwietlania sceny 

	glShadeModel(GL_SMOOTH); // w³aczenie ³agodnego cieniowania
	glEnable(GL_LIGHTING);   // w³aczenie systemu oœwietlenia sceny 
	glEnable(GL_LIGHT0);     // w³¹czenie Ÿród³a o numerze 0
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST); // w³¹czenie mechanizmu z-bufora

							 
/*************************************************************************************/


}
void inline DrawGrid(int HALF_GRID_SIZE)		//Nie dziala jako funkcja (?????)
{
	
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_LINES);
	glColor3f(0.3f, 0.3f, 0.3f);
	for (int i = -HALF_GRID_SIZE; i <= HALF_GRID_SIZE; i+=20)
	{
		glVertex3f((float)i, 0, (float)-HALF_GRID_SIZE);
		glVertex3f((float)i, 0, (float)HALF_GRID_SIZE);

		glVertex3f((float)-HALF_GRID_SIZE, 0, (float)i);
		glVertex3f((float)HALF_GRID_SIZE, 0, (float)i);
	}
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
	
	
}

void Axes(void)
{

	point3  x_min = { -8.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x

	point3  y_min = { 0.0, -8.0, 0.0 };
	point3  y_max = { 0.0,  8.0, 0.0 };
	// pocz?tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -8.0 };
	point3  z_max = { 0.0, 0.0,  8.0 };
	//  pocz?tek i koniec obrazu osi y
	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glLineWidth(1.0);
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y

	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z

	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();

}

/*************************************************************************************/



// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana, gdy trzeba 
// przerysowaæ scenê)

float distance2(float x1, float y1, float x2, float y2)
{
	return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

float distance(float x1, float y1, float x2, float y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void pointCheck()
{
	if (points == N - 1)
	{
		lastDonut = 1;
	}
	if (points == N)
	{
		cout << "naaajs" << endl;
	}
}

void donutCollision()
{
	if (flyingThroughDonut == 0)
	{
		for (int i = 0; i < N; i++)
		{
			if (!lastDonut && i == earthPosition) continue;
			if ((planets[i].donutOn) && (distance2(spaceship.position.X, spaceship.position.Z, planets[i].donutPosX, planets[i].donutPosZ) < 0.5f))
			{
				flyingThroughDonut = 1;
				currentDonut = i;
				if (lastDonut) {
					point = engine->play2D("audio/congratulations.mp3", true, true);
					music->setIsPaused(true);
					point->setIsPaused(false);
					point->setVolume(1.0); }
				else point = engine->play2D("audio/point.wav", false, true); point->setIsPaused(false); point->setVolume(0.7);
				cout << "najs" << endl;
				points++;
				teapotSize+=0.7;
				pointCheck();
				break;
			}
		}
	}
	else
	{
		if (distance2(spaceship.position.X, spaceship.position.Z, planets[currentDonut].donutPosX, planets[currentDonut].donutPosZ) > 0.7f)
		{
			flyingThroughDonut = 0;
			planets[currentDonut].donutOn = 0;
		}
	}
	
}

void colision()
{
	for (int i = 0; i < N; i++)
	{
		if (distance2(spaceship.position.X, spaceship.position.Z, planets[i].posX, planets[i].posZ) <= planets[i].size*planets[i].size)	// pierwiastek jest wolny. Odleglosc w ^2
		{
			spaceship.isAlive = 0;
		}
	}
	if( distance2(spaceship.position.X, spaceship.position.Z, 0.0, 0.0) < sunRadius * sunRadius) spaceship.isAlive = 0;
}

GLfloat getAlpha(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	return  atan((y2 - y1) / (x2 - x1));
}

GLfloat getBeta(GLfloat x, GLfloat y)
{
	return atan((y / x)) ;
}

void bounce()
{
	if ((distance2(spaceship.position.X, spaceship.position.Z, 0.0, 0.0) < sunRadius * sunRadius) && bouncing == false)
	{
		bouncing = true;

		GLfloat l = sqrt(spaceship.velocity.X*spaceship.velocity.X + spaceship.velocity.Z * spaceship.velocity.Z);
		GLfloat alpha = getAlpha(0, 0, spaceship.position.X, spaceship.position.Z) * 180 / PI;
		GLfloat phii = getBeta(spaceship.velocity.X, spaceship.velocity.Z) * 180 / PI - alpha;

		GLfloat lpar = l * cos(phii * PI / 180);
		GLfloat lper = l * sin(phii * PI / 180);

		spaceship.velocity.X = lpar *cos(alpha* PI / 180) + lper * cos((90 * PI / 180 - alpha * PI / 180) );
		spaceship.velocity.Z = lpar *sin(alpha* PI / 180) + lper * sin((90 * PI / 180 - alpha * PI / 180));

	}
	else if (distance2(spaceship.position.X, spaceship.position.Z, 0.0, 0.0) > sunRadius * sunRadius + 30) bouncing = false;
}

void planetsPositionCalc()
{
		for (int i = 0; i < N; i++)
		{
			planets[i].positionCalc();
			planets[i].donutPositionCalc();
			planets[i].theta += planets[i].rotationSpeed * timeSpeed/2 * stala * 0.05 / galaxySize;
		}
}

void won()
{
	spaceship.color.red = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	spaceship.color.green = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	spaceship.color.blue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void calculateGravityEffect()
{
	for (int i = 0; i < N; i++)
	{
		gForceX += gravityStrenghtPlanets * planets[i].getGravityForceX(spaceship.position.X, spaceship.position.Z, false);
		gForceZ += gravityStrenghtPlanets * planets[i].getGravityForceZ(spaceship.position.X, spaceship.position.Z, false);
	}
	// Efekt G slonca:
	if (distance2(0, 0, spaceship.position.X, spaceship.position.Z) > sunRadius*sunRadius) {
		gForceX += gravityStrenghtSun * sunMass / (distance2(0, 0, spaceship.position.X, spaceship.position.Z)) * spaceship.position.X / (fabs(spaceship.position.X) + fabs(spaceship.position.Z));
		gForceZ += gravityStrenghtSun * sunMass / (distance2(0, 0, spaceship.position.X, spaceship.position.Z)) * spaceship.position.Z / (fabs(spaceship.position.X) + fabs(spaceship.position.Z));
	}
	spaceship.gravityEffect(timeSpeed * gForceX, timeSpeed * gForceZ);
	spaceship.idleDrifting(timeSpeed);
}
float calculateGravityEffectX(float x, float z, bool forGrid)
{
	gForceX = 0;
	for (int i = 0; i < N; i++)
	{
		gForceX += gravityStrenghtPlanets * planets[i].getGravityForceX(x, z, forGrid);
	}
	// Efekt G slonca:
	// TO SAMO Z PLANETAMI ZROBIC!!!
	// 21.07.2020: Nie mam pojêcia co mia³em na myœli
	gForceX += 50 * gravityStrenghtSun * sunMass / (distance2(0, 0, x, z)) * x / (fabs(x)*fabs(x) + fabs(z)*fabs(z));

	return gForceX;
}
float calculateGravityEffectZ(float x, float z, bool forGrid)
{
	gForceZ = 0;
	for (int i = 0; i < N; i++)
	{
		gForceZ += gravityStrenghtPlanets * planets[i].getGravityForceZ(x, z, forGrid);
	}
	// Efekt G slonca:
	gForceZ += 50 * gravityStrenghtSun * sunMass / (distance2(0, 0, x, z)) * z / (fabs(x)*fabs(x)+ fabs(z)*fabs(z));

	return gForceZ;
}

void calculateGravityGrid()
{
	for(int i = -gravityHalfGrid; i < gravityHalfGrid; i+= gravityGridInc)
		for (int j = -gravityHalfGrid; j < gravityHalfGrid; j+= gravityGridInc)
		{

			gravityX[i+ gravityHalfGrid][j+ gravityHalfGrid] = calculateGravityEffectX(i, j, true);
			gravityZ[i+ gravityHalfGrid][j+ gravityHalfGrid] = calculateGravityEffectZ(i, j, true);
		}
}

float returnValueOrMax(float value)
{
	if (value >= 1000) return 31.6228;
	else return sqrt(value);
}


void displayGravity()
{
	calculateGravityGrid();
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor3f(0.1f, 0.1f, 0.1f);
	for (int i = -gravityHalfGrid; i < gravityHalfGrid-1; i+= gravityGridInc)
	{
		for (int j = -gravityHalfGrid; j < gravityHalfGrid-1; j+= gravityGridInc)
		{
			if (!(i == 0 && j == 0))
			{
				glBegin(GL_LINES);
				
				glVertex3f(i, -returnValueOrMax(distance(0, 0, gravityX[i + gravityHalfGrid][j + gravityHalfGrid], gravityZ[i + gravityHalfGrid][j + gravityHalfGrid])), j);
				glVertex3f(i, -returnValueOrMax(distance(0, 0, gravityX[i + gravityHalfGrid][j + gravityGridInc + gravityHalfGrid], gravityZ[i + gravityHalfGrid][j + gravityGridInc + gravityHalfGrid])), j + gravityGridInc);

				glVertex3f(i, -returnValueOrMax(distance(0, 0, gravityX[i + gravityHalfGrid][j + gravityHalfGrid], gravityZ[i + gravityHalfGrid][j + gravityHalfGrid])), j);
				glVertex3f(i + gravityGridInc, -returnValueOrMax(distance(0, 0, gravityX[i + gravityGridInc + gravityHalfGrid][j + gravityHalfGrid], gravityZ[i + gravityGridInc + gravityHalfGrid][j + gravityHalfGrid])), j);

				glEnd();
			}
		}
	}
	glEnable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	
}

// if speed > velocityCap divide by velocityDivider, else divide by 2
void checkForBorders(float border, float velocityCap, float velocityDivider)
{
	if (fabs(spaceship.position.X) > border && inBorderX == 1)
	{
		spaceship.velocity.X = -spaceship.velocity.X;
		if (fabs(spaceship.velocity.X) > velocityCap || fabs(spaceship.velocity.Z) > velocityCap)
		{
			spaceship.velocity.Z /= velocityDivider;
			spaceship.velocity.X /= velocityDivider;
		}
		else
		{
			spaceship.velocity.Z /= 2;
			spaceship.velocity.X /= 2;
		}
	inBorderX = 0;
	}
	else if (fabs(spaceship.position.Z) < border - 5) inBorderX = 1;
	if (fabs(spaceship.position.Z) > border && inBorderZ == 1)
	{
		spaceship.velocity.Z = -spaceship.velocity.Z;
		if (fabs(spaceship.velocity.X) > velocityCap || fabs(spaceship.velocity.Z) > velocityCap)
		{
			spaceship.velocity.Z /= velocityDivider;
			spaceship.velocity.X /= velocityDivider;
		}
		else
		{
			spaceship.velocity.Z /= 2;
			spaceship.velocity.X /= 2;
		}
	inBorderZ = 0;
	}
	else if (fabs(spaceship.position.Z) < border-5) inBorderZ = 1;
}

void idleFunc()
{
	if (GetAsyncKeyState(VK_RIGHT)) spaceship.orientation.Y -= 0.05;
	if (GetAsyncKeyState(VK_LEFT)) spaceship.orientation.Y += 0.05;
	if (GetAsyncKeyState(VK_UP)) { if (cameraHeight > 1) if (status == 1) cameraHeight -= cameraHeight/200; else cameraHeight -= cameraHeight / 400; };
	if (GetAsyncKeyState(VK_DOWN)) { if (status == 1) cameraHeight += cameraHeight / 200; else cameraHeight += cameraHeight / 400; };

	if (!spaceship.isAlive)	// Jezeli wykryto kolizje
	{
		if (!deadPlaying) { music->setIsPaused(true); dead = engine->play2D("audio/dead.mp3", false, true); dead->setIsPaused(false); dead->setVolume(0.7); deadPlaying = 1; }
		status = 2;
		cameraHeight = 40;
		if (dead->isFinished())
		{
			music->setIsPaused(false);
			music->setPlayPosition(0);
			deadPlaying = 0;
			lines.clear();
			spaceship.reset();
			gameActive = 0;
		}
	}

	if (points == N) won();

	if (gameActive && spaceship.isAlive) {
		donutCollision();		// kolizja z donutem

		planetsPositionCalc();
		if (GetAsyncKeyState('A')) spaceship.propulsionLeft(timeSpeed);
		if (GetAsyncKeyState('D')) spaceship.propulsionRight(timeSpeed);
		if (GetAsyncKeyState('W'))
		{
			spaceship.propulsion(timeSpeed);
			spaceship.isPowered = 1;
		}
		else spaceship.isPowered = 0;

		if (GetAsyncKeyState('S')) spaceship.propulsionBackwards(timeSpeed);

		gForceX = gForceZ = 0.0;
		// Efekt G planet:
		calculateGravityEffect();
		// odbicie na brzegach
		checkForBorders(2000, 0.5, 20);

		if (!visualMode)
			colision();
		//else bounce();

		if (linesTime % linesFrequency == 0)
		{
			lines.push_back(point2D() = { spaceship.position.X, spaceship.position.Z });		//dodanie do listy
			if (lines.size() >= linesCapacity)
			{
				lines.pop_front();
			}
		}

		if (linesTime % planetLinesFrequency == 0)
		{

			for (int i = 0; i < planetLines.size(); i++)
			{
				planetLines[i].push_back(point2D() = { planets[i].posX, planets[i].posZ });		//dodanie do listy
				if (planetLines[i].size() >= planetLinesCapacity)
				{
					planetLines[i].pop_front();
				}
			}
		}
		linesTime++;

		
	}
	glutPostRedisplay();
}

void keyboard(unsigned char Key, int x, int y)
{
	switch (Key)
	{
	case '1': {status = 1; cameraHeight = 1; }break;
	case '2': {status = 2; cameraHeight = 30; } break;
	case '3': status = 3; break;
	case 'o': {spaceship.thrustPower = 5; spaceship.color.green = 0.0; spaceship.color.red = 0.0; spaceship.color.blue = 1.0; }break;
	case 'p': {spaceship.thrustPower = 0.5; spaceship.color.green = 0.0; spaceship.color.red = 1.0; spaceship.color.blue = 0.0; }break;
	case 'i': {spaceship.thrustPower = 8; spaceship.color.green = 1.0; spaceship.color.red = 0.0; spaceship.color.blue = 0.0; }break;
	case 'l': gridOn = (++gridOn) % 3; break;
	case 'f': gravityStrenghtSun++; break;
	case 'v': if(gravityStrenghtSun!= 0)gravityStrenghtSun--; break;
	case 'g': gravityStrenghtPlanets++; break;
	case 'b': if(gravityStrenghtPlanets!=0)gravityStrenghtPlanets--; break;
	case 'h': timeSpeed+=1; break;
	case 'n': timeSpeed-=1; break;
	case 'j': if (gravityGridInc != 1)gravityGridInc--; break;
	case 'm': if (gravityGridInc != 10)gravityGridInc++; break;
	case 'k': visualMode = !visualMode; break;
	case 32 : if (gameActive) gameActive = 0; else gameActive = 1; break;
	case 27: {
		music->setIsPaused(false); music->drop(); delete point; delete dead; delete engine; exit(1);
	}
		break;
	};
}


void RenderScene(void)
{

	glClearColor(0.1, 0.1, 0.1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym
	

	glLoadIdentity();
	// Czyszczenie macierzy biezacej

	if (status == 1) gluLookAt(spaceship.position.X + cameraHeight * 0.6 * sin(spaceship.orientation.Y * PI / 180 - PI / 2) / 2.5, 0.1 * cameraHeight, +spaceship.position.Z + cameraHeight * 0.6 * cos(spaceship.orientation.Y * PI / 180 - PI / 2) / 2.5, spaceship.position.X, 0.1, spaceship.position.Z, 0.0, 1, 0.0);
	//else if (status == 3) gluLookAt(-30.0, cameraHeight, -30.0, 0.0, 0.0, 0.0, 1, 0, 0.0);
	else if (status == 3) gluLookAt(cameraHeight * 0.6 * sin(spaceship.orientation.Y * PI / 180 - PI / 2) / 2.5, 0.1 * cameraHeight, cameraHeight * 0.6 * cos(spaceship.orientation.Y * PI / 180 - PI / 2) / 2.5, 0, 0.1, 0, 0.0, 1, 0.0);
	else if (status == 2) gluLookAt(spaceship.position.X +  0.001* sin(spaceship.orientation.Y * PI / 180 - PI / 2) / 2.5, cameraHeight, spaceship.position.Z + 0.001 * cos(spaceship.orientation.Y * PI / 180 - PI / 2) / 2.5, spaceship.position.X, 0.2, spaceship.position.Z, 0.0, 1, 0.0);
	
	Axes();

	glColor3f(0.0, 0.0, 0.0);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, _textureIdSun);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluSphere(sphere, sunRadius, 20, 20);
	glRotatef(+90, 1.0, 0.0, 0.0);

	glBindTexture(GL_TEXTURE_2D, _textureIdUniverse);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(45, 0.0, 1.0, 1.0);
	gluSphere(sphere, 1500 * galaxySize, 20, 20);		//Galaxy
	glRotatef(-45, 0.0, 1.0, 1.0);
	glRotatef(+90, 1.0, 0.0, 0.0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	for (int i = 0; i < N; i++)
	{


		glPushMatrix();
		glTranslatef(planets[i].posX, 0, planets[i].posZ);									// Update polozenia planety
		glRotatef(-90, 1.0, 0.0, 0.0);														// Ustawienie planety pionowo
		glBindTexture(GL_TEXTURE_2D, _textureId[i]);
		gluSphere(sphere, planets[i].size, 20, 20);
		glPopMatrix();

		if (planets[i].donutOn)																//Rysowanie obreczy
		{
			if (!lastDonut && i == earthPosition) continue;
			glBindTexture(GL_TEXTURE_2D, _textureId[4]);
			glPushMatrix();
			glTranslatef(planets[i].donutPosX, 0.0, planets[i].donutPosZ);
			glRotatef(planets[i].theta * 180 / PI + 90, 0.0, 1.0, 0.0);
			glutSolidTorus(0.07, 0.7, 10, 8);
			glPopMatrix();
		}
		


	}
	glPushMatrix();

	glTranslatef(spaceship.position.X, spaceship.position.Y, spaceship.position.Z);
	glRotatef(spaceship.orientation.Y, 0.0, 1.0, 0.0);

	glutSolidTeapot(0.005 * teapotSize);

	glPopMatrix();
	///////////////////////////////////////////////// Linie
	glDisable(GL_TEXTURE_2D);
	glLineWidth(1.0);
	glEnable(GL_COLOR_MATERIAL);
	
	glColor3f(0.8f, 0.8f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < lines.size(); i++)
	{
		glVertex3f(lines[i].posX, 0.0, lines[i].posZ);
	}
	glEnd();

	for (int i = 0; i < planetLines.size(); i++)
	{
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < planetLines[i].size(); j++)
		{
			glVertex3f(planetLines[i][j].posX, 0.0, planetLines[i][j].posZ);
		}
		glEnd();
	}
	glDisable(GL_COLOR_MATERIAL);
	/////////////////////////////////////////////////


	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0.3f, 0.3f, 0.3f);

	if (gridOn == 1) DrawGrid(800);
	else if (gridOn == 2) displayGravity();

	
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	MyInit();
	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania
	glutSwapBuffers();
}
/*************************************************************************************/

// Funkcja ustalaj¹ca stan renderowania




/*************************************************************************************/


// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹ 
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.




void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;
	// przeliczenie pikseli na stopnie

	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej 

	gluPerspective(60, (float)horizontal / vertical , 0.1, 1500 * galaxySize * 2);
	// Ustawienie parametrów dla rzutu perspektywicznego

	glViewport(0, 0, horizontal, vertical);

	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej 

}

/*************************************************************************************/

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli

void Initialization()
{
	for (int i = 0; i < N; i++)
	{
		planetLines.push_back(deque<point2D>());
	}
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);

	glGenTextures(1, &_textureIdSun);
	sunImage = a.loadBMP("planets/sun.bmp");
	glBindTexture(GL_TEXTURE_2D, _textureIdSun);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sunImage->width, sunImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, sunImage->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &_textureIdUniverse);
	universeImage = a.loadBMP("planets/starsHQ.bmp");
	glBindTexture(GL_TEXTURE_2D, _textureIdUniverse);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, universeImage->width, universeImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, universeImage->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glGenTextures(N, _textureId); //Make room for our texture
	glEnable(GL_TEXTURE_2D);

	img[0] = a.loadBMP("planets/lava.bmp");
	glBindTexture(GL_TEXTURE_2D, _textureId[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img[0]->width, img[0]->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[0]->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	img[1] = a.loadBMP("planets/mercury.bmp");
	glBindTexture(GL_TEXTURE_2D, _textureId[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img[1]->width, img[1]->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[1]->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	img[2] = a.loadBMP("planets/earth.bmp");
	glBindTexture(GL_TEXTURE_2D, _textureId[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img[2]->width, img[2]->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[2]->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	img[3] = a.loadBMP("planets/venus.bmp");
	glBindTexture(GL_TEXTURE_2D, _textureId[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img[3]->width, img[3]->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[3]->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	img[4] = a.loadBMP("planets/jupiter.bmp");
	glBindTexture(GL_TEXTURE_2D, _textureId[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img[4]->width, img[4]->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[4]->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	img[5] = a.loadBMP("planets/icy.bmp");
	glBindTexture(GL_TEXTURE_2D, _textureId[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img[5]->width, img[5]->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[5]->pixels);

	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	srand(static_cast <unsigned> (time(0)));
	music = engine->play2D("audio/soundtrack.mp3", true, true); music->setIsPaused(false); music->setVolume(0.7);

}

void main(void)
{
	///////////////////////-----INIT----///////////////////////
	
	///////////////////////////////////////////////////////////

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(500, 500);

	glutCreateWindow("Russel's teapot");

	glutFullScreen();

	Initialization();

	glutIdleFunc(idleFunc);

	glutKeyboardFunc(keyboard);

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
	// (callback function).  Bêdzie ona wywo³ywana za ka¿dym razem 
	// gdy zajdzie potrzeba przerysowania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
	// za zmiany rozmiaru okna                       



	MyInit();
	// Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst¹pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania niewidocznych elementów sceny

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/