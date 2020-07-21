#pragma once
#include <GL/glut.h>
class Image
{
public:
	Image(char* ps, int w, int h);
	Image() {};
	~Image();

	Image* loadBMP(const char* filename);
	GLuint loadTextureFromImage(Image* image);


	char* pixels;
	int width, height;
};