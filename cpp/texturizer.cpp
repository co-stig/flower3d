#include "texturizer.h"
#include <string>

texturizer::texturizer(void)
: textures (0)
{
}

texturizer::~texturizer(void)
{
	delete[] textures;
}

bool texturizer::load_textures (int count)
{
	delete[] textures;
	textures = new (GLuint[count]);

	for (int l = 0; l < count; l++)
	{
		std::string filename = "data/texture" + l;
		textures [l] = SOIL_load_OGL_texture(
				filename.c_str(),
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
			);
	}

	return true;
}

GLuint texturizer::get_texture (int num)
{
	return textures[num];
}

GLuint texturizer::operator[] (int num)
{
	return get_texture(num);
}
