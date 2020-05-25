#include "Texture.hpp"
#include <GL/glew.h>
#include <SOIL2/SOIL2.h>
#include <fmt/core.h>

namespace oeng
{
	Texture::Texture(Path file)
	{
		Vector<int, 2> size;
		auto num_channels = 0;

		auto str = file->string();
		const std::unique_ptr<unsigned char[]> image{SOIL_load_image(str.c_str(), &size[0], &size[1], &num_channels, SOIL_LOAD_AUTO)};
		if (!image) throw std::runtime_error{fmt::format("Failed to load image '{}': {}", str, SOIL_last_result())};

		const auto format = num_channels == 4 ? GL_RGBA : GL_RGB;
		size_ = Vec2u16{size};

		glGenTextures(1, &id_);
		Activate();
		
		glTexImage2D(GL_TEXTURE_2D, 0, format, size[0], size[1], 0, format, GL_UNSIGNED_BYTE, image.get());
		
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
