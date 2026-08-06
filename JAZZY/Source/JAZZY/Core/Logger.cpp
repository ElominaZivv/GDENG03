#include <JAZZY/Core/Logger.h>
#include <iostream>

jazzy::Logger::Logger(LogLevel logLevel) : m_logLevel(logLevel)
{
}

jazzy::Logger::~Logger()
{
}

void jazzy::Logger::_log(LogLevel level, const char* message)
{
	auto logLevelToString = [](LogLevel level) {
		switch (level)
		{
		case LogLevel::Info: return "Info";
		case LogLevel::Warning: return "Warning";
		case LogLevel::Error: return "Error";
		default: return "Unknown";
		}
		};

	if (level > m_logLevel) return;
	std::clog << "[JAZZY " << logLevelToString(level) << "]: " << message << "\n";
}