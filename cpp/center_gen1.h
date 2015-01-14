#pragma once

#include <vector>
#include <math.h>
#include "petal_turn.h"
#include "matrix3x3.h"
#include "texturizer.h"

double const PI = 3.14159265358979;

class center_gen1
{

	public:
	
		void generate_center (int petals_number, double sc);
		void display_gl (texturizer* tex);
		void connect (vector3d v1, vector3d v2);
		void get_turn (int index, petal_turn* turn, vector3d* perpendicular);
		matrix3x3 get_basis (void);
		void set_basis (matrix3x3 new_basis);
	
	private:

		std::vector<petal_turn> turns;
		std::vector<petal_turn> turns_small;
		std::vector<vector3d> perps;
		std::vector<vector3d> big_texture_coords;
		std::vector<vector3d> small_texture_coords;
		vector3d bottom, top, connection;
		matrix3x3 basis;

};
