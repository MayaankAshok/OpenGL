#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifndef STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#endif // !STB_IMAGE_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION


class Texture{

public:
	unsigned int m_RendererID;
	void AddTexture(const char* filename);
	Texture(const char* filename);
	~Texture();
	void Bind();
	void UnBind();
};


