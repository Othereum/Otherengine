#include <iostream>
#include "application.h"

int main()
{
	using namespace game;

	try
	{
		application app;
		app.run_loop();
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		throw;
	}
}
