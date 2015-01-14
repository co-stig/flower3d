#pragma once

#include <vector>
#include "petal_turn.h"

class petal_gen1
{
	public:

		petal_gen1(void);
		~petal_gen1(void);
		void generate_petal (std::vector<petal_turn>* turns, vector3d* perpendicular);
};
