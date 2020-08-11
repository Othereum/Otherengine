#ifdef OE_MODULE_USE_AVX2
#error Launch module should not use AVX2
#endif

#include "Engine.hpp"

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
			OE_LOG(kEngine, kCritical, What(e));
			return EXIT_FAILURE;
		}
	}
}
