#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
namespace jazzy
{
	// Final means no class can derive from GraphicsEngine class
	class GraphicsEngine final: public Base
	{
	public:
		GraphicsEngine(const GraphicsEngineDesc& desc);
		virtual ~GraphicsEngine() override;
	private:
		std::unique_ptr<RenderSystem> m_renderSystem{};
	};
}
