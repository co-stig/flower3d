#include "trunk.h"
#include <math.h>

trunk::trunk(void)
{
//	double xx = 15 * (((rand() / double(RAND_MAX)) - 0.5f));
//	double yy = 15 * (((rand() / double(RAND_MAX)) - 0.5f));
//	double zz = 1 * (((rand() / double(RAND_MAX))));
//	double th = 50 * (rand() / double(RAND_MAX));
	double xx = 0 * (((rand() / double(RAND_MAX)) - 0.5f));
	double yy = 0 * (((rand() / double(RAND_MAX)) - 0.5f));
	double zz = 10 * (((rand() / double(RAND_MAX))));
	double th = 20 * (rand() / double(RAND_MAX));

	root.set_coord(vector3d(xx, yy, 0));
	root.set_thickness(th);
	root.add_child(vector3d(xx, yy, zz));
}

trunk::~trunk(void)
{
}


void trunk::display_gl(void)
{
	display_tree(&root);
}

void trunk::generate_trunk (void)
{
	gen_tree (root.get_child(0), 100);
}

void trunk::gen_tree (trunk_segment* p, int depth)
{
	if (depth < 1)
	{
		return;
	}

	int branches = 3; // number of possible branches
	double xang = 1.7; // possible angles to rotate
	double yang = 5.7;
	double zang = 1.7;
	int solutions = 3; // number of random solutions to select
	                   // the best variant from
	double minthick = 0.1; // minimal branch thickness
	double smallest_thickness = 1.0f; // smallest branch thickness to exist
	double tlf = 2.0f; // thickness / length factor
	double grad = 5; // gradations coefficient (per 1 length)

	double thickness = p->get_thickness();

	int asolutions = int(double(solutions) * (rand() / double(RAND_MAX)));

	if ((branches == 0) || (thickness < smallest_thickness))
	{
		return;
	}

	// Calculate all thickness...
	double* ta = new (double[branches + 10]);
	double overall = 1.0f - minthick;

	for (int n = 0; n < (branches - 1); n++)
	{
		double athickness = (rand() / double(RAND_MAX)) * overall;
		ta[n] = athickness;
		overall -= athickness;
	}
	ta[branches - 1] = overall;

	for (int n = 0; n < branches; n++)
	{
		// 1. Generate random branch parameters

		double axang = (1.0f - ta[n]) * 2.0f * xang * ((rand() / double(RAND_MAX)) - 0.5f);
		double ayang = (1.0f - ta[n]) * 2.0f * yang * ((rand() / double(RAND_MAX)) - 0.5f);
		double azang = (1.0f - ta[n]) * 2.0f * zang * ((rand() / double(RAND_MAX)) - 0.5f);

		// 2. Find previous sub-trunk vector and continue it to alength:
		
		double newlength = sqrt(ta[n] * thickness) * tlf;
		int agrad = int(newlength * grad) + 1;

		// Now, rotate it gradually
		
		double ixang = axang / double(agrad);
		double iyang = ayang / double(agrad);
		double izang = azang / double(agrad);
		axang = ayang = azang = 0.0f;
		vector3d cc = p->get_coord(); // current coords

		trunk_segment* ts = p;
	
		for (int g = 0; g < agrad; g++)
		{
			axang += ixang; ayang += iyang; azang += izang;

			vector3d pv = ((p->get_coord() - p->get_parent()->get_coord())
				.get_normalized() 
				* newlength * (1.0f / double(agrad))) 
				/*+ p->get_coord()*/;

			// 3. Rotate it over Ox, Oy, Oz

			double x1, y1, z1, cosa, sina;

			cosa = cos (axang); sina = sin (axang);
			y1 = pv.y; z1 = pv.z;
			pv.y = y1 * cosa - z1 * sina;
			pv.z = y1 * sina + z1 * cosa;

			cosa = cos (ayang); sina = sin (ayang);
			x1 = pv.x; z1 = pv.z;
			pv.x = x1 * cosa - z1 * sina;
			pv.z = x1 * sina + z1 * cosa;

			cosa = cos (azang); sina = sin (azang);
			x1 = pv.x; y1 = pv.y;
			pv.x = x1 * cosa - y1 * sina;
			pv.y = x1 * sina + y1 * cosa;

			cc = cc + pv;

			ts = ts->add_child(cc);
			ts->set_thickness(ta[n] * thickness);
		}

		// 5. Recursively call itself for each sub-trunk

		ts->set_thickness(ta[n] * thickness);

		gen_tree (ts, (depth - 1));
	}

	delete[] ta;
}

void trunk::display_tree (trunk_segment* p)
{
	if (!p)
	{
		return;
	}

	for (int n = 0; n < p->count(); n++)
	{
		vector3d v1 = p->get_coord();
		vector3d v2 = p->get_child(n)->get_coord();

		//num++;

		glLineWidth(p->get_child(n)->get_thickness() / 3);
		//glLineWidth(1);
		glBegin(GL_LINES);
			glVertex3f (v1.x, v1.y, v1.z);
			glVertex3f (v2.x, v2.y, v2.z);
		glEnd();

		display_tree(p->get_child(n));
	}
}

trunk_segment* trunk::get_root (void)
{
	return &root;
}

int trunk::free_count(void)
{
	int i = 0;
	rec_count_free(&root, &i);
	return i;
}

void trunk::rec_count_free (trunk_segment* ts, int* k)
{
	if (ts->count() == 0)
	{
		(*k)++;
	} else {
		for (int i = 0; i < ts->count(); i++)
		{
			rec_count_free (ts->get_child(i), k);
		}
	}
}