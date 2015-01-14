#include "flowerpot.h"

double const PI = 3.14159265358979;

void flowerpot::display_gl (texturizer* tex)
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	double f = 0.5f;
	double h = 8.0f;
	double vshift = h - 1.0f;

//	glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glColor4d(0.2f, 0.7f, 0.2f, 1);
	glColor4d(0.9f, 0.9f, 0.9f, 0.5f);
	for (unsigned n = 0; n < turns.size()-1; n++)
	{
		glBegin(GL_QUADS);
			vector3d v1 = turns[n];
			vector3d v2 = turns[n + 1];
			glVertex3d(v1.x, v1.y, v1.z + vshift);
			glVertex3d(v2.x, v2.y, v2.z + vshift);
			glVertex3d(v2.x * f, v2.y * f, v2.z * f - h + vshift);
			glVertex3d(v1.x * f, v1.y * f, v1.z * f - h + vshift);
		glEnd();
	}
	glBegin(GL_QUADS);
		vector3d v1 = turns[turns.size() - 1];
		vector3d v2 = turns[0];
		glVertex3d(v1.x, v1.y, v1.z + vshift);
		glVertex3d(v2.x, v2.y, v2.z + vshift);
		glVertex3d(v2.x * f, v2.y * f, v2.z * f - h + vshift);
		glVertex3d(v1.x * f, v1.y * f, v1.z * f - h + vshift);
	glEnd();
	
	//glColor4d(0.1f, 0.5f, 0.1f, 1);
	//glBegin(GL_POLYGON);
	//for (unsigned n = 0; n < turns.size(); n++)
	//{
	//	vector3d v = turns[n];
	//	glVertex3d(f * v.x, f * v.y, v.z - 2.0f);
	//}
	//glEnd();

	glDisable(GL_BLEND);		// Turn Blending On
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
}

void flowerpot::generate_flowerpot (double radius)
{
	int accuracy = 30;
//	double eps_factor = 0.5f;
	double eps_factor = 0.0f;

	turns.clear();

	double ang = 2.0f * PI / double(accuracy);
	for (int n = 0; n < accuracy; n++)
	{
		vector3d v;
		double ref1 = 1.0f + (eps_factor * (double(rand()) / RAND_MAX)) - 0.5f * eps_factor;
		double ref2 = 1.0f + (eps_factor * (double(rand()) / RAND_MAX)) - 0.5f * eps_factor;
		v.x = ref1 * radius * cos (ang * n);
		v.y = ref2 * radius * sin (ang * n);
		v.z = 0;
		turns.push_back(v);
	}
}

