#pragma once

namespace oeng
{
	class Shader;
	class Renderer;
	
	class ISprite
	{
	public:
		virtual void Draw(Shader& shader) const noexcept = 0;
		[[nodiscard]] virtual int GetDrawOrder() const noexcept = 0;

		ISprite(const ISprite&) = delete;
		ISprite(ISprite&&) = delete;
		ISprite& operator=(const ISprite&) = delete;
		ISprite& operator=(ISprite&&) = delete;

	protected:
		constexpr ISprite() noexcept = default;
		~ISprite() = default;
	};

	class IMesh
	{
	public:
		virtual void Draw() const noexcept = 0;

		IMesh(const IMesh&) = delete;
		IMesh(IMesh&&) = delete;
		IMesh& operator=(const IMesh&) = delete;
		IMesh& operator=(IMesh&&) = delete;

	protected:
		constexpr IMesh() noexcept = default;
		~IMesh() = default;
	};
}
