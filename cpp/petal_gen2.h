#pragma once

#include <vector>
#include "petal_turn.h"

class petal_gen2
{
	public:

		petal_gen2(void);
		~petal_gen2(void);
		void generate_petal (std::vector<petal_turn>* turns, vector3d* perpendicular);
};
