#pragma once

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
	
	enum class Keycode;
	enum class KeyMod;
	enum class MouseBtn;
	enum class MouseAxis;
	enum class CtrlBtn;
	enum class CtrlAxis;
}

namespace oeng
{
	using namespace engine;
}
