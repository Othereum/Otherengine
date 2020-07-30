#pragma once
#include "Interfaces/Drawable.hpp"
#include "SceneComponent.hpp"
#include "Path.hpp"

struct SDL_Renderer;

namespace oeng::renderer
{
	class Renderer;
	class Texture;
}

namespace oeng::engine
{
	class ENGINE_API SpriteComponent : public SceneComponent, public ISpriteComponent
	{
	public:
		DELETE_CPMV(SpriteComponent);
		
		explicit SpriteComponent(AActor& owner, int draw_order = 100, int update_order = 100);
		~SpriteComponent();

		[[nodiscard]] bool ShouldDraw() const noexcept override { return IsActive(); }
		[[nodiscard]] int GetDrawOrder() const noexcept override { return draw_order_; }
		[[nodiscard]] const Mat4& GetDrawTrsf() const noexcept override { return GetWorldTrsfMatrix(); }

		void SetTexture(Path file);
		void SetTexture(SharedRef<Texture> texture) noexcept { texture_ = std::move(texture); }
		[[nodiscard]] Texture& GetTexture() const noexcept override { return *texture_; }

	protected:
		void OnBeginPlay() override;

	private:
		SharedRef<Texture> texture_;
		int draw_order_;
	};
}