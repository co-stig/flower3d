#pragma once

#include "texturizer.h"
#include "petal.h"
#include "smooth_double.h"

const int k = 6;
const int m = 10;
const double alpha = 0.8f;

bool red_comp = true;
bool green_comp = false;
bool blue_comp = false;
bool alpha_comp = false;
bool lighting = false;
bool draw_flowers = true;
bool draw_pot = true;

template <class petal_generator, class center_generator>
class flower
{
	public:

		void display_gl(texturizer* tex);
		void generate_flower(vector3d v1 = vector3d(0, 0, 0), vector3d v2 = vector3d(0, 0, 1), double sz = 1.0f);
		void connect (vector3d v1, vector3d v2);

	private:

		petal <petal_generator> petals[k];
		center_generator center;
		vector3d mv1, mv2;

		smooth_double r;
		smooth_double g;
		smooth_double b;
		//smooth_double ra;
		//smooth_double rs;
};

template <class petal_generator, class center_generator>
void flower<petal_generator, center_generator>::display_gl(texturizer* tex)
{
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	
	glColor4f (1.0f, 1.0f, 1.0f, GLfloat(alpha));
	center.display_gl(tex);
	
	// Make and set colors...

	if (!r.in_progress())
	{
		double start = 0.5f;
		long tme = long(10000.0f * rand() / double(RAND_MAX));
		if (red_comp)
		{
			r.set_smooth((start + ((1.0f - start) * rand() / double(RAND_MAX))), tme);
		} else {
			r = 0;
		}
		if (green_comp)
		{
			g.set_smooth((start + ((1.0f - start) * rand() / double(RAND_MAX))), tme);
		} else {
			g = 0;
		}
		if (blue_comp)
		{
			b.set_smooth((start + ((1.0f - start) * rand() / double(RAND_MAX))), tme);
		} else {
			b = 0;
		}
//		ra.set_smooth( (2.0f * (0.5f - (rand() / double(RAND_MAX)))) / 100.0f * 5.0f , tme);
//		rs.set_smooth( 4 , tme);
	}

	glColor4d (r, g, b, alpha);
	for (int q = 0; q < k; q++)
	{
		petals[q].display_gl(tex);
	}

	// ******************
//	return;

	for (int q = 0; q < k; q++)
	{
		for (int z = 0; z < 20; z++)
		{
//			petals[q].turn (int(rs), ra);
		}
	}
	
}

template <class petal_generator, class center_generator>
void flower<petal_generator, center_generator>::generate_flower(vector3d v1, vector3d v2, double sz)
{
	double PI = 3.14159265358979;
	mv1 = v1; mv2 = v2;

	center.generate_center (k, sz * 1);

	for (int q = 0; q < k; q++)
	{
		petals[q].generate_petal();
		for (int z = 0; z < 20; z++)
		{
			petals[q].turn (z, PI / 40);
		}
	}

	connect(v1, v2);
}

template <class petal_generator, class center_generator>
void flower<petal_generator, center_generator>::connect (vector3d v1, vector3d v2)
{
	center.connect (v1, v2);

	petal_turn t;
	vector3d p;

	for (int q = 0; q < k; q++)
	{
		center.get_turn(q, &t, &p);
		petals[q].connect(t.v1, t.v2, p);
	}
}
