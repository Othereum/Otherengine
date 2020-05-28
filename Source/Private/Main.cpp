#include <iostream>
#include "Engine.hpp"

using namespace oeng;

int main()
{
	try
	{
		Engine engine;
		engine.RunLoop();
	}
	catch (const std::exception& e)
	{
		std::cerr << "[UNHANDLED EXCEPTION] " << e.what() << std::endl;
		throw;
	}
}
