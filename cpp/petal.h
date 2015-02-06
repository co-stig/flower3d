#pragma once

#include "stdafx.h"

#include <math.h>
#include <vector>
#include "matrix3x3.h"
#include "petal_turn.h"
#include "texturizer.h"

template <class petal_generator>
class petal
{

public:
	
	petal(void);
	~petal(void);

	matrix3x3 get_basis (void);
	void set_basis (matrix3x3 new_basis);
	void generate_petal (void);

	void connect (vector3d v1, vector3d v2, vector3d p);
	void turn (int index, double angle);

	void display (void);
	void display_gl (texturizer* tex);

	petal_generator* get_generator (void);

private:

	matrix3x3 basis;
	vector3d perpendicular;
	std::vector<petal_turn> turns;
	petal_generator generator;

};

// ********************************************************************
// ********************************************************************
// ********************************************************************

template <class petal_generator>
petal<petal_generator>::petal(void)
: perpendicular(0, 0, 0)
{
	basis.make_e();
	set_basis(basis);
}

template <class petal_generator>
petal<petal_generator>::~petal(void)
{
	//for (unsigned n = 0; n < turns.size(); n++)
	//{
	//	turns.pop_back();
	//}
}

template <class petal_generator>
matrix3x3 petal<petal_generator>::get_basis (void)
{
	return basis;
}

template <class petal_generator>
void petal<petal_generator>::set_basis (matrix3x3 new_basis)
{
	// move everything to new basis!
	new_basis.multiply_by_vector(&perpendicular);
	for (unsigned n = 0; n < turns.size(); n++)
	{
		new_basis.multiply_by_vector(&(turns[n].v1));
		new_basis.multiply_by_vector(&(turns[n].v2));
	}
	basis = new_basis;
}

template <class petal_generator>
void petal<petal_generator>::display (void)
{
}

template <class petal_generator>
void petal<petal_generator>::generate_petal (void)
{
	turns.clear();
	generator.generate_petal(&turns, &perpendicular);
}

template <class petal_generator>
void petal<petal_generator>::connect (vector3d v1, vector3d v2, vector3d p)
{
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// 1. Move everything to the beginning of v1:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	double dx = turns[0].v1.x;
	double dy = turns[0].v1.y;
	double dz = turns[0].v1.z;

	for (unsigned n = 0; n < turns.size(); n++)
	{
		turns[n].v1.x -= dx; turns[n].v2.x -= dx;
		turns[n].v1.y -= dy; turns[n].v2.y -= dy;
		turns[n].v1.z -= dz; turns[n].v2.z -= dz;
	}
	perpendicular.x -= dx;
	perpendicular.y -= dy;
	perpendicular.z -= dz;

	v2.x -= v1.x; v2.y -= v1.y; v2.z -= v1.z;
	p.x -= v1.x; p.y -= v1.y; p.z -= v1.z;
	dx = v1.x; dy = v1.y; dz = v1.z;
	v1.x = 0; v1.y = 0; v1.z = 0;

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!
	// 1.1 Find the length factor:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!

	double begilength = sqrt (
		(turns[0].v2.x - turns[0].v1.x) * (turns[0].v2.x - turns[0].v1.x) +
		(turns[0].v2.y - turns[0].v1.y) * (turns[0].v2.y - turns[0].v1.y) +
		(turns[0].v2.z - turns[0].v1.z) * (turns[0].v2.z - turns[0].v1.z));

	double factor = 0;

	if (begilength != 0)
	{
		factor = sqrt (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z) / begilength;
	} else {
		throw;
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// 1.1.1 Multiply all vectors by length factor:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if (factor != 1)
	{
		for (unsigned n = 0; n < turns.size(); n++)
		{
			turns[n].v1.x *= factor; turns[n].v2.x *= factor;
			turns[n].v1.y *= factor; turns[n].v2.y *= factor;
			turns[n].v1.z *= factor; turns[n].v2.z *= factor;
		}
		perpendicular.x *= factor;
		perpendicular.y *= factor;
		perpendicular.z *= factor;
	}

	// !!!!!!!!!!!!!!!!!!
	// 2. Make new basis:
	// !!!!!!!!!!!!!!!!!!
	// Oz will be (v2 - v1) = v2
	// Oy will be p
	// !!!!!!!!!!!!!!!!!!!!
	// 2.1 We will find Ox:
	// !!!!!!!!!!!!!!!!!!!!

	vector3d ox;
	ox.x = p.y * v2.z - v2.y * p.z;
	ox.y = p.z * v2.x - v2.z * p.x;
	ox.z = p.x * v2.y - v2.x * p.y;

	matrix3x3 new_basis;
	new_basis[0][0] = ox.x; new_basis[0][1] = p.x; new_basis[0][2] = v2.x;
	new_basis[1][0] = ox.y; new_basis[1][1] = p.y; new_basis[1][2] = v2.y;
	new_basis[2][0] = ox.z; new_basis[2][1] = p.z; new_basis[2][2] = v2.z;

	new_basis.normalize_cols();

	matrix3x3 tau = basis.tau_this_H(new_basis);

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// 3. Get all vectors in new basis:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	for (unsigned n = 0; n < turns.size(); n++)
	{
		tau.multiply_by_vector(&turns[n].v1);
		tau.multiply_by_vector(&turns[n].v2);
	}
	tau.multiply_by_vector(&perpendicular);

	// !!!!!!!!!!!!!!!!!!!!!!!!!!
	// 4. Find angles and rotate:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// 4.1 Over Oy to get turns[0] in Oyz:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	double newLen = sqrt(turns[0].v2.x * turns[0].v2.x + turns[0].v2.z * turns[0].v2.z);
	if (newLen != 0)
	{

		// find cos & sin to rotate over Oy:

		double sina = turns[0].v2.x / newLen;
		double cosa = turns[0].v2.z / newLen;
		double x1;
		double z1;

		// now, rotate everything over Oy:

		for (unsigned n = 0; n < turns.size(); n++)
		{
			x1 = turns[n].v1.x;
			z1 = turns[n].v1.z;
			turns[n].v1.x = x1 * cosa - z1 * sina;
			turns[n].v1.z = x1 * sina + z1 * cosa;
			x1 = turns[n].v2.x;
			z1 = turns[n].v2.z;
			turns[n].v2.x = x1 * cosa - z1 * sina;
			turns[n].v2.z = x1 * sina + z1 * cosa;
		}
		x1 = perpendicular.x;
		z1 = perpendicular.z;  
		perpendicular.x = x1 * cosa - z1 * sina;
		perpendicular.z = x1 * sina + z1 * cosa;
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// 4.2 Over Ox to get turns[0] as Oz:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	newLen = sqrt(turns[0].v2.y * turns[0].v2.y + turns[0].v2.z * turns[0].v2.z);

	if (newLen != 0)
	{

		// find cos & sin to rotate over Ox:

		double sina = turns[0].v2.y / newLen;
		double cosa = turns[0].v2.z / newLen;
		double y1;
		double z1;

		// now, rotate everything over Ox:

		for (unsigned n = 0; n < turns.size(); n++)
		{
			y1 = turns[n].v1.y;
			z1 = turns[n].v1.z;
			turns[n].v1.y = y1 * cosa - z1 * sina;
			turns[n].v1.z = y1 * sina + z1 * cosa;
			y1 = turns[n].v2.y;
			z1 = turns[n].v2.z;
			turns[n].v2.y = y1 * cosa - z1 * sina;
			turns[n].v2.z = y1 * sina + z1 * cosa;
		}
		y1 = perpendicular.y;
		z1 = perpendicular.z;  
		perpendicular.y = y1 * cosa - z1 * sina;
		perpendicular.z = y1 * sina + z1 * cosa;
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// 4.3 Over Oz to get perpendicular as Oy:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	newLen = sqrt(perpendicular.y * perpendicular.y + perpendicular.x * perpendicular.x);

	if (newLen != 0)
	{

		// find cos & sin to rotate over Oz:

		double sina = perpendicular.x / newLen;
		double cosa = perpendicular.y / newLen;
		double x1;
		double y1;

		// now, rotate everything over Oz:

		for (unsigned n = 0; n < turns.size(); n++)
		{
			x1 = turns[n].v1.x;
			y1 = turns[n].v1.y;
			turns[n].v1.x = x1 * cosa - y1 * sina;
			turns[n].v1.y = x1 * sina + y1 * cosa;
			x1 = turns[n].v2.x;
			y1 = turns[n].v2.y;
			turns[n].v2.x = x1 * cosa - y1 * sina;
			turns[n].v2.y = x1 * sina + y1 * cosa;
		}
		x1 = perpendicular.x;
		y1 = perpendicular.y;
		perpendicular.x = x1 * cosa - y1 * sina;
		perpendicular.y = x1 * sina + y1 * cosa;
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// 5. Get all vectors in old basis:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	tau = tau.get_reverse();

	for (unsigned n = 0; n < turns.size(); n++)
	{
		tau.multiply_by_vector(&turns[n].v1);
		tau.multiply_by_vector(&turns[n].v2);
	}

	tau.multiply_by_vector(&perpendicular);

	// !!!!!!!!!!!!!!!!!!!!!!!!
	// 6. Move everything back:
	// !!!!!!!!!!!!!!!!!!!!!!!!

	for (unsigned n = 0; n < turns.size(); n++)
	{
		turns[n].v1.x += dx; turns[n].v2.x += dx;
		turns[n].v1.y += dy; turns[n].v2.y += dy;
		turns[n].v1.z += dz; turns[n].v2.z += dz;
	}
	perpendicular.x += dx;
	perpendicular.y += dy;
	perpendicular.z += dz;

	// Connection done!
}

template <class petal_generator>
void petal<petal_generator>::display_gl (texturizer* tex)
{
	//glColor3d(1, 0.5, 0.5);

	glBindTexture(GL_TEXTURE_2D, tex->get_texture(0));

	double hf = double(1) / turns.size();

	for (unsigned n = 0; n < turns.size() - 1; n++)
	{
		glBegin(GL_POLYGON);

			glTexCoord2d(0.0f, hf * n);
			glVertex3d ( turns[n].v1.x, turns[n].v1.y, turns[n].v1.z );
			glTexCoord2d(0.0f, hf * (n + 1));
			glVertex3d ( turns[n + 1].v1.x, turns[n + 1].v1.y, turns[n + 1].v1.z );
			glTexCoord2d(1.0f, hf * (n + 1));
			glVertex3d ( turns[n + 1].v2.x, turns[n + 1].v2.y, turns[n + 1].v2.z );
			glTexCoord2d(1.0f, hf * n);
			glVertex3d ( turns[n].v2.x, turns[n].v2.y, turns[n].v2.z );

		glEnd();
	}
}

template <class petal_generator>
petal_generator* petal<petal_generator>::get_generator (void)
{
	return &generator;
}

template <class petal_generator>
void petal<petal_generator>::turn (int index, double angle)
{
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// 1. Move everything to the beginning of turns[index].v1:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	double dx = turns[index].v1.x;
	double dy = turns[index].v1.y;
	double dz = turns[index].v1.z;

	for (unsigned n = index; n < turns.size(); n++)
	{
		turns[n].v1.x -= dx; turns[n].v2.x -= dx;
		turns[n].v1.y -= dy; turns[n].v2.y -= dy;
		turns[n].v1.z -= dz; turns[n].v2.z -= dz;
	}

	// !!!!!!!!!!!!!!!!!!
	// 2. Make new basis:
	// !!!!!!!!!!!!!!!!!!
	// Oz will be turns[index].v2
	// !!!!!!!!!!!!!!!!!!!!!!!!!
	// 2.1 We will find Ox & Oy:
	// !!!!!!!!!!!!!!!!!!!!!!!!!

	// At first, find a non-collinear temp vector...
	vector3d tv = turns[index].v2;
	if (tv.x == 0) tv.x = 1;
	if (tv.y == 0) tv.y = 1;
	if (tv.z == 0) tv.z = 1;
	tv.x = -tv.x;

	vector3d ox;
	ox.x = turns[index].v2.y * tv.z - tv.y * turns[index].v2.z;
	ox.y = turns[index].v2.z * tv.x - tv.z * turns[index].v2.x;
	ox.z = turns[index].v2.x * tv.y - tv.x * turns[index].v2.y;

	vector3d oy;
	oy.x = turns[index].v2.y * ox.z - ox.y * turns[index].v2.z;
	oy.y = turns[index].v2.z * ox.x - ox.z * turns[index].v2.x;
	oy.z = turns[index].v2.x * ox.y - ox.x * turns[index].v2.y;

	matrix3x3 new_basis;
	new_basis[0][0] = ox.x; new_basis[0][1] = oy.x; new_basis[0][2] = turns[index].v2.x;
	new_basis[1][0] = ox.y; new_basis[1][1] = oy.y; new_basis[1][2] = turns[index].v2.y;
	new_basis[2][0] = ox.z; new_basis[2][1] = oy.z; new_basis[2][2] = turns[index].v2.z;

	new_basis.normalize_cols();

	matrix3x3 tau = basis.tau_this_H(new_basis);

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// 3. Get all vectors in new basis:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	for (unsigned n = index; n < turns.size(); n++)
	{
		tau.multiply_by_vector(&turns[n].v1);
		tau.multiply_by_vector(&turns[n].v2);
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!
	// 4 Rotate over Oz on angle:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!

	double sina = sin (angle);
	double cosa = cos (angle);
	double x1;
	double y1;

	for (unsigned n = index; n < turns.size(); n++)
	{
		x1 = turns[n].v1.x;
		y1 = turns[n].v1.y;
		turns[n].v1.x = x1 * cosa - y1 * sina;
		turns[n].v1.y = x1 * sina + y1 * cosa;
		x1 = turns[n].v2.x;
		y1 = turns[n].v2.y;
		turns[n].v2.x = x1 * cosa - y1 * sina;
		turns[n].v2.y = x1 * sina + y1 * cosa;
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// 5. Get all vectors in old basis:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	tau = tau.get_reverse();

	for (unsigned n = index; n < turns.size(); n++)
	{
		tau.multiply_by_vector(&turns[n].v1);
		tau.multiply_by_vector(&turns[n].v2);
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!
	// 6. Move everything back:
	// !!!!!!!!!!!!!!!!!!!!!!!!

	for (unsigned n = index; n < turns.size(); n++)
	{
		turns[n].v1.x += dx; turns[n].v2.x += dx;
		turns[n].v1.y += dy; turns[n].v2.y += dy;
		turns[n].v1.z += dz; turns[n].v2.z += dz;
	}
}

