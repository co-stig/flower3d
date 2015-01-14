#include "stdafx.h"
#include "center_gen1.h"

void center_gen1::generate_center (int petals_number, double sc)
{
	double sz = 0.5;				// size coefficient
	sz = sc;
	double r = 1;				// main (big) radius
	double sr = 0.2;			// small radius
	double h = 0.05;				// small circle height
	double perp_h = r;			// petals tilt

	r *= sz; sr *= sz; h *= sz;
	double ang = 2 * PI / double(petals_number);
	double tcoeff = sr / r;
	petal_turn t;

	turns.clear();
	turns_small.clear();
	perps.clear();

	for (int q = 0; q < petals_number; q++)
	{
		double y1 = r * sin(ang * double(q));
		double x1 = r * cos(ang * double(q));
		double y2 = r * sin(ang * double(q + 1));
		double x2 = r * cos(ang * double(q + 1));
		t.v1 = vector3d (x1, y1, 0);
		t.v2 = vector3d (x2, y2, 0);
		turns.push_back(t);
		perps.push_back(vector3d (y1 - y2 + x1, x2 - x1 + y1, -perp_h));

		y1 = sr * sin(ang * double(q));
		x1 = sr * cos(ang * double(q));
		y2 = sr * sin(ang * double(q + 1));
		x2 = sr * cos(ang * double(q + 1));
		t.v1 = vector3d (x1, y1, h);
		t.v2 = vector3d (x2, y2, h);
		turns_small.push_back(t);

		vector3d tc ((1.0f + cos(ang * double(q))) / 2.0f, (1.0f + sin(ang * double(q))) / 2.0f, 0);
		big_texture_coords.push_back(tc);
		tc = vector3d((1.0f + tcoeff * cos(ang * double(q))) / 2.0f, (1.0f + tcoeff * sin(ang * double(q))) / 2.0f, 0);
		small_texture_coords.push_back(tc);
	}

	big_texture_coords.push_back(big_texture_coords[0]);
	small_texture_coords.push_back(small_texture_coords[0]);

	bottom.x = 0; bottom.y = 0; bottom.z = -2 * r;
	top.x = 0; top.y = 0; top.z = 2 * h;
	connection.x = 0; connection.y = 0; connection.z = -1.8 * r;

	basis.make_e();
}

void center_gen1::get_turn (int index, petal_turn* turn, vector3d* perpendicular)
{
	(*perpendicular) = perps[index];
	turn->v1 = turns[index].v1;
	turn->v2 = turns[index].v2;
}

void center_gen1::display_gl (texturizer* tex)
{
	for (unsigned n = 0; n < turns.size(); n++)
	{
		//glColor3f (1, 0.8, 0.4);

		glBindTexture(GL_TEXTURE_2D, tex->get_texture(1));

		glBegin(GL_POLYGON);

			glTexCoord2f(big_texture_coords[n].x, big_texture_coords[n].y);
			glVertex3d ( turns[n].v1.x, turns[n].v1.y, turns[n].v1.z );
			glTexCoord2f(big_texture_coords[n + 1].x, big_texture_coords[n + 1].y);
			glVertex3d ( turns[n].v2.x, turns[n].v2.y, turns[n].v2.z );
			glTexCoord2f(small_texture_coords[n + 1].x, small_texture_coords[n + 1].y);
			glVertex3d ( turns_small[n].v2.x, turns_small[n].v2.y, turns_small[n].v2.z );
			glTexCoord2f(small_texture_coords[n].x, small_texture_coords[n].y);
			glVertex3d ( turns_small[n].v1.x, turns_small[n].v1.y, turns_small[n].v1.z );

		glEnd();

		glBegin(GL_POLYGON);

			glTexCoord2f(small_texture_coords[n].x, small_texture_coords[n].y);
			glVertex3d ( turns_small[n].v1.x, turns_small[n].v1.y, turns_small[n].v1.z );
			glTexCoord2f(small_texture_coords[n + 1].x, small_texture_coords[n + 1].y);
			glVertex3d ( turns_small[n].v2.x, turns_small[n].v2.y, turns_small[n].v2.z );
			glTexCoord2f(0.5f, 0.5f);
			glVertex3d ( top.x, top.y, top.z );

		glEnd();

		//glColor3f (0.4, 0.8, 0.4);

		glBindTexture(GL_TEXTURE_2D, tex->get_texture(2));

		glBegin(GL_POLYGON);

			glTexCoord2f(big_texture_coords[n].x, big_texture_coords[n].y);
			glVertex3d ( turns[n].v1.x, turns[n].v1.y, turns[n].v1.z );
			glTexCoord2f(big_texture_coords[n + 1].x, big_texture_coords[n + 1].y);
			glVertex3d ( turns[n].v2.x, turns[n].v2.y, turns[n].v2.z );
			glTexCoord2f(0.5f, 0.5f);
			glVertex3d ( bottom.x, bottom.y, bottom.z );

		glEnd();
	}
}

matrix3x3 center_gen1::get_basis (void)
{
	return basis;
}

void center_gen1::set_basis (matrix3x3 new_basis)
{
	// move everything to new basis!
	new_basis.multiply_by_vector(&top);
	new_basis.multiply_by_vector(&bottom);
	new_basis.multiply_by_vector(&connection);
	for (unsigned n = 0; n < turns.size(); n++)
	{
		new_basis.multiply_by_vector(&(turns[n].v1));
		new_basis.multiply_by_vector(&(turns[n].v2));
		new_basis.multiply_by_vector(&(turns_small[n].v1));
		new_basis.multiply_by_vector(&(turns_small[n].v2));
		new_basis.multiply_by_vector(&(perps[n]));
	}
	basis = new_basis;
}

void center_gen1::connect (vector3d v1, vector3d v2)
{
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// 1. Move connection point to the v2:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	double dx = connection.x;
	double dy = connection.y;
	double dz = connection.z;

	for (unsigned n = 0; n < turns.size(); n++)
	{
		turns[n].v1.x -= dx; turns[n].v2.x -= dx;
		turns[n].v1.y -= dy; turns[n].v2.y -= dy;
		turns[n].v1.z -= dz; turns[n].v2.z -= dz;
		turns_small[n].v1.x -= dx; turns_small[n].v2.x -= dx;
		turns_small[n].v1.y -= dy; turns_small[n].v2.y -= dy;
		turns_small[n].v1.z -= dz; turns_small[n].v2.z -= dz;
		perps[n].x -= dx;
		perps[n].y -= dy;
		perps[n].z -= dz;
	}

	top.x -= dx; bottom.x -= dx; connection.x -= dx;
	top.y -= dy; bottom.y -= dy; connection.y -= dy;
	top.z -= dz; bottom.z -= dz; connection.z -= dz;

	dx = connection.x + v2.x;
	dy = connection.y + v2.y;
	dz = connection.z + v2.z;

	v1.x -= v2.x; v1.y -= v2.y; v1.z -= v2.z;
	v1.x = -v1.x; v1.y = -v1.y; v1.z = -v1.z;

	// Done...

	// !!!!!!!!!!!!!!!!!!
	// 2. Make new basis:
	// !!!!!!!!!!!!!!!!!!
	// Oz will be v2
	// !!!!!!!!!!!!!!!!!!!!!!!!!
	// 2.1 We will find Ox & Oy:
	// !!!!!!!!!!!!!!!!!!!!!!!!!

	vector3d ox;
	ox.x = top.y * v1.z - v1.y * top.z;
	ox.y = top.z * v1.x - v1.z * top.x;
	ox.z = top.x * v1.y - v1.x * top.y;

	vector3d oy;
	oy.x = v1.y * ox.z - ox.y * v1.z;
	oy.y = v1.z * ox.x - ox.z * v1.x;
	oy.z = v1.x * ox.y - ox.x * v1.y;

	if (!(
		((ox.x == ox.y) && (ox.y == ox.z) && (ox.z == 0)) ||
		((oy.x == oy.y) && (oy.y == oy.z) && (oy.z == 0))
		)) {

		matrix3x3 new_basis;
		new_basis[0][0] = ox.x; new_basis[0][1] = oy.x; new_basis[0][2] = v1.x;
		new_basis[1][0] = ox.y; new_basis[1][1] = oy.y; new_basis[1][2] = v1.y;
		new_basis[2][0] = ox.z; new_basis[2][1] = oy.z; new_basis[2][2] = v1.z;

		new_basis.normalize_cols();

		matrix3x3 tau = basis.tau_this_H(new_basis);

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// 3. Get all vectors in new basis:
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		for (unsigned n = 0; n < turns.size(); n++)
		{
			tau.multiply_by_vector(&turns[n].v1);
			tau.multiply_by_vector(&turns[n].v2);
			tau.multiply_by_vector(&turns_small[n].v1);
			tau.multiply_by_vector(&turns_small[n].v2);
			tau.multiply_by_vector(&perps[n]);
		}
		tau.multiply_by_vector(&top);
		tau.multiply_by_vector(&bottom);
		tau.multiply_by_vector(&connection);
		tau.multiply_by_vector(&v1);
		tau.multiply_by_vector(&v2);

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// 4. Find angles and rotate over Ox:
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		double newLen = sqrt(top.y * top.y + top.z * top.z);

		if (newLen != 0)
		{
			// find cos & sin to rotate over Ox:
			double sina = top.y / newLen;
			double cosa = top.z / newLen;
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

				y1 = perps[n].y;
				z1 = perps[n].z;
				perps[n].y = y1 * cosa - z1 * sina;
				perps[n].z = y1 * sina + z1 * cosa;

				y1 = turns_small[n].v1.y;
				z1 = turns_small[n].v1.z;
				turns_small[n].v1.y = y1 * cosa - z1 * sina;
				turns_small[n].v1.z = y1 * sina + z1 * cosa;
				y1 = turns_small[n].v2.y;
				z1 = turns_small[n].v2.z;
				turns_small[n].v2.y = y1 * cosa - z1 * sina;
				turns_small[n].v2.z = y1 * sina + z1 * cosa;
			}
			y1 = top.y;
			z1 = top.z;  
			top.y = y1 * cosa - z1 * sina;
			top.z = y1 * sina + z1 * cosa;
			y1 = bottom.y;
			z1 = bottom.z;  
			bottom.y = y1 * cosa - z1 * sina;
			bottom.z = y1 * sina + z1 * cosa;
			y1 = connection.y;
			z1 = connection.z;  
			connection.y = y1 * cosa - z1 * sina;
			connection.z = y1 * sina + z1 * cosa;
		}

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// 5. Get all vectors in old basis:
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		tau = tau.get_reverse();
		for (unsigned n = 0; n < turns.size(); n++)
		{
			tau.multiply_by_vector(&turns[n].v1);
			tau.multiply_by_vector(&turns[n].v2);
			tau.multiply_by_vector(&turns_small[n].v1);
			tau.multiply_by_vector(&turns_small[n].v2);
			tau.multiply_by_vector(&perps[n]);
		}
		tau.multiply_by_vector(&top);
		tau.multiply_by_vector(&bottom);
		tau.multiply_by_vector(&connection);
	
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!
	// 6. Move everything back:
	// !!!!!!!!!!!!!!!!!!!!!!!!

	for (unsigned n = 0; n < turns.size(); n++)
	{
		turns[n].v1.x += dx; turns[n].v2.x += dx;
		turns[n].v1.y += dy; turns[n].v2.y += dy;
		turns[n].v1.z += dz; turns[n].v2.z += dz;
		turns_small[n].v1.x += dx; turns_small[n].v2.x += dx;
		turns_small[n].v1.y += dy; turns_small[n].v2.y += dy;
		turns_small[n].v1.z += dz; turns_small[n].v2.z += dz;
		perps[n].x += dx;
		perps[n].y += dy;
		perps[n].z += dz;
	}

	top.x += dx; bottom.x += dx; connection.x += dx;
	top.y += dy; bottom.y += dy; connection.y += dy;
	top.z += dz; bottom.z += dz; connection.z += dz;

	// Connection done!
}
