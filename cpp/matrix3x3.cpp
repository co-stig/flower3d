#include "StdAfx.h"
#include "matrix3x3.h"

double* matrix3x3::operator[] (int n)
{
	return data[n];
}

matrix3x3& matrix3x3::operator= (matrix3x3 d)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			data[i][j] = d[i][j];
	return *this;
}

matrix3x3::matrix3x3 (const matrix3x3& d)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			data[i][j] = d.data[i][j];
}

matrix3x3::matrix3x3 (void)
{
	make_0();
}

matrix3x3 matrix3x3::get_reverse (void)
{
	matrix3x3 result;

	// 1. Get |1 / Det|:

	double det	= data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1])
				- data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0])
				+ data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);
	
	if (det == 0) throw;
	det = fabs(1.0 / det);

	// 2. Get A11, ... , A33

	result[0][0] = det * (data[1][1] * data[2][2] - data[1][2] * data[2][1]);
	result[1][0] =-det * (data[1][0] * data[2][2] - data[1][2] * data[2][0]);
	result[2][0] = det * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);
	
	result[0][1] =-det * (data[0][1] * data[2][2] - data[0][2] * data[2][1]);
	result[1][1] = det * (data[0][0] * data[2][2] - data[0][2] * data[2][0]);
	result[2][1] =-det * (data[0][0] * data[2][1] - data[0][1] * data[2][0]);
	
	result[0][2] = det * (data[0][1] * data[1][2] - data[0][2] * data[1][1]);
	result[1][2] =-det * (data[0][0] * data[1][2] - data[0][2] * data[1][0]);
	result[2][2] = det * (data[0][0] * data[1][1] - data[0][1] * data[1][0]);

	return result;
}

void matrix3x3::multiply_by_vector (vector3d* v)
{
	//double r[3] = {0, 0, 0};
	//for (int i = 0; i < 3; i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//		r[i] += data[i][j] * (*v)[j];
	//}
	//for (int j = 0; j < 3; j++)
	//	(*v)[j] = r[j];

	// ?!! Multiply transponed ?!!

	double r[3] = {0, 0, 0};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			r[i] += data[j][i] * (*v)[j];
	}
	for (int j = 0; j < 3; j++)
		(*v)[j] = r[j];
}

void matrix3x3::make_e (void)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			data[i][j] = 0;
		data[i][i] = 1;
	}
}

void matrix3x3::make_0 (void)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			data[i][j] = 0;
}

void matrix3x3::normalize_cols (void)
{
	for (int i = 0; i < 3; i++)
	{
		double s = sqrt (data[0][i] * data[0][i] + 
			                data[1][i] * data[1][i] + 
					        data[2][i] * data[2][i]);
		if (s != 0)
		{
			data[0][i] /= s;
			data[1][i] /= s;
			data[2][i] /= s;
		}
	}
}

void matrix3x3::swap_rows (int i1, int i2)
{
	for (int i = 0; i < 3; i++)
	{
		double k = data[i1][i];
		data[i1][i] = data[i2][i];
		data[i2][i] = k;
	}
}

void matrix3x3::display (void)
{
}

matrix3x3 matrix3x3::tau_this_H (matrix3x3 H)
{
	matrix3x3 m = (*this);

	for (int y = 0; y < 3; y++) {

		double h[3];
		for (int i = 0; i < 3; i++)
			h[i] = H[i][y];

		for (int c = 0; c < 3; c++)
		{
			if (c == 0)
			{
				if (data[0][0] == 0)
				{
					if (data[1][0] != 0)
					{
						swap_rows (1, 0);
						double t = h[0]; h[0] = h[1]; h[1] = t;
					} else {
						if (data[2][0] != 0) 
						{
							swap_rows (2, 0);
							double t = h[2]; h[2] = h[0]; h[0] = t;
						} else
							throw;
					}
				}
			}

			if (c == 1)
			{
				if (data[1][1] == 0)
				{
					if (data[2][1] != 0)
					{
						double t = h[2]; h[2] = h[1]; h[1] = t;
						swap_rows (1, 2);
					} else {
						throw;
					}
				}
			}
		
			if (c == 2)
			{
				if (data[2][2] == 0)
					throw;
			}

			// G[0][0] != 0
			for (int i = 0; i < 3; i++)
			{
				if (i == c) continue;
				double d = data[i][c] / data[c][c];
				for (int j = 0; j < 3; j++)
					data[i][j] -= (data[c][j] * d);
				// j == 4
				h[i] -= (h[c] * d);
			}
			
		}

		// normalize...
		for (int i = 0; i < 3; i++)
		{
			h[i] /= data[i][i];
			H[i][y] = h[i];
		}

		(*this) = m;
	}
	return H;
}

void matrix3x3::display_gl (void)
{
	glBegin(GL_LINES);
		//glColor3d (1.0, 0.0, 0.0);
		glVertex3d ( 0, 0, 0 );
		glVertex3d ( data[0][0], data[1][0], data[2][0] );
		glVertex3d ( 0, 0, 0 );
		glVertex3d ( data[0][1], data[1][1], data[2][1] );
		glVertex3d ( 0, 0, 0 );
		glVertex3d ( data[0][2], data[1][2], data[2][2] );
	glEnd();
}
