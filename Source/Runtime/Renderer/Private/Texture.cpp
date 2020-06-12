#include "Texture.hpp"
#include <SOIL2/SOIL2.h>
#include "OpenGL.hpp"

namespace oeng
{
	Texture::Texture(Path path)
	{
		LoadInternal(path);
	}

	Texture::Texture(Texture&& r) noexcept
		:id_{r.id_}, size_{r.size_}
	{
		r.id_ = 0;
		r.size_ = {1, 1};
	}

	Texture::~Texture()
	{
		unsigned err;
		// glDelete silently ignores 0
		gl(err, glDeleteTextures, 1, &id_);
	}

	Texture& Texture::operator=(Texture&& r) noexcept
	{
		Texture{std::move(r)}.swap(*this);
		return *this;
	}

	void Texture::LoadInternal(Path path)
	{
		Vector<int, 2> size;
		auto num_channels = 0;
		
		const std::unique_ptr<const unsigned char[]> image{
			SOIL_load_image(path->string().c_str(), &size[0], &size[1], &num_channels, SOIL_LOAD_AUTO)
		};
		if (!image) throw std::runtime_error{SOIL_last_result()};

		constexpr auto max_size = std::numeric_limits<uint16_t>::max();
		if (size[0] > max_size || size[1] > max_size)
			throw std::runtime_error{format("Too big ({0}x{1}). Max size is {2}x{2}", size[0], size[1], max_size)};

		int img_format;
		int alignment;
		switch (num_channels)
		{
		case 3: img_format = GL_RGB; alignment = 1; break;
		case 4: img_format = GL_RGBA; alignment = 4; break;
		default: throw std::runtime_error{format("Invalid format: num_channels was {}. Must be 3 or 4", num_channels)};
		}

		size_ = Vec2u16{size};
		gl(glGenTextures, 1, &id_);
		Activate();

		gl(glPixelStorei, GL_UNPACK_ALIGNMENT, alignment);
		gl(glTexImage2D, GL_TEXTURE_2D, 0, img_format, size[0], size[1], 0, img_format, GL_UNSIGNED_BYTE, image.get());
		
		gl(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		gl(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void Texture::Load(Path path)
	{
		Texture{path}.swap(*this);
	}

	void Texture::Activate() const
	{
		gl(glBindTexture, GL_TEXTURE_2D, id_);
	}

	void Texture::swap(Texture& r) noexcept
	{
		using std::swap;
		swap(id_, r.id_);
		swap(size_, r.size_);
	}
}
