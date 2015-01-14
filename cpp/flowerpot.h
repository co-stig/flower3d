#pragma once

#include "stdafx.h"
#include "texturizer.h"
#include "vector3d.h"
#include <vector>
#include <math.h>

class flowerpot
{

	public:
		
		void display_gl (texturizer* tex);
		void generate_flowerpot (double radius);

	private:

		std::vector<vector3d> turns;

};
