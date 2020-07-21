#include "Engine.hpp"
#include <SDL2/SDL.h>

namespace oeng::engine
{
	Engine* const kEngine = nullptr;

	Renderer& GetRenderer() noexcept
	{
		assert(kEngine);
		return kEngine->GetRenderer();
	}

	InitEngine::InitEngine(Engine* engine)
	{
		assert(engine);
		assert(kEngine);
		const_cast<Engine*&>(kEngine) = engine;

		log::Info(u8"Initializing engine..."sv);
		
		const auto sdl_result = SDL_Init(SDL_INIT_EVERYTHING);
		if (sdl_result != 0) throw std::runtime_error{SDL_GetError()};
	}

	InitEngine::~InitEngine()
	{
		SDL_Quit();
		const_cast<Engine*&>(kEngine) = nullptr;
	}

	Engine::Engine(std::u8string game_module_path)
		:EngineBase{std::move(game_module_path)}, InitEngine(this)
	{
		log::Info(u8"Engine initialization successful."sv);
		log::Info(u8"Loading game module..."sv);
		GetGameDll().Call<void(Engine&)>(u8"GameMain"sv, *this);
		log::Info(u8"Game module loaded."sv);
	}

	void Engine::RunLoop()
	{
		world_.BeginTick();

		log::Info(u8"Engine loop started."sv);
		const auto start = Clock::now();

		while (is_running_)
		{
			Tick();
			++ticks_;
		}

		const auto elapsed = Clock::now() - start;
		const auto sec = duration_cast<time::seconds>(elapsed).count();
		if (sec != 0 && ticks_ != 0)
		{
			const auto ms = duration_cast<time::duration<Float, std::milli>>(elapsed).count();
			log::Info(u8"Average fps: {}, frame time: {:.2f} ms"sv, ticks_ / sec, ms / ticks_);
		}
	}

	void Engine::Shutdown()
	{
		is_running_ = false;
		log::Info(u8"Engine shutdown requested."sv);
	}

	void Engine::Tick()
	{
		ProcessEvent();
		world_.Tick();
		renderer_.DrawScene();
	}

	void Engine::ProcessEvent()
	{
		input_system_.ClearEvents();
		
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				Shutdown();
				return;
			}
			
			input_system_.AddEvent(event);
		}

		input_system_.PostAddAllEvents();
	}
}
