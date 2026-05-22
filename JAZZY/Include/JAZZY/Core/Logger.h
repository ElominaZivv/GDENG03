#pragma once
namespace jazzy
{
	class Logger final
	{
	public:
		enum class LogLevel
		{
			Error = 0,
			Warning,
			Info
		};

		explicit Logger(LogLevel loglevel = LogLevel::Error);

		void log(LogLevel level, const char* message) const;
	private:
		LogLevel m_logLevel = LogLevel::Error;
	};
}
