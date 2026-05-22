#include <All.h>
#include <iostream>
#include <string>
	
int main()
{
	int zzWindowDisplayHeight = 400;	// Originally 720
	int zzWindowDisplayWidth = zzWindowDisplayHeight * 1.78;	// Originally 1280

	try
	{
		jazzy::Game game({ {zzWindowDisplayWidth, zzWindowDisplayHeight}, jazzy::Logger::LogLevel::Info });
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
