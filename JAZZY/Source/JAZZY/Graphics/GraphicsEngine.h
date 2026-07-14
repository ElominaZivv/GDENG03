#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/Math/Vec3.h>
#include <JAZZY/Math/Vec4.h>
#include <JAZZY/Math/Mat4x4.h>
#include <vector>

namespace jazzy
{
	// Final means no class can derive from GraphicsEngine class
	class GraphicsEngine final: public Base
	{
	public:
		GraphicsEngine(const GraphicsEngineDesc& desc);
		virtual ~GraphicsEngine() override;

		GraphicsDevice& getGraphicsDevice() noexcept;

		void render(World& world, SwapChain& swapChain, f32 deltaTime);
	private:
		struct alignas(16) ConstantData
		{
			Mat4x4 m_world{};
			Mat4x4 m_view{};
			Mat4x4 m_projection{};
			f32 m_time;
		};
	private:
		std::shared_ptr<GraphicsDevice> m_graphicsDevice{};
		RefPtr<DeviceContext> m_deviceContext{};
		RefPtr<GraphicsPipelineState> m_pipeline{};
		RefPtr<VertexBuffer> m_vb{};
		RefPtr<ConstantBuffer> m_cb{};
		RefPtr<IndexBuffer> m_ib{};

		// TEMPORARY DEPENDENCY FOR DEBUGGING
		f32 m_time{0.0f};

		// EditorCamera
		RefPtr<EditorCamera> m_editorCamera{};

	};
}
