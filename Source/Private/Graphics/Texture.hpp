#pragma once
#include "Math.hpp"

namespace oeng::graphics
{
	class Texture
	{
	public:
		explicit Texture(const char* name);
		~Texture();

		void Activate() const;
		[[nodiscard]] auto& Size() const noexcept { return size_; }

		Texture(const Texture&) = delete;
		Texture(Texture&&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&&) = delete;

	private:
		unsigned id_ = 0;
		Vector<uint16_t, 2> size_;
	};
}