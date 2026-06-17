#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/Math/Vec3.h>
#include <JAZZY/Math/Vec4.h>
#include <JAZZY/Math/Triangle.h>
#include <JAZZY/Math/Quad.h>
#include <vector>

#include "JAZZY/Math/Mat4x4.h"

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

	// Debug things
	private:
		ui32 time{};
		ui32 time_prev{};
		ui32 time_curr{};
		ui32 delta_time{};
	private:
		struct alignas(16) ConstantData
		{
			Mat4x4 m_world{};
			Mat4x4 m_view{};
			Mat4x4 m_projection{};
			ui32 m_time;
		};
	private:
		std::shared_ptr<GraphicsDevice> m_graphicsDevice{};
		DeviceContextPtr m_deviceContext{};
		GraphicsPipelineStatePtr m_pipeline{};
		VertexBufferPtr m_vb{};
		ConstantBufferPtr m_cb{};
		IndexBufferPtr m_ib{};

		void updateConstantData(ConstantData& data);
	};
}
