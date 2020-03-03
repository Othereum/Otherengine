#include <iostream>
#include "application.h"

int main()
{
	try
	{
		game::application app;
		app.run_loop();
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		throw;
	}
}
