#include <iostream>
#include "Engine.hpp"

int main()
{
	try
	{
		oeng::Engine engine;
		engine.RunLoop();
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		throw;
	}
}
