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
#include "JAZZY/Components/MeshComponent.h"
#include "JAZZY/Components/PlaneComponent.h"
#include "JAZZY/Components/SphereComponent.h"
#include "JAZZY/EditorCamera/EditorCamera.h"
#include "JAZZY/Game/GameObject.h"
#include "JAZZY/UI/UIManager.h"

jazzy::Game::Game(const GameDesc& desc)
{
	m_logger = std::make_unique<Logger>(desc.logLevel);
	m_inputSystem = std::make_unique<InputSystem>(InputSystemDesc{ *m_logger });
	m_graphicsDevice = std::make_shared<GraphicsDevice>(GraphicsDeviceDesc{ *m_logger });
	m_display = std::make_unique<Display>(DisplayDesc{ {*m_logger,desc.windowSize},*m_graphicsDevice });
	auto context = SystemContext{ *m_graphicsDevice };
	m_resourceManager = std::make_unique<ResourceManager>(ResourceManagerDesc{ {*m_logger}, context });
	m_world = std::make_unique<World>(WorldDesc{ BaseDesc{*m_logger}, GameContext{*m_inputSystem, *m_resourceManager, *m_graphicsDevice} });
	m_uiManager = std::make_unique<UIManager>(UIManagerDesc{*m_display, *m_graphicsDevice, *m_world});
	m_worldRenderer = std::make_unique<WorldRenderer>(WorldRendererDesc{ {*m_logger},*m_graphicsDevice });

	m_inputSystem->setCursorLockArea(m_display->getClientAreaInScreenSpace());

	// My Free-cam
	m_editorCamera = std::make_shared<EditorCamera>(EditorCameraDesc{ *m_logger , *m_inputSystem });

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

	// Meshes
	auto marbleBustTex = getResourceManager().createResourceFromFile<jazzy::TextureResource>(L"Game/Assets/Textures/marble_bust_01_diff_1k.jpg");
	auto marbleBustMesh = getResourceManager().createResourceFromFile<jazzy::MeshResource>(L"./Game/Assets/Meshes/marble_bust_01.obj");
	auto marbleBustMat = getResourceManager().createResourceFromFile<jazzy::MaterialResource>(L"./Game/Assets/Shaders/BasicShader.hlsl");
	if (marbleBustMat)
	{
		marbleBustMat->setTexture(0, marbleBustTex);
	}

	// Marble Bust
	auto mesh = m_world->createGameObject<jazzy::GameObject>();
	auto bustComp = mesh->createOrGetComponent<jazzy::MeshComponent>();
	bustComp->setMesh(marbleBustMesh);
	bustComp->setMaterial(0, marbleBustMat);
	mesh->getTransform().setScale({ 4, 4, 4 });
	mesh->getTransform().setPosition({ 0, -1, 2 });

	// Plane
	auto plane = m_world->createGameObject<jazzy::GameObject>("plane");
	auto comp = plane->createOrGetComponent<jazzy::PlaneComponent>();
	comp->setMaterial(stoneMat);
	TransformComponent* plane_transform = plane->createOrGetComponent<jazzy::TransformComponent>();
	plane_transform->setPosition({ 0.0f, -5.0f, 0.0f });
	plane_transform->setScale({ 25.0f, 25.0f, 25.0f });

	// Parent
	test_parent = m_world->createGameObject<jazzy::GameObject>("parent");
	auto parent_comp = test_parent->createOrGetComponent<jazzy::CubeComponent>();
	parent_comp->setMaterial(woodMat);
	
	TransformComponent* parent_transform = test_parent->createOrGetComponent<jazzy::TransformComponent>();
	parent_transform->setPosition({ 2.0f, 0.0f, 4.0f });

	// Child
	test_child = m_world->createGameObject<jazzy::GameObject>("child");
	auto child_comp = test_child->createOrGetComponent<jazzy::CubeComponent>();
	child_comp->setMaterial(woodMat);
	TransformComponent* child_transform = test_child->createOrGetComponent<jazzy::TransformComponent>();
	child_transform->setPosition({ 0.0f, 0.0f, 4.0f });


	test_child->setParent(test_parent);
	test_parent->setParent(plane);

	// Sphere
	auto sphere = m_world->createGameObject<jazzy::GameObject>("sphere");
	auto sphereComp = sphere->createOrGetComponent<jazzy::SphereComponent>();
	sphereComp->setMaterial(stoneMat);
	sphere->getTransform().setPosition({ -2.0f, -2.0f, 2.0f });
	sphere->getTransform().setScale({ 2.0f, 2.0f, 2.0f });

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
	m_worldRenderer->render(*m_world, m_display->getSwapChain(), *m_editorCamera, *m_uiManager, deltaTime);
}
