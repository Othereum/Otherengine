#include "Texture.hpp"

#include <GL/glew.h>
#include <SOIL2/SOIL2.h>

#include "StringUtil.hpp"

namespace oeng::graphics
{
	Texture::Texture(const char* name)
	{
		Vector<int, 2> size;
		auto num_channels = 0;

		const std::unique_ptr<unsigned char[]> image{SOIL_load_image(name, &size.x, &size.y, &num_channels, SOIL_LOAD_AUTO)};
		if (!image) throw std::runtime_error{str::Concat("Failed to load image ", name, ": ", SOIL_last_result())};

		const auto format = num_channels == 4 ? GL_RGBA : GL_RGB;
		size_ = size;

		glGenTextures(1, &id_);
		
		glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, image.get());
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &id_);
	}

	void Texture::Activate() const
	{
		glBindTexture(GL_TEXTURE_2D, id_);
	}
}
