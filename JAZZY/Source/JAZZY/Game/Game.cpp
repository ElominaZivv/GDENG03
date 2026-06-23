#include <random>
#include <JAZZY/Game/Game.h>
#include <JAZZY/Window/Window.h>
#include <JAZZY/Graphics/GraphicsEngine.h>
#include <JAZZY/Core/Logger.h>
#include<JAZZY/Game/Display.h>
#include <JAZZY/Input/InputSystem.h>

jazzy::Game::Game(const GameDesc& desc):
	Base({*std::make_unique<Logger>(desc.logLevel).release()}),
	m_LoggerPtr(&m_logger)
{
	m_inputSystem = std::make_shared<InputSystem>(InputSystemDesc{ m_logger });
	m_inputSystem->setCursorLockArea(desc.windowSize);
	m_graphicsEngine = std::make_unique<GraphicsEngine>(GraphicsEngineDesc{m_logger, m_inputSystem});
	m_display = std::make_unique<Display>(DisplayDesc{ {m_logger, desc.windowSize}, m_graphicsEngine->getGraphicsDevice() });

	m_previousTime = std::chrono::steady_clock::now();
	using namespace std::chrono_literals;
	m_timestep = 16ms;

	DX3DLogInfo("Game initialized.");
}

jazzy::Game::~Game()
{
	DX3DLogInfo("Game is shutting down.");
}

void jazzy::Game::onInternalUpdate()
{
	// Get Current Time
	auto currentTime = std::chrono::steady_clock::now();
	// Get Duration
	auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (currentTime - m_previousTime);
	// Get DeltaTime in f32
	std::chrono::duration<f32> delta = dur;
	auto deltaTime = delta.count();
	// Set Previous Time to Current Time
	m_previousTime = currentTime;

	m_inputSystem->update();
	if (m_inputSystem->isKeyDown(KeyCode::Escape))
	{
		DX3DLogInfo("Escape Pressed");
		m_isRunning = false;
	}
	if (m_inputSystem->isKeyPressed(KeyCode::Space))
	{
		DX3DLogInfo("Space Pressed");
		// Randomizer
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<f32>dis(-1.0f, 1.0f);

		Ball newBall({ 0.0f, 0.0f, 0.0f }, { dis(gen), dis(gen), 0.0f }, 0.25f);
		std::vector<Ball>* balls = m_graphicsEngine->getBallObjects();
		balls->push_back(newBall);
	}
	if (m_inputSystem->isKeyPressed(KeyCode::Backspace))
	{
		DX3DLogInfo("Backspace Pressed");
		std::vector<Ball>* balls = m_graphicsEngine->getBallObjects();
		if (balls->size() > 0) balls->pop_back();
	}
	if (m_inputSystem->isKeyPressed(KeyCode::Delete))
	{
		DX3DLogInfo("Delete Pressed");
		std::vector<Ball>* balls = m_graphicsEngine->getBallObjects();
		if (balls->size() > 0) balls->clear();
	}

	// Increment Duration to the current Nanoseconds
	m_currNanoSeconds += dur;
	m_fixedDeltaTime += deltaTime;
	if (m_currNanoSeconds >= m_timestep)
	{
		// Render
		m_graphicsEngine->render(m_fixedDeltaTime, m_display->getSwapChain());
		// Reset current nanoseconds to 0
		m_currNanoSeconds -= m_currNanoSeconds;
		m_fixedDeltaTime = 0.0f;
	}
}