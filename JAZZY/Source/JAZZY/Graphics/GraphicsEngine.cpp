#include <JAZZY/Graphics/GraphicsEngine.h>
#include <JAZZY/Graphics/RenderSystem.h>
jazzy::GraphicsEngine::GraphicsEngine()
{
	m_renderSystem = std::make_unique< RenderSystem>();
}

jazzy::GraphicsEngine::~GraphicsEngine()
{
}
