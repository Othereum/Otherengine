#include <iostream>
#include "application.h"

int main(const int argc, char** const argv)
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
