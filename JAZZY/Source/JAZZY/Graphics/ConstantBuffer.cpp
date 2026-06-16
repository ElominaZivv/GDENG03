#include <JAZZY/Graphics/ConstantBuffer.h>

jazzy::ConstantBuffer::ConstantBuffer(const ConstantBufferDesc& desc, const GraphicsResourceDesc& gDesc): GraphicsResource(gDesc), m_size(desc.bufferSize)
{
	D3D11_BUFFER_DESC buffDesc{};
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.ByteWidth = desc.bufferSize;
	buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = desc.buffer;

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
