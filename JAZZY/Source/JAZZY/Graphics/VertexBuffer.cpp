#include <JAZZY/Graphics/VertexBuffer.h>

jazzy::VertexBuffer::VertexBuffer(const VertexBufferDesc& desc, const GraphicsResourceDesc& gDesc):
	GraphicsResource(gDesc), m_vertexSize(desc.vertexSize), m_vertexListSize(desc.vertexListSize)
{
	if (!desc.vertexList) DX3DLogThrowInvalidArg("No vertex list provided.");
	if (!desc.vertexListSize) DX3DLogThrowInvalidArg("Vertex list size must be non-zero.");
	if (!desc.vertexSize) DX3DLogThrowInvalidArg("Vertex size must be non-zero");

	D3D11_BUFFER_DESC buffDesc{};
	buffDesc.ByteWidth = desc.vertexListSize * desc.vertexSize;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = desc.vertexList;

	DX3DGraphicsLogThrowOnFail
	(
		m_device.CreateBuffer
		(
			&buffDesc, // Allocate memory in GPU for the buffer
			&initData,
			&m_buffer
		),
		"CreateBuffer failed."
	);
}

jazzy::ui32 jazzy::VertexBuffer::getVertexListSize() const noexcept
{
	return m_vertexListSize;
}
