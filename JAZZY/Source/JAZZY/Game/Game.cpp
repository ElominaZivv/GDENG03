#include <JAZZY/Game/Game.h>
#include <JAZZY/Window/Window.h>
#include <JAZZY/Graphics/GraphicsEngine.h>
#include <JAZZY/Core/Logger.h>
#include<JAZZY/Game/Display.h>
#include <JAZZY/Input/InputSystem.h>
#include <JAZZY/Cube.h>
#include <iostream>
#include <random>
#include <ranges>

jazzy::Game::Game(const GameDesc& desc):
	Base({*std::make_unique<Logger>(desc.logLevel).release()}),
	m_LoggerPtr(&m_logger)
{
	m_inputSystem = std::make_shared<InputSystem>(InputSystemDesc{ m_logger });
	m_inputSystem->setCursorLockArea(desc.windowSize);
	m_graphicsEngine = std::make_unique<GraphicsEngine>(GraphicsEngineDesc{m_logger, m_inputSystem});
	m_display = std::make_unique<Display>(DisplayDesc{ {m_logger, desc.windowSize}, m_graphicsEngine->getGraphicsDevice() });

	m_previousTime = std::chrono::steady_clock::now();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<f32> distrib(-15, 15);
	for (auto i : std::views::iota(0u, 50u))
	{
		Cube newCube({ (f32)distrib(gen), (f32)distrib(gen), -2.0f }, { 0.75f, 0.75f, 0.75f });
		m_graphicsEngine->getCubes()->push_back(newCube);
	}


	DX3DLogInfo("Game initialized.");
}

jazzy::Game::~Game()
{
	DX3DLogInfo("Game is shutting down.");
}

void jazzy::Game::onInternalUpdate()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<f32> delta = currentTime - m_previousTime;
	m_previousTime = currentTime;
	auto deltaTime = delta.count();

	m_inputSystem->update();
	if (m_inputSystem->isKeyPressed(KeyCode::Escape)) m_isRunning = false;

	// DepthTest
	// Spawns Cubes
	if (m_inputSystem->isKeyPressed(KeyCode::Q))
	{
		f32 cube_num = m_graphicsEngine->getCubes()->size();
		Cube newCube({ 0.0f, 0.0f, -2.0f + cube_num }, { 1.0f, 1.0f, 1.0f });
		m_graphicsEngine->getCubes()->push_back(newCube);
	}
	// Spawns Plane
	if (m_inputSystem->isKeyPressed(KeyCode::E))
	{
		Cube newCube({ 0.0f, 0.0f, 0.0f }, { 20.0f, 0.01f, 20.0f });
		m_graphicsEngine->getCubes()->push_back(newCube);
	}

	m_graphicsEngine->render(deltaTime, m_display->getSwapChain());
}