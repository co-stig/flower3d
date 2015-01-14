#include "stdafx.h"

#include <math.h>
#include "petal_gen1.h"

double const PI = 3.14159265358979;

petal_gen1::petal_gen1(void)
{
}

petal_gen1::~petal_gen1(void)
{
}

void petal_gen1::generate_petal (std::vector<petal_turn>* turns, vector3d* perpendicular)
{
	int nu = 20;					// accuracy (turns number)
	double ra = 3;					// long radius
	double rb = 1;					// short radius
	double startangle = PI / 3;		// starting angle

	double startx = ra * cos (startangle);
	double starty = rb * sin (startangle);

	double anginc = (PI - startangle) / double(nu);

	double ang = startangle;
	petal_turn p;

	p.v1.x = startx; p.v2.x = startx; (*perpendicular).x = 1000;
	p.v1.y = -starty; p.v2.y = starty; (*perpendicular).y = -starty;
	p.v1.z = 0; p.v2.z = 0; (*perpendicular).z = 0;

	turns->push_back(p);

	for (int a = 0; a < nu + 2; a++)
	{
		p.v2.x = ra * cos (ang); p.v1.x = p.v2.x;
		p.v2.y = rb * sin (ang); p.v1.y = -p.v2.y;
		p.v2.z = 0; p.v1.z = 0;
		ang += anginc;
		turns->push_back(p);
	}
}
