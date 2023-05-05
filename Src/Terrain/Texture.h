#pragma once
#include "GL/glew.h"
#include "SFML/OpenGL.hpp"

#include <string>

struct Texture
{
	Texture(std::string path) : m_path(path)
	{
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		sf::Image image;
		image.loadFromFile(m_path);

		auto size = image.getSize();

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}


	std::string m_path;
	GLuint m_texture;

};