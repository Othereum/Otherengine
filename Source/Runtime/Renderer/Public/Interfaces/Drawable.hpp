#pragma once

namespace oeng
{
	class Shader;
	class Renderer;
	
	class ISprite
	{
	public:
		virtual void Draw(Shader& shader) const noexcept = 0;
		virtual int GetDrawOrder() const noexcept = 0;

		ISprite(const ISprite&) = default;
		ISprite(ISprite&&) = default;
		ISprite& operator=(const ISprite&) = default;
		ISprite& operator=(ISprite&&) = default;

	protected:
		~ISprite() = default;
	};

	class IMesh
	{
	public:
		virtual void Draw() const noexcept = 0;

		IMesh(const IMesh&) = default;
		IMesh(IMesh&&) = default;
		IMesh& operator=(const IMesh&) = default;
		IMesh& operator=(IMesh&&) = default;

	protected:
		~IMesh() = default;
	};
}
