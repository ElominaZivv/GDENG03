#pragma once
#include "JAZZY/Graphics/GraphicsResource.h"
#include <JAZZY/Math/Vec4.h>

namespace jazzy
{
	class DeviceContext final: public GraphicsResource
	{
	public:
		explicit DeviceContext(const GraphicsResourceDesc& gDesc);
		void clearAndSetBackBuffer(const SwapChain& swapChain, const Vec4& color);
		void setGraphicsPipelineState(const GraphicsPipelineState& pipeline);
		void setVertexBuffer(const VertexBuffer& buffer);
		void setViewportSize(const Rect& size);
		void drawTriangleList(ui32 vertexCount, ui32 startVertexLocation);

	private:
		Microsoft::WRL::ComPtr <ID3D11DeviceContext > m_context{};

		friend class GraphicsDevice;
	};
}
