#pragma once

#include "vector3d.h"

struct petal_turn
{
	vector3d v1, v2;
	// COLORREF c1, c2;
	void display_gl (void);
};
