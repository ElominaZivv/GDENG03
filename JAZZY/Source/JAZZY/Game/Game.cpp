#include <JAZZY/Game/Game.h>
#include <JAZZY/Window/Window.h>
#include <JAZZY/Graphics/GraphicsDevice.h>
#include <JAZZY/Core/Logger.h>
#include <JAZZY/Input/InputSystem.h>
#include <JAZZY/Game/Display.h>
#include <JAZZY/Game/World.h>
#include <JAZZY/Game/GameObject.h>
#include <JAZZY/Game/WorldRenderer.h>
#include <JAZZY/Resource/ResourceManager.h>

#include <iostream>
#include <ranges>

#include "JAZZY/Components/CubeComponent.h"
#include "JAZZY/EditorCamera/EditorCamera.h"
#include "JAZZY/Game/GameObject.h"

jazzy::Game::Game(const GameDesc& desc)
{
	m_logger = std::make_unique<Logger>(desc.logLevel);
	m_inputSystem = std::make_unique<InputSystem>(InputSystemDesc{ *m_logger });
	m_graphicsDevice = std::make_shared<GraphicsDevice>(GraphicsDeviceDesc{ *m_logger });
	m_display = std::make_unique<Display>(DisplayDesc{ {*m_logger,desc.windowSize},*m_graphicsDevice });
	auto context = SystemContext{ *m_graphicsDevice };
	m_resourceManager = std::make_unique<ResourceManager>(ResourceManagerDesc{ {*m_logger}, context });
	m_world = std::make_unique<World>(WorldDesc{ BaseDesc{*m_logger}, GameContext{*m_inputSystem, *m_resourceManager, *m_graphicsDevice} });
	m_worldRenderer = std::make_unique<WorldRenderer>(WorldRendererDesc{ {*m_logger},*m_graphicsDevice });

	m_inputSystem->setCursorLockArea(m_display->getClientAreaInScreenSpace());

	// My Free-cam
	m_editorCamera = std::make_shared<EditorCamera>(EditorCameraDesc{ *m_logger , *m_inputSystem });
	
	// FIX ME! m_graphicsEngine->initializeUI(m_display->getHWND(),*m_world);

	/*
	// Plane
	auto plane = m_world->createGameObject<jazzy::GameObject>("plane");
	plane->createOrGetComponent<jazzy::CubeComponent>();
	TransformComponent* plane_transform = plane->createOrGetComponent<jazzy::TransformComponent>();
	plane_transform->setPosition({ 0.0f, -10.0f, 0.0f });
	plane_transform->setScale({ 20.0f, 0.05f, 20.0f });

	// Parent
	test_parent = m_world->createGameObject<jazzy::GameObject>("parent");
	test_parent->createOrGetComponent<jazzy::CubeComponent>();
	TransformComponent* parent_transform = test_parent->createOrGetComponent<jazzy::TransformComponent>();
	parent_transform->setPosition({ 0.0f, 0.0f, 0.0f });

	// Child
	test_child = m_world->createGameObject<jazzy::GameObject>("child");
	test_child->createOrGetComponent<jazzy::CubeComponent>();
	TransformComponent* child_transform = test_child->createOrGetComponent<jazzy::TransformComponent>();
	child_transform->setPosition({ 2.0f, 0.0f, 0.0f });


	test_child->setParent(test_parent);
	test_parent->setParent(plane);
	*/

	// How does Pardcode call this function when class Game does not inherit class Base?
	DX3DLogInfo("Game initialized.");
}

jazzy::Game::~Game()
{
	// How does Pardcode call this function when class Game does not inherit class Base?
	DX3DLogInfo("Game is shutting down.");
}

jazzy::World& jazzy::Game::getWorld() noexcept
{
	return *m_world;
}

jazzy::Logger& jazzy::Game::getLogger() const noexcept
{
	return *m_logger;
}

jazzy::InputSystem& jazzy::Game::getInputSystem() noexcept
{
	return *m_inputSystem;
}

jazzy::ResourceManager& jazzy::Game::getResourceManager() noexcept
{
	return *m_resourceManager;
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
	onUpdate(deltaTime);
	m_world->update(deltaTime);

	// Editor Camera
	m_inputSystem->setCursorLockArea(m_display->getClientAreaInScreenSpace());
	m_editorCamera->setDisplayRect(m_display->getClientAreaInScreenSpace());
	m_editorCamera->update(deltaTime);

	// World Renderer
	m_worldRenderer->render(*m_world, m_display->getSwapChain(), deltaTime);

	// Graphics Engine
	// m_graphicsEngine->render(*m_world, m_display->getSwapChain(), deltaTime);
}
