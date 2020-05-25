#pragma once
#include <memory>
#include "ActorComponent.hpp"
#include "Path.hpp"

struct SDL_Renderer;

namespace oeng
{
	class Renderer;
	class Texture;
	class Shader;
	
	class CSpriteComponent : public CActorComponent
	{
	public:
		explicit CSpriteComponent(AActor& owner, int draw_order = 100, int update_order = 100);
		~CSpriteComponent();

		void BeginPlay() override;
		
		void SetTexture(std::shared_ptr<Texture>&& texture);
		void SetTexture(const std::shared_ptr<Texture>& texture);
		void SetTexture(Path file);
		[[nodiscard]] Texture& GetTexture() const { return *texture_; }

		[[nodiscard]] int GetDrawOrder() const noexcept { return draw_order_; }
		[[nodiscard]] Renderer& GetRenderer() const noexcept;

		void Draw(Shader& shader) const;

	private:
		std::shared_ptr<Texture> texture_;
		int draw_order_;
	};
}