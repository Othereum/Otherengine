#include "Engine.hpp"
#include <SDL2/SDL.h>

namespace oeng::engine
{
	static Engine* engine = nullptr;

	InitEngine::InitEngine(Engine* e)
	{
		assert(e);
		assert(!engine);
		engine = e;

		log::Info(u8"Initializing engine..."sv);
		
		const auto sdl_result = SDL_Init(SDL_INIT_EVERYTHING);
		if (sdl_result != 0) throw std::runtime_error{SDL_GetError()};
	}

	InitEngine::~InitEngine()
	{
		SDL_Quit();
		engine = nullptr;
	}

	Engine& Engine::Get() noexcept
	{
		assert(engine);
		return *engine;
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
		const auto sec = duration_cast<time::duration<Float>>(elapsed).count();
		const auto ms = duration_cast<time::duration<Float, std::milli>>(elapsed).count();
		log::Info(u8"Average fps: {:.0f}, frame time: {:.2f} ms"sv, ticks_ / sec, ms / ticks_);
	}

	void Engine::Shutdown()
	{
		is_running_ = false;
		log::Info(u8"Engine shutdown requested."sv);
	}

	void Engine::Tick()
	{
		TRY(ProcessEvent());
		TRY(world_.Tick());
		TRY(renderer_.DrawScene());
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
