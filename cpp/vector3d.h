#pragma once

#include "stdafx.h"

struct vector3d
{

	vector3d (double xx, double yy, double zz);
	vector3d (void) { };
	
	double& operator[] (int n);
	vector3d operator- (vector3d v);
	vector3d operator+ (vector3d v);
	vector3d operator* (double f);

	void display (void);
	void display_gl (void);
	double length (void);
	vector3d get_normalized (void);

	double x, y, z;

};

