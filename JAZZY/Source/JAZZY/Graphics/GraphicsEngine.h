#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/Math/Vec3.h>
#include <JAZZY/Math/Vec4.h>
namespace jazzy
{
	// Final means no class can derive from GraphicsEngine class
	class GraphicsEngine final: public Base
	{
	public:
		GraphicsEngine(const GraphicsEngineDesc& desc);
		virtual ~GraphicsEngine() override;


		GraphicsDevice& getGraphicsDevice() noexcept;

		void render(SwapChain& swapChain);
	private:
		struct Vertex
		{
			Vec3 position;
			Vec4 color;
		};
	private:
		std::shared_ptr<GraphicsDevice> m_graphicsDevice{};
		DeviceContextPtr m_deviceContext{};
		GraphicsPipelineStatePtr m_pipeline{};
		VertexBufferPtr m_vb{};
		ConstantBufferPtr m_cb{};
	};
}
