#include "StdAfx.h"
#include "vector3d.h"
#include <math.h>
#include <gl\gl.h>

double& vector3d::operator[] (int n)
{
	if (n == 2) return z;
	if (n == 1) return y;
	return x;
}

vector3d vector3d::operator- (vector3d v)
{
	return (vector3d(x - v.x, y - v.y, z - v.z));
}

vector3d vector3d::operator+ (vector3d v)
{
	return (vector3d(x + v.x, y + v.y, z + v.z));
}

vector3d vector3d::operator* (double f)
{
	return (vector3d(x * f, y * f, z * f));
}

vector3d::vector3d (double xx, double yy, double zz)
: x(xx), y(yy), z(zz) 
{ 

}

void vector3d::display (void) 
{ 
	char t[100];
	sprintf (t, "%f\n%f\n%f", x, y, z);
	MessageBox (NULL, t, "vector3d", MB_OK);
}

void vector3d::display_gl (void)
{
	glBegin(GL_LINES);
		glVertex3d ( 0, 0, 0 );
		glVertex3d ( x, y, z );
	glEnd();
}

double vector3d::length(void)
{
	return sqrt (x * x + y * y + z * z);
}

vector3d vector3d::get_normalized (void)
{
	return ((*this) * (1.0f / length()));
}
