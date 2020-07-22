#ifdef OE_MODULE_USE_AVX2
#error Launch module should not use AVX2
#endif

#include "Engine.hpp"
#include "Log.hpp"
#include "Platform.hpp"

static void EngineMain()
{
	oeng::Engine engine{u8"./" U8_TEXT(OE_GAME_MODULE)};
	engine.RunLoop();
}

int main()
{
	using namespace oeng;

	if (IsDebugging())
	{
		EngineMain();
	}
	else
	{
		try
		{
			EngineMain();
		}
		catch (const std::exception& e)
		{
			log::Critical(What(e));
		}
	}
}