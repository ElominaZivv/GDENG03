#include <JAZZY/Graphics/GraphicsEngine.h>
#include <JAZZY/Graphics/RenderSystem.h>
jazzy::GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
	m_renderSystem = std::make_unique< RenderSystem>(RenderSystemDesc{m_logger});
}

jazzy::GraphicsEngine::~GraphicsEngine()
{
}
