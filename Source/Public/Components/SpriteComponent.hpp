#pragma once
#include "ActorComponent.hpp"
#include "Path.hpp"

struct SDL_Renderer;

namespace oeng
{
	class Renderer;
	class Texture;
	class Shader;
	
	class SpriteComponent : public ActorComponent
	{
	public:
		explicit SpriteComponent(AActor& owner, int draw_order = 100, int update_order = 100);
		~SpriteComponent();

		void Draw(Shader& shader) const noexcept;

		void SetTexture(Path file);
		void SetTexture(SharedPtr<Texture> texture) noexcept { texture_ = std::move(texture); }
		[[nodiscard]] Texture& GetTexture() const { return *texture_; }

		[[nodiscard]] int GetDrawOrder() const noexcept { return draw_order_; }
		[[nodiscard]] Renderer& GetRenderer() const noexcept;

	private:
		void OnBeginPlay() override;
		
		SharedPtr<Texture> texture_;
		int draw_order_;
	};
}