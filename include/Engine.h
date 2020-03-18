#pragma once

#include <chrono>
#include <memory>
#include <unordered_map>
#include "Rotation.h"

struct SDL_Window;
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

namespace Game
{
	class CSpriteComponent;
	class CRenderer;
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
		
		[[nodiscard]] std::shared_ptr<SDL_Texture> GetTexture(const char* filename);
		[[nodiscard]] auto GetTime() const noexcept { return time_; }
		[[nodiscard]] CRenderer& GetRenderer() const noexcept { return *renderer_; }
		
	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput() const;
		
		float UpdateTime();
		
		void RegisterActor(std::unique_ptr<AActor>&& actor);
		
		[[nodiscard]] std::shared_ptr<SDL_Texture> LoadTexture(const char* filename);


		std::unique_ptr<CRenderer> renderer_;

		std::unordered_map<std::string, std::weak_ptr<SDL_Texture>> textures_;
		
		std::vector<std::reference_wrapper<const CInputComponent>> inputComps_;
		std::vector<std::reference_wrapper<CCircleComponent>> collisions_;
		
		std::vector<std::unique_ptr<AActor>> actors_;
		std::vector<std::unique_ptr<AActor>> pendingActors_;
		
		std::chrono::steady_clock::time_point time_;

		bool bIsRunning_ = true;
	};
}
