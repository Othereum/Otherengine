#include "Engine.hpp"
#include <Windows.h>

int main()
{
	auto* const game = LoadLibraryA(OE_GAME_NAME"-Module");
	assert(game);

	const auto load = reinterpret_cast<void(*)(oeng::Engine&)>(GetProcAddress(game, "LoadGame"));
	assert(load);
	
	Main("Test Game", load);
}
