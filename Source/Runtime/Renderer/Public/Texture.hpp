#pragma once
#include "Math.hpp"
#include "Path.hpp"

namespace oeng
{
	class OEAPI Texture
	{
	public:
		/**
		 * \brief Default black texture of 1x1 size. It can be used for rendering, just like normal loaded textures.
		 */
		constexpr Texture() noexcept = default;

		/**
		 * \brief Load texture from image file (png, jpg, ...)
		 * \param path Image file path
		 * \throw std::runtime_error If failed to load texture
		 */
		explicit Texture(Path path);
		
		Texture(Texture&& r) noexcept;
		Texture(const Texture&) = delete;
		~Texture();

		Texture& operator=(Texture&& r) noexcept;
		Texture& operator=(const Texture&) = delete;

		/**
		 * \brief Load texture from image file (png, jpg, ...)
		 * \param path Image file path
		 * \throw std::runtime_error If failed to load texture
		 */
		void Load(Path path);
		
		void Activate() const;
		
		[[nodiscard]] bool Loaded() const noexcept { return id_ != 0; }
		[[nodiscard]] Vec2u16 Size() const noexcept { return size_; }

		void swap(Texture& r) noexcept;

	private:
		void LoadInternal(Path path);

		unsigned id_ = 0;
		Vec2u16 size_{1, 1};
	};

	inline void swap(Texture& a, Texture& b) noexcept { a.swap(b); }
}