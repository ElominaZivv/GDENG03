#include <JAZZY/Core/Logger.h>
#include <iostream>
jazzy::Logger::Logger(LogLevel loglevel): m_logLevel(loglevel)
{
	std::clog << " Logger | Group 5 JAZZY Engine | Object Parenting | GDENG03 " << "\n";
	std::clog << "------------------------------------------------" << "\n";
}

jazzy::Logger::~Logger()
{
}

void jazzy::Logger::log(LogLevel level, const char* message)
{
	auto logLevelToString = [](LogLevel level)
		{
			switch (level)
			{
			case LogLevel::Info: return "Info";
			case LogLevel::Warning: return "Warning";
			case LogLevel::Error: return "Error";
			default: return "Unkown";
			}
		};

	if (level > m_logLevel) return;
	std::clog << "[JAZZY " << logLevelToString(level) << "]: " << message << "\n";
}
