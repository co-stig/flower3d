#pragma once

#include "stdafx.h"

class texturizer
{

	public:

		texturizer(void);
		~texturizer(void);
		bool load_textures (int count);
		GLuint get_texture (int num);
		GLuint operator[] (int num);

	private:

		AUX_RGBImageRec* load_BMP(char* filename);
		GLuint*	textures;

};
