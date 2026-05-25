#pragma once
#include <JAZZY/Graphics/GraphicsResource.h>

namespace jazzy
{
	class VertexBuffer final: public GraphicsResource
	{
	public:
		VertexBuffer(const VertexBufferDesc& desc, const GraphicsResourceDesc& gDesc);
		ui32 getVertexListSize() const noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_buffer{};
		ui32 m_vertexSize{};
		ui32 m_vertexListSize{};

		friend class DeviceContext;
	};
}
