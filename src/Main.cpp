#include <iostream>
#include <Game.h>

int main(const int argc, char** const argv)
{
	try
	{
		neg::game game;
		game.run_loop();
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		throw;
	}
}
