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

	// Plane
	Vec3 scale(2.5, 3.5, 0.001);
	Vec3 scale2(2.5, 0.001, 3.5);
	Vec3 rotation(0.0f, 0.0f, 0.0f);
	f32 angle = 0.44f;
	Vec3 rotation1(angle, 0.0f, 0.0f);
	Vec3 rotation2(-angle, 0.0f, 0.0f);
	Cube plane1	({ 0.0f, 0.0f, -6.0f }, scale, rotation1);
	Cube plane2	({ 0.0f, 0.0f, -3.0f }, scale, rotation2);
	Cube plane3({ 0.0f, 0.0f, 0.0f }, scale, rotation1);
	Cube plane4({ 0.0f, 0.0f, 3.0f }, scale, rotation2);
	Cube plane5({ 0.0f, 0.0f, 6.0f }, scale, rotation1);
	Cube plane6({ 0.0f, 0.0f, 9.0f }, scale, rotation2);
	Cube plane7({ 0.0f, 4*3.16f, 0.0f }, scale, rotation1);
	Cube plane8({ 0.0f, 4*3.16f, 3.0f }, scale, rotation2);
	Cube plane9({ 0.0f, 2*3.16f, -3.0f }, scale, rotation1);
	Cube plane10({ 0.0f, 2*3.16f, 0.0f }, scale, rotation2);
	Cube plane11({ 0.0f, 2*3.16f, 3.0f }, scale, rotation1);
	Cube plane12({ 0.0f, 2*3.16f, 6.0f }, scale, rotation2);
	Cube plane13({ 0.0f, 3.16f, -1.5f }, scale2, rotation);
	Cube plane14({ 0.0f, 3.16f, 4.5f }, scale2, rotation);
	Cube plane15({ 0.0f, 3*3.16f, 1.5f }, scale2, rotation);

	m_graphicsEngine->getCubes()->push_back(plane1);
	m_graphicsEngine->getCubes()->push_back(plane2);
	m_graphicsEngine->getCubes()->push_back(plane3);
	m_graphicsEngine->getCubes()->push_back(plane4);
	m_graphicsEngine->getCubes()->push_back(plane5);
	m_graphicsEngine->getCubes()->push_back(plane6);
	m_graphicsEngine->getCubes()->push_back(plane7);
	m_graphicsEngine->getCubes()->push_back(plane8);
	m_graphicsEngine->getCubes()->push_back(plane9);
	m_graphicsEngine->getCubes()->push_back(plane10);
	m_graphicsEngine->getCubes()->push_back(plane11);
	m_graphicsEngine->getCubes()->push_back(plane12);
	m_graphicsEngine->getCubes()->push_back(plane13);
	m_graphicsEngine->getCubes()->push_back(plane14);
	m_graphicsEngine->getCubes()->push_back(plane15);

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

	m_graphicsEngine->render(deltaTime, m_display->getSwapChain());
}