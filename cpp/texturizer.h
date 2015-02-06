#pragma once

#include "stdafx.h"
#include "SOIL.h"

class texturizer
{

	public:

		texturizer(void);
		~texturizer(void);
		bool load_textures (int count);
		GLuint get_texture (int num);
		GLuint operator[] (int num);

	private:

		GLuint*	textures;

};
