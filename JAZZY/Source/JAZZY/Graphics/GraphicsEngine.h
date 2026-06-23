#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/Math/Vec3.h>
#include <JAZZY/Math/Vec4.h>
#include <JAZZY/Math/Vertex.h>
#include <vector>

#include <JAZZY/Ball.h>
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

		void render(f32 deltaTime, SwapChain& swapChain);

		std::vector<Ball>* getBallObjects();
	private:
		struct alignas(16) ConstantData
		{
			Mat4x4 m_world{};
			Mat4x4 m_view{};
			Mat4x4 m_projection{};
			f32 m_time;
		};
	private:
		std::vector<Ball> balls;

		void updateConstantData(ConstantData& data, Ball ball);
		void generatePolygonVerticesAndIndexLists(f32 radius, ui32 num_vertices);
		std::vector<Vertex> polygonVertexList;
		std::vector<ui32> polygonIndexList;
	private:
		std::shared_ptr<GraphicsDevice> m_graphicsDevice{};
		DeviceContextPtr m_deviceContext{};
		GraphicsPipelineStatePtr m_pipeline{};
		VertexBufferPtr m_vb{};
		ConstantBufferPtr m_cb{};
		IndexBufferPtr m_ib{};
		// TEMPORARY DEPENDENCY FOR DEBUGGING
		f32 m_time{0.0f};
		InputSystemPtr m_inputSystem{};
		f32 rotx{};
		f32 roty{};
		f32 rotz{};
		bool cursorVisibleToggle{ true };
		bool cursorLockToggle{ true };

	};
}
