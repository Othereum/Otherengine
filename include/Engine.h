#pragma once

#include <chrono>
#include <memory>
#include <unordered_map>

#include "Rotation.h"
#include "Rect.h"

struct SDL_Window;
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

namespace Game
{
	class CSpriteComponent;
	constexpr FVector2<uint16_t> kScrSz{1024, 768};

	using TWindowPtr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;
	using TRendererPtr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
	
	class AActor;
	class CInputComponent;
	class CCircleComponent;

	class CSdlRaii
	{
	public:
		CSdlRaii();
		~CSdlRaii();
	};
	
	class CEngine : CSdlRaii
	{
	public:
		CEngine();
		~CEngine();
		
		void RunLoop();
		void Shutdown();

		
		template <class T>
		T& SpawnActor()
		{
			static_assert(std::is_base_of_v<AActor, T>);
			auto ptr = std::make_unique<T>(*this);
			auto& actor = *ptr;
			RegisterActor(std::move(ptr));
			return actor;
		}
		
		void RegisterInputComponent(const CInputComponent& comp);
		void UnregisterInputComponent(const CInputComponent& comp);

		void RegisterCollision(CCircleComponent& comp);
		void UnregisterCollision(CCircleComponent& comp);

		void RegisterSprite(const CSpriteComponent& sprite);
		void UnregisterSprite(const CSpriteComponent& sprite);

		
		void Draw(SDL_Texture& texture, const TFRect& dest, TDegrees angle = {}) const;
		void Draw(SDL_Texture& texture, const SDL_Rect& src, const SDL_Rect& dest, TDegrees angle = {}) const;
		
		[[nodiscard]] std::shared_ptr<SDL_Texture> GetTexture(const char* filename);


		[[nodiscard]] auto GetTime() const noexcept { return time_; }

		
	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();
		
		float UpdateTime();
		
		void RegisterActor(std::unique_ptr<AActor>&& actor);
		
		[[nodiscard]] std::shared_ptr<SDL_Texture> LoadTexture(const char* filename);


		TWindowPtr window_;
		TRendererPtr renderer_;

		std::unordered_map<std::string, std::weak_ptr<SDL_Texture>> textures_;
		
		std::vector<std::reference_wrapper<const CSpriteComponent>> sprites_;
		std::vector<std::reference_wrapper<const CInputComponent>> inputComps_;
		std::vector<std::reference_wrapper<CCircleComponent>> collisions_;
		
		std::vector<std::unique_ptr<AActor>> actors_;
		std::vector<std::unique_ptr<AActor>> pendingActors_;
		
		std::chrono::steady_clock::time_point time_;

		bool bIsRunning_ = true;
	};
}
