#pragma once
#include "Math.hpp"
#include "Asset.hpp"

RENDERER_BEGIN

class RENDERER_API Texture : public Asset
{
public:
	/**
	 * Default black texture of 1x1 size.
	 */
	Texture() noexcept = default;

	/**
	 * Load texture from image file (png, jpg, ...)
	 * @param path Image file path
	 * @throw std::runtime_error If failed to load texture
	 */
	explicit Texture(Path path);
	
	/**
	 * Load texture from image file (png, jpg, ...)
	 * @param path Image file path
	 * @throw std::runtime_error If failed to load texture
	 */
	void Load(Path path)
	{
		Texture{path}.swap(*this);
	}
	
	~Texture();

	Texture(Texture&& r) noexcept
		:Asset{std::move(r)}, id_{r.id_}, size_{r.size_}
	{
		r.id_ = 0;
		r.size_ = {1, 1};
	}

	Texture& operator=(Texture&& r) noexcept
	{
		Texture{std::move(r)}.swap(*this);
		return *this;
	}

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	void Activate() const;
	
	[[nodiscard]] bool Loaded() const noexcept { return id_ != 0; }
	[[nodiscard]] Vec2u16 Size() const noexcept { return size_; }

	void swap(Texture& r) noexcept
	{
		Asset::swap(r);
		using std::swap;
		swap(id_, r.id_);
		swap(size_, r.size_);
	}

private:
	unsigned id_ = 0;
	Vec2u16 size_{1, 1};
};

inline void swap(Texture& a, Texture& b) noexcept { a.swap(b); }

RENDERER_END
