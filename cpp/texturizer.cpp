#include "texturizer.h"

texturizer::texturizer(void)
: textures (0)
{
}

texturizer::~texturizer(void)
{
	delete[] textures;
}

AUX_RGBImageRec* texturizer::load_BMP(char* filename)
{
	FILE* f = fopen(filename, "r");
	if (f)
	{
		fclose(f);
		return auxDIBImageLoad(filename);
	} else {
		return 0;
	}
}

bool texturizer::load_textures (int count)
{
	delete[] textures;
	textures = new (GLuint[count]);
	AUX_RGBImageRec** timage = new (AUX_RGBImageRec* [count]);

	for (int l = 0; l < count; l++)
	{
		CString s;
		s.Format("Data/texture%d.bmp", l);

		if (timage[l] = load_BMP(s.GetBuffer()))
		{
			glGenTextures(1, &textures[l]);
			glBindTexture(GL_TEXTURE_2D, textures[l]);
			glTexImage2D(
				GL_TEXTURE_2D, 
				0, 
				3, 
				timage[l]->sizeX, 
				timage[l]->sizeY, 
				0, 
				GL_RGB, 
				GL_UNSIGNED_BYTE, 
				timage[l]->data
				);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		} 
		else 
		{
			delete[] timage;
			return false;
		}

		if (timage[l])
		{
			if (timage[l]->data) free(timage[l]->data);
			free(timage[l]);
		}
	}

	delete[] timage;
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
