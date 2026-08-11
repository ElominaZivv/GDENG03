#include <JAZZY/Core/Logger.h>

#include <JAZZY/UI/Screens/DebugConsole.h>

#include <iostream>

jazzy::Logger::Logger(LogLevel logLevel) : m_logLevel(logLevel)
{
}

jazzy::Logger::~Logger()
{
}

void jazzy::Logger::SetDebugConsole(DebugConsole* consoleUI)
{
	debugConsole = consoleUI;
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
	std::string logLvlStr = logLevelToString(level);
	std::string fullMessage = "[JAZZY " + logLvlStr + "]: " + message;
	std::clog << fullMessage << "\n";

	debugConsole->AddToDebugLog(fullMessage);
}