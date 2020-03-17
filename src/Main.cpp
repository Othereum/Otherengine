#include <iostream>
#include "Engine.h"

int main()
{
	using namespace Game;

	try
	{
		CEngine app;
		app.RunLoop();
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		throw;
	}
}
