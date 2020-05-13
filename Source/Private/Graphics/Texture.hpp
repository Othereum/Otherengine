#pragma once
#include "Math.hpp"

namespace oeng
{
	class Texture
	{
	public:
		explicit Texture(std::string_view name);
		~Texture();

		void Activate() const;
		[[nodiscard]] auto& Size() const noexcept { return size_; }

		Texture(const Texture&) = delete;
		Texture(Texture&&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&&) = delete;

	private:
		unsigned id_ = 0;
		Vec2u16 size_;
	};
}