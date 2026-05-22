#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Logger.h>

namespace jazzy
{
	struct BaseDesc
	{
		Logger& logger;
	};

	struct WindowDesc
	{
		BaseDesc base;
	};

	struct GraphicsEngineDesc
	{
		BaseDesc base;
	};

	struct RenderSystemDesc
	{
		BaseDesc base;
	};

	struct GameDesc
	{
		Logger::LogLevel logLevel = Logger::LogLevel::Error;
	};
}