#include <JAZZY/Game/Game.h>
#include <JAZZY/Window/Window.h>
#include <JAZZY/Graphics/GraphicsEngine.h>
#include <JAZZY/Core/Logger.h>

jazzy::Game::Game()
{
	m_LoggerPtr = std::make_unique<Logger>(Logger::LogLevel::Info);
	m_graphicsEngine = std::make_unique<GraphicsEngine>();
	m_display = std::make_unique<Window>();

	m_LoggerPtr->log(Logger::LogLevel::Info, "Game Initialized.");
}

jazzy::Game::~Game()
{
	m_LoggerPtr->log(Logger::LogLevel::Info, "Game deallocation started.");
}