#include <JAZZY/Graphics/IndexBuffer.h>

jazzy::IndexBuffer::IndexBuffer(const IndexBufferDesc& desc, const GraphicsResourceDesc& gDesc) :
	GraphicsResource(gDesc), m_indexListSize(desc.indexListSize)
{
	if (!desc.indexList) DX3DLogThrowInvalidArg("No index list provided.");
	if (!desc.indexListSize) DX3DLogThrowInvalidArg("Index list size must be non-zero.");

	D3D11_BUFFER_DESC buffDesc{};
	buffDesc.ByteWidth = sizeof(ui32) * desc.indexListSize;	// Integer is 4 bytes
	buffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = desc.indexList;

	DX3DGraphicsLogThrowOnFail
	(
		m_device.CreateBuffer
		(
			&buffDesc, // Allocate memory in GPU for the buffer
			&initData,
			&m_buffer
		),
		"CreateBuffer Index Buffer failed."
	);
}

jazzy::ui32 jazzy::IndexBuffer::getIndexListSize() const noexcept
{
	return m_indexListSize;
}
