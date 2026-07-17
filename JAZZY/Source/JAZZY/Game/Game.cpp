#include <ranges>
#include <JAZZY/Game/Game.h>
#include <JAZZY/Window/Window.h>
#include <JAZZY/Graphics/GraphicsEngine.h>
#include <JAZZY/Core/Logger.h>
#include<JAZZY/Game/Display.h>
#include <JAZZY/Input/InputSystem.h>
#include <JAZZY/Game/World.h>

#include "JAZZY/Components/CubeComponent.h"
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
	m_world = std::make_unique<World>(WorldDesc{ BaseDesc{m_logger}, GameContext{*m_inputSystem, m_graphicsEngine->getGraphicsDevice()} });
	m_graphicsEngine->initializeUI(m_display->getHWND(),*m_world);

	m_previousTime = std::chrono::steady_clock::now();

	auto plane = m_world->createGameObject<jazzy::GameObject>();
	plane->createOrGetComponent<jazzy::CubeComponent>();
	TransformComponent* plane_transform = plane->createOrGetComponent<jazzy::TransformComponent>();
	plane_transform->setPosition({ 0.0f, -5.0f, 0.0f });
	plane_transform->setScale({ 15.0f, 0.05f, 15.0f });

	auto cube = m_world->createGameObject<jazzy::GameObject>();
	cube->createOrGetComponent<jazzy::CubeComponent>();
	TransformComponent* cube_transform = cube->createOrGetComponent<jazzy::TransformComponent>();
	cube_transform->setPosition({ 0.0f, -1.0f, 0.0f });
	cube_transform->setRotation({ 3.14f, 0.0f, 0.0f });

	cube->setParent(plane);
	auto parent = cube->getParent();
	if (parent)
	{
		TransformComponent* parent_transform = parent->createOrGetComponent<jazzy::TransformComponent>();
		parent_transform->setPosition({ 0.0f, 0.0f, 0.0f });

		/*
		auto child = parent->getChild(1);
		if (child)
		{
			DX3DLogInfo("Child Found");
			TransformComponent* child_transform = child->createOrGetComponent<jazzy::TransformComponent>();
			child_transform->setPosition({ 0.0f, 0.0f, 0.0f });
		}
		*/
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
	//m_inputSystem->setCursorLocked(true);
	//m_inputSystem->setCursorVisible(false);
	m_editorCamera->setDisplayRect(m_display->getClientAreaInScreenSpace());
	m_editorCamera->update(deltaTime);

	// Graphics Engine
	m_graphicsEngine->render(*m_world, m_display->getSwapChain(), deltaTime);
}