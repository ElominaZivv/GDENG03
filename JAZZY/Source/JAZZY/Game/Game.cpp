#include <ranges>
#include <JAZZY/Game/Game.h>
#include <JAZZY/Window/Window.h>
#include <JAZZY/Graphics/GraphicsEngine.h>
#include <JAZZY/Core/Logger.h>
#include<JAZZY/Game/Display.h>
#include <JAZZY/Input/InputSystem.h>
#include <JAZZY/Game/World.h>
#include <JAZZY/Cube.h>

#include "JAZZY/EditorCamera/EditorCamera.h"
#include "JAZZY/Game/GameObject.h"

jazzy::Game::Game(const GameDesc& desc):
	Base({*std::make_unique<Logger>(desc.logLevel).release()}),
	m_LoggerPtr(&m_logger)
{
	m_inputSystem = std::make_shared<InputSystem>(InputSystemDesc{ m_logger });
	m_inputSystem->setCursorLockArea(desc.windowSize);
	m_editorCamera = std::make_shared<EditorCamera>(EditorCameraDesc{ m_logger , m_inputSystem });
	m_graphicsEngine = std::make_unique<GraphicsEngine>(GraphicsEngineDesc{m_logger, m_editorCamera});
	m_display = std::make_unique<Display>(DisplayDesc{ {m_logger, desc.windowSize}, m_graphicsEngine->getGraphicsDevice() });
	m_world = std::make_unique<World>(WorldDesc{ BaseDesc{m_logger}, GameContext{*m_inputSystem} });

	m_previousTime = std::chrono::steady_clock::now();

	m_world->createGameObject<jazzy::GameObject>();

	// Spawn plane
	Cube newPlane({ 0.0f, -1.0f, 0.0f }, { 20.0f, 0.01f, 20.0f });
	m_graphicsEngine->getCubes()->push_back(newPlane);
	// Spawn a few cubes into the scene
	for (auto i : std::views::iota(0u, 10u))
	{
		f32 cube_num = m_graphicsEngine->getCubes()->size();
		Cube newCube({ 1.0f, -0.25f, -4.0f + cube_num }, { 1.0f, 1.0f, 1.0f });
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
	// Time
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<f32> delta = currentTime - m_previousTime;
	m_previousTime = currentTime;
	auto deltaTime = delta.count();

	// Input System
	m_inputSystem->update();
	if (m_inputSystem->isKeyPressed(KeyCode::Escape)) m_isRunning = false;	// Close the program

	// World
	m_world->update(deltaTime);

	// Editor Camera
	m_inputSystem->setCursorLockArea(m_display->getClientAreaInScreenSpace());
	m_inputSystem->setCursorLocked(true);
	m_inputSystem->setCursorVisible(false);
	m_editorCamera->setDisplayRect(m_display->getClientAreaInScreenSpace());
	m_editorCamera->update(deltaTime);

	// Graphics Engine
	m_graphicsEngine->render(deltaTime, m_display->getSwapChain());
}