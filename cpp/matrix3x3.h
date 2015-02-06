#pragma once

#include "stdafx.h"
#include <math.h>
#include "vector3d.h"

class matrix3x3
{

private:

	double data[3][3];

public:

	double* operator[] (int n);
	matrix3x3& operator= (matrix3x3 d);
	matrix3x3 (const matrix3x3& d);
	matrix3x3 (void);
	void multiply_by_vector (vector3d* v);
	void make_e (void);
	void make_0 (void);
	void normalize_cols (void);
	void swap_rows (int i1, int i2);
	void display (void);
	void display_gl (void);
	matrix3x3 tau_this_H (matrix3x3 H);
	matrix3x3 get_reverse (void);

};
