#include <iostream>
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

	m_previousTime = std::chrono::steady_clock::now();

	// Plane
	auto plane = m_world->createGameObject<jazzy::GameObject>();
	plane->createOrGetComponent<jazzy::CubeComponent>();
	TransformComponent* plane_transform = plane->createOrGetComponent<jazzy::TransformComponent>();
	plane_transform->setPosition({ 0.0f, -10.0f, 0.0f });
	plane_transform->setScale({ 20.0f, 0.05f, 20.0f });

	// Parent
	test_parent = m_world->createGameObject<jazzy::GameObject>();
	test_parent->createOrGetComponent<jazzy::CubeComponent>();
	TransformComponent* parent_transform = test_parent->createOrGetComponent<jazzy::TransformComponent>();
	parent_transform->setPosition({ 0.0f, 0.0f, 0.0f });

	// Child
	test_child = m_world->createGameObject<jazzy::GameObject>();
	test_child->createOrGetComponent<jazzy::CubeComponent>();
	TransformComponent* child_transform = test_child->createOrGetComponent<jazzy::TransformComponent>();
	child_transform->setPosition({ 2.0f, 0.0f, 0.0f });


	test_child->setParent(test_parent);

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

	// Parent Test
	TransformComponent* parentTransform = test_parent->getComponent<jazzy::TransformComponent>();
	/*
	Vec3 parentVec3 = parentTransform->getScale();
	f32 scaleSpeed = 0.2f;
	parentVec3.x += deltaTime * scaleSpeed;
	parentTransform->setScale(parentVec3);
	*/

	Vec3 parentVec3 = parentTransform->getRotation();
	parentVec3.z += deltaTime;
	parentTransform->setRotation(parentVec3);

	// Child Test
	/*
	TransformComponent* childTransform = test_child->getComponent<jazzy::TransformComponent>();
	Vec3 childVec3 = childTransform->getPosition();
	childVec3.x -= 2.0f * deltaTime;
	childTransform->setPosition(childVec3);
	*/

	// World
	m_world->update(deltaTime);

	// Editor Camera
	m_inputSystem->setCursorLockArea(m_display->getClientAreaInScreenSpace());
	m_inputSystem->setCursorLocked(true);
	m_inputSystem->setCursorVisible(false);
	m_editorCamera->setDisplayRect(m_display->getClientAreaInScreenSpace());
	m_editorCamera->update(deltaTime);

	// Graphics Engine
	m_graphicsEngine->render(*m_world, m_display->getSwapChain(), deltaTime);
}