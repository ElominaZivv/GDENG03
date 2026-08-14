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
#include <algorithm>
#include <ranges>

#include "JAZZY/Components/CubeComponent.h"
#include "JAZZY/Components/CapsuleComponent.h"
#include "JAZZY/Components/CylinderComponent.h"
#include "JAZZY/Components/MeshComponent.h"
#include "JAZZY/Components/PlaneComponent.h"
#include "JAZZY/Components/RigidBodyComponent.h"
#include "JAZZY/Components/SphereComponent.h"
#include "JAZZY/EditorCamera/EditorCamera.h"
#include "JAZZY/Game/GameObject.h"
#include "JAZZY/Game/WorldPhysics.h"
#include "JAZZY/UI/UIManager.h"
#include <JAZZY/SaveSystem/SceneSerializer.h>

jazzy::Game::Game(const GameDesc& desc)
{
	m_previousTime = std::chrono::steady_clock::now();

	m_logger = std::make_unique<Logger>(desc.logLevel);
	m_inputSystem = std::make_unique<InputSystem>(InputSystemDesc{ *m_logger });
	m_graphicsDevice = std::make_shared<GraphicsDevice>(GraphicsDeviceDesc{ *m_logger });
	m_display = std::make_unique<Display>(DisplayDesc{ {*m_logger,desc.windowSize},*m_graphicsDevice });
	auto context = SystemContext{ *m_graphicsDevice };
	m_resourceManager = std::make_unique<ResourceManager>(ResourceManagerDesc{ {*m_logger}, context });
	
	m_worldPhysics = std::make_unique<WorldPhysics>(WorldPhysicsDesc{ BaseDesc{ *m_logger } });
	m_sceneSerializer = std::make_unique<SceneSerializer>(SceneSerializerDesc{ BaseDesc{*m_logger}, std::filesystem::path("Game") / "Assets" / "Levels" });
	m_world = std::make_unique<World>(WorldDesc{ BaseDesc{*m_logger}, GameContext{*m_inputSystem, *m_resourceManager, *m_graphicsDevice}, *m_worldPhysics, *m_sceneSerializer });
	m_uiManager = std::make_unique<UIManager>(UIManagerDesc{ *m_display, *m_graphicsDevice, *m_world, *m_resourceManager });
	m_logger->SetDebugConsole(m_uiManager->GetDebugConsoleScreen());

	m_worldRenderer = std::make_unique<WorldRenderer>(WorldRendererDesc{ {*m_logger},*m_graphicsDevice });

	m_inputSystem->setCursorLockArea(m_display->getClientAreaInScreenSpace());

	// My Free-cam
	m_editorCamera = std::make_shared<EditorCamera>(EditorCameraDesc{ *m_logger , *m_inputSystem });

	// SCENE ROOT NODE DO NOT REMOVE
		auto rootScene = m_world->createGameObject<jazzy::GameObject>("Scene");
		rootScene->_id = m_world->ROOTSCENE_ID;
		auto rootTransform = rootScene->createOrGetComponent<jazzy::TransformComponent>();
		rootTransform->setPosition({ 0.0f, 0.0f, 0.0f });
	// End of dont remove

	// Create Materials
	auto woodTexture = getResourceManager().createResourceFromFile<jazzy::TextureResource>(L"./Game/Assets/Textures/wood.jpg");
	auto stoneTexture = getResourceManager().createResourceFromFile<jazzy::TextureResource>(L"./Game/Assets/Textures/stone.jpg");
	auto woodMat = getResourceManager().createResourceFromFile<jazzy::MaterialResource>(L"./Game/Assets/Shaders/BasicShader.hlsl");
	auto stoneMat = getResourceManager().createResourceFromFile<jazzy::MaterialResource>(L"./Game/Assets/Shaders/BasicShader.hlsl");
	if (woodMat)
	{
		woodMat->setTexture(0, woodTexture);
	}
	if (stoneMat)
	{
		stoneMat->setTexture(0, stoneTexture);
	}

	// Plane
	/*
	m_world->AddGameSceneObject("Floor", { World::COMP_Plane, World::COMP_RigidBody }, { 0.0f, -5.0f, 0.0f });
	auto plane = m_world->getGameObjectByName("Floor");
	auto planeComp = plane->getComponent<jazzy::PlaneComponent>();
	auto planeRigidComp = plane->getComponent<jazzy::RigidBodyComponent>();
	planeRigidComp->setBodyType(reactphysics3d::BodyType::STATIC);
	planeRigidComp->addBoxCollider({ 12.5f, 0.01f, 12.5f }, { 0.0f, 0.0f,0.0f });
	planeComp->setMaterial(stoneMat);
	plane->getTransform().setPosition({ 0.0f, -3.0f, 0.0f });
	plane->getTransform().setScale({ 25.0f, 1.0f, 25.0f });

	// 100 Physics Cubes
	for (auto i : std::views::iota(0u, 10u))
	{
		for (auto j : std::views::iota(0u, 10u))
		{
			auto id = m_world->AddGameSceneObject("Cube", { World::COMP_Cube, World::COMP_RigidBody }, { 0.0f, 0.0f, 0.0f });
			auto cube = m_world->getGameObjectByID(id);
			auto cubeComp = cube->getComponent<jazzy::CubeComponent>();
			auto cubeRb = cube->getComponent<jazzy::RigidBodyComponent>();
			cubeRb->setBodyType(reactphysics3d::BodyType::DYNAMIC);
			cubeRb->addBoxCollider({ 0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f,0.0f });
			cubeComp->setMaterial(woodMat);
			cube->getTransform().setPosition({ f32(-10.0f + (i * 2)), f32(3.0f + (i+j)), f32(-10.0f + (j * 2)) });
		}
	}
	*/

	DX3DLogInfo("Game initialized.");
}

jazzy::Game::~Game()
{
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
	auto deltaTime = std::min(delta.count(), 1.0f / 50.0f); // Set up safety clamp so it won't explode at first

	// Input System
	m_inputSystem->update();
	if (m_inputSystem->isKeyPressed(KeyCode::Escape)) m_isRunning = false;	// Close the program

	// World
	onUpdate(deltaTime);
	m_world->update(deltaTime);

	// World Physics
	m_worldPhysics->update(deltaTime);
	m_world->syncPhysicsComponents();

	// Editor Camera
	m_inputSystem->setCursorLockArea(m_display->getClientAreaInScreenSpace());
	m_editorCamera->setDisplayRect(m_display->getClientAreaInScreenSpace());
	m_editorCamera->update(deltaTime);

	// World Renderer
	m_worldRenderer->render(*m_world, m_display->getSwapChain(), *m_editorCamera, *m_uiManager, deltaTime);
}
