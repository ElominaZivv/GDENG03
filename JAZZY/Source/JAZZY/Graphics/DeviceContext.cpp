#include <JAZZY/Graphics/DeviceContext.h>
#include <JAZZY/Graphics/SwapChain.h>
#include <JAZZY/Graphics/GraphicsPipelineState.h>
#include <JAZZY/Graphics/VertexBuffer.h>
jazzy::DeviceContext::DeviceContext(const GraphicsResourceDesc& gDesc): GraphicsResource(gDesc)
{
	DX3DGraphicsLogThrowOnFail
	(
		m_device.CreateDeferredContext(0, &m_context),
		"CreateDeferredContext failed."
	);
}

void jazzy::DeviceContext::clearAndSetBackBuffer(const SwapChain& swapChain, const Vec4& color)
{
	f32 fColor[] = { color.x, color.y, color.z, color.w };
	auto rtv = swapChain.m_rtv.Get();
	m_context->ClearRenderTargetView(rtv, fColor);
	m_context->OMSetRenderTargets(1, &rtv, nullptr);
}

void jazzy::DeviceContext::setGraphicsPipelineState(const GraphicsPipelineState& pipeline)
{
	m_context->IASetInputLayout(pipeline.m_layout.Get());
	m_context->VSSetShader(pipeline.m_vs.Get(), nullptr, 0);
	m_context->PSSetShader(pipeline.m_ps.Get(), nullptr, 0);
}

void jazzy::DeviceContext::setVertexBuffer(const VertexBuffer& buffer)
{
	auto stride = buffer.m_vertexSize;
	auto buf = buffer.m_buffer.Get();
	auto offset = 0u;	// 'u' indicates it is an unsigned integer
	m_context->IASetVertexBuffers
	(
		0,	// Starting point of the buffer. There can be multiple buffers with different starting points
		1,	// The number of buffers
		&buf,
		&stride,	// stride represents the length of a single vertex in bytes
		&offset
	);
}

void jazzy::DeviceContext::setViewportSize(const Rect& size)
{
	D3D11_VIEWPORT vp{};
	vp.Width = static_cast<f32>(size.width);
	vp.Height = static_cast<f32>(size.height);
	vp.MinDepth = 0.0f;		// Near Clipping Plane
	vp.MaxDepth = 1.0f;		// Far Clipping Plane

	m_context->RSSetViewports
	(
		1,		// Number of viewports
		&vp
	);
}

void jazzy::DeviceContext::drawTriangleList(ui32 vertexCount, ui32 startVertexLocation)
{
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// Tells the GPU how to interpret the vertex data
	m_context->Draw(vertexCount, startVertexLocation);	// Executes Graphics Pipeline

}
