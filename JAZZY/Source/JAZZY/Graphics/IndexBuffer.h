#pragma once
#include <JAZZY/Graphics/GraphicsResource.h>

namespace jazzy
{
	class IndexBuffer final : public GraphicsResource
	{
	public:
		IndexBuffer(const IndexBufferDesc& desc, const GraphicsResourceDesc& gDesc);
		ui32 getIndexListSize() const noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_buffer{};
		ui32 m_indexListSize{};

		friend class DeviceContext;
	};
}
