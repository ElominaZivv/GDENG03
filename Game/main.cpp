#include <All.h>
#include <iostream>
#include <string>
	
// hello

int main()
{
	try
	{
		jazzy::Game game({ {1280, 720}, jazzy::Logger::LogLevel::Info });
		game.run();
	}
	catch (const std::runtime_error&)
	{
		return EXIT_FAILURE;
	}
	catch (const std::invalid_argument)
	{
		return EXIT_FAILURE;
	}
	catch (const std::exception&)
	{
		return EXIT_FAILURE;
	}
	catch (...)
	{
		return EXIT_FAILURE;
	}
	return 0;
}
