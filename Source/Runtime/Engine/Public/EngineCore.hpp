#pragma once
#include "Core.hpp"
#include "RendererFwd.hpp"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Rect;
struct _SDL_GameController;
union SDL_Event;

namespace oeng::engine
{
	class Engine;
	class World;
	class AActor;
	class MeshComponent;
	class CircleComponent;
	
	enum class Keycode;
	enum class KeyMod;
	enum class MouseBtn;
	enum class MouseAxis;
	enum class CtrlBtn;
	enum class CtrlAxis;

	[[nodiscard]] ENGINE_API Renderer& GetRenderer() noexcept;
}

namespace oeng
{
	using namespace engine;
}
