#include <JAZZY/Game/Game.h>
#include <JAZZY/Window/Window.h>

jazzy::Game::Game()
{
	m_display = std::make_unique<Window>();
}

jazzy::Game::~Game()
{
}