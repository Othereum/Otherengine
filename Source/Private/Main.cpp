#include <iostream>
#include "Engine.h"

int main()
{
	try
	{
		oeng::CEngine engine;
		engine.RunLoop();
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		throw;
	}
}
