#include "Window.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "EngineBase.hpp"
#include "OpenGL.hpp"

namespace oeng::renderer
{
	static void SetGlAttribute(SDL_GLattr attr, int value)
	{
		if (0 != SDL_GL_SetAttribute(attr, value))
			throw std::runtime_error{SDL_GetError()};
	}

	class WindowCreator
	{
	public:
		DELETE_CPMV(WindowCreator);
		
		WindowCreator()
		{
			WriteDisplayModes();
			LoadDisplayIdx();
			WriteWindowSize();
			LoadDisplayMode();
			SetupAttributes();
		}
		
		[[nodiscard]] SDL_Window* operator()() const
		{
			auto* const window = SDL_CreateWindow(
				AsString(EngineBase::Get().GetGameName().data()),
				SDL_WINDOWPOS_CENTERED_DISPLAY(dp_idx_), SDL_WINDOWPOS_CENTERED_DISPLAY(dp_idx_),
				dp_mode_.w, dp_mode_.h, flags_
			);

			if (!window) throw std::runtime_error{SDL_GetError()};
			return window;
		}

		~WindowCreator()
		{
			(void)ConfigSystem::Get().Save(cfg_name_);
		}

	private:
		void WriteDisplayModes() const
		{
			const auto num_dp = SDL_GetNumVideoDisplays();
			if (num_dp < 1) throw std::runtime_error{SDL_GetError()};
			
			auto& displays = cfg_["Display modes (read only)"] = Json::object();
			for (auto dp = 0; dp < num_dp; ++dp)
			{
				const auto num_dm = SDL_GetNumDisplayModes(dp);
				if (num_dm < 1) throw std::runtime_error{SDL_GetError()};

				auto& modes = displays[fmt::format("Display {}", dp)] = Json::array();
				for (auto dm = 0; dm < num_dm; ++dm)
				{
					SDL_DisplayMode mode;
					SDL_GetDisplayMode(dp, dm, &mode);
					auto str = Format(u8"[{}] {}x{} {}Hz"sv, dm, mode.w, mode.h, mode.refresh_rate);
					modes.emplace_back(AsString(std::move(str)));
				}
			}
		}

		void WriteWindowSize() const
		{
			if (!cfg_.contains("WindowedSize"))
			{
				SDL_DisplayMode dp_mode;
				SDL_GetDesktopDisplayMode(dp_idx_, &dp_mode);
				Vector size{dp_mode.w, dp_mode.h};
				cfg_["WindowedSize"] = (size *= 5, size /= 6);
			}
		}
		
		void LoadDisplayIdx()
		{
			const auto num_dp = SDL_GetNumVideoDisplays();
			if (dp_idx_ >= num_dp)
			{
				log::Warn(u8"Attempted to use a non-existent display (tried: {}, max: {})"sv, dp_idx_, num_dp-1);
				log::Warn(u8"Using display 0..."sv);
				dp_idx_ = 0;
			}
		}

		void LoadDisplayMode()
		{
			if (fullscreen_) LoadFullscreenDpMode();
			else LoadWindowedDpMode();
		}

		void LoadFullscreenDpMode()
		{
			auto dm_idx = cfg_.at("FullscreenDisplayMode").get<int>();
			const auto num_dm = SDL_GetNumDisplayModes(dp_idx_);
			
			if (dm_idx >= num_dm)
			{
				log::Warn(u8"Attempted to use a non-existent display mode (tried: {}, max: {})"sv, dm_idx, num_dm-1);
				log::Warn(u8"Using display mode 0..."sv);
				dm_idx = 0;
			}

			if (0 != SDL_GetDisplayMode(dp_idx_, dm_idx, &dp_mode_))
				throw std::runtime_error{SDL_GetError()};

			log::Info(u8"Fullscreen mode: {}x{} {}Hz"sv, dp_mode_.w, dp_mode_.h, dp_mode_.refresh_rate);
		}

		void LoadWindowedDpMode()
		{
			const auto size = cfg_.at("WindowedSize").get<Vec2u16>();
			dp_mode_.w = size[0];
			dp_mode_.h = size[1];
			log::Info(u8"Windowed mode: {}x{}"sv, dp_mode_.w, dp_mode_.h);
		}

		static void SetupAttributes()
		{
			SetGlAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SetGlAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OE_OPENGL_VERSION_MAJOR);
			SetGlAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OE_OPENGL_VERSION_MINOR);
			SetGlAttribute(SDL_GL_RED_SIZE, 8);
			SetGlAttribute(SDL_GL_GREEN_SIZE, 8);
			SetGlAttribute(SDL_GL_BLUE_SIZE, 8);
			SetGlAttribute(SDL_GL_ALPHA_SIZE, 8);
			SetGlAttribute(SDL_GL_DEPTH_SIZE, 24);
			SetGlAttribute(SDL_GL_DOUBLEBUFFER, true);
			SetGlAttribute(SDL_GL_ACCELERATED_VISUAL, true);
		}

		const Name cfg_name_{u8"Display"sv};
		Json& cfg_{ConfigSystem::Get()(cfg_name_)};

		const bool fullscreen_ = cfg_.at("Fullscreen").get<bool>();
		const uint32_t flags_ = SDL_WINDOW_OPENGL | (fullscreen_ ? SDL_WINDOW_FULLSCREEN : 0);
		
		int dp_idx_ = cfg_.at("Display").get<int>();
		SDL_DisplayMode dp_mode_{};
	};

	static void* CreateGlContext(SDL_Window* window)
	{
		auto* const context = SDL_GL_CreateContext(window);
		if (!context) throw std::runtime_error{SDL_GetError()};
		
		glewExperimental = true;

		if (const auto err = glewInit(); err != GLEW_OK)
			throw std::runtime_error{reinterpret_cast<const char*>(glewGetErrorString(err))};
		
		// On some platforms, GLEW will emit a benign error code, so clear it
		glGetError();

		auto& cfg = ConfigSystem::Get()(u8"Display"sv);
		if (cfg.at("VSync").get<bool>())
		{
			if (cfg.at("VSync_Adaptive").get<bool>())
			{
				if (SDL_GL_SetSwapInterval(-1) == 0)
				{
					log::Info(u8"Adaptive sync enabled"sv);
				}
				else
				{
					log::Warn(u8"Adaptive sync not supported. Falling back to normal VSync..."sv);
					SDL_GL_SetSwapInterval(1);
				}
			}
			else
			{
				log::Info(u8"VSync enabled"sv);
				SDL_GL_SetSwapInterval(1);
			}
		}
		else
		{
			log::Info(u8"VSync disabled"sv);
			SDL_GL_SetSwapInterval(0);
		}

		gl(glClearColor, 0.f, 0.f, 0.f, 1.f);
		
		return context;
	}

	Window::Window()
		:window_{WindowCreator{}()},
		gl_context_{CreateGlContext(window_)}
	{
	}

	Window::~Window()
	{
		SDL_DestroyWindow(window_);
		SDL_GL_DeleteContext(gl_context_);
	}

	Vec2u16 Window::GetSize() const noexcept
	{
		int w, h;
		SDL_GetWindowSize(window_, &w, &h);
		return {uint16_t(w), uint16_t(h)};
	}

	void Window::SetTitle(std::u8string_view title) const noexcept
	{
		SDL_SetWindowTitle(window_, AsString(title.data()));
	}

	void Window::SwapBuffer() const noexcept
	{
		SDL_GL_SwapWindow(window_);
	}
}
