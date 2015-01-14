#include "stdafx.h"
#include "petal_turn.h"

void petal_turn::display_gl (void)
{
	glBegin(GL_LINES);
	glVertex3d ( v1.x, v1.y, v1.z );
	glVertex3d ( v2.x, v2.y, v2.z );
	glEnd();
}

