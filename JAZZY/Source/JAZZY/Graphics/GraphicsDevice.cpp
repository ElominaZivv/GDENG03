#include <JAZZY/Graphics/GraphicsDevice.h>
#include <JAZZY/Graphics/GraphicsLogUtils.h>
#include <JAZZY/Graphics/SwapChain.h>
#include <JAZZY/Graphics/DeviceContext.h>
#include <JAZZY/Graphics/ShaderBinary.h>
#include <JAZZY/Graphics/GraphicsPipelineState.h>
#include <JAZZY/Graphics/VertexBuffer.h>
#include <JAZZY/Graphics/VertexShaderSignature.h>
#include <JAZZY/Graphics/ConstantBuffer.h>
#include <JAZZY/Graphics/IndexBuffer.h>

using namespace jazzy;

jazzy::GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc& desc) : Base(desc.base)
{
	D3D_FEATURE_LEVEL featureLevel{};
	UINT createDeviceFlags{};

#ifndef _DEBUG	// Helps get error and warning messages during runtime
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DX3DGraphicsLogThrowOnFail
	(
		D3D11CreateDevice
		(
			NULL,						// XGI Adaptor represents graphics card GPU installed in the system
			D3D_DRIVER_TYPE_HARDWARE,	// if XGI adaptor is null, and this is hardware, it will pick the primary GPU
			NULL,						// Handle to a dll a custom software rastorizer but we arent using in this case
			createDeviceFlags,
			NULL,						// A pointer to an array of feature level, if NULL, the system will pick the best one
			0,							// Size of the previous array, since NULL, 0
			D3D11_SDK_VERSION,			// Microsoft safety check to prevent mismatch of versions and to make sure the versions match the direct3d runtime  

			// OUTPUT STUFF BELOW
			&m_d3dDevice,				// Expects a double pointer, this is where all the resources like buffer are
			&featureLevel,
			&m_d3dContext				// Expects a double pointer, this is where all the resources like buffer are
		),
		"Direct3D initialization failed."
	);

	DX3DGraphicsLogThrowOnFail
	(
		m_d3dDevice->QueryInterface(IID_PPV_ARGS(&m_dxgiDevice)),
		"QueryInterface failed to retrieve IDXDGIDevice"
	);

	DX3DGraphicsLogThrowOnFail
	(
		m_dxgiDevice->GetParent(IID_PPV_ARGS(&m_dxgiAdapter)),
		"GetParent failed to retrieve IDXDGIAdapter"
	);

	DX3DGraphicsLogThrowOnFail
	(
		m_dxgiAdapter->GetParent(IID_PPV_ARGS(&m_dxgiFactory)),
		"GetParent failed to retrieve IDXDGIFactory"
	);
}

jazzy::GraphicsDevice::~GraphicsDevice()
{
}

jazzy::RefPtr<SwapChain> jazzy::GraphicsDevice::createSwapChain(const SwapChainDesc& desc) const
{
	return std::make_shared<SwapChain>(desc, getGraphicsResourceDesc());
}

RefPtr<DeviceContext> GraphicsDevice::createDeviceContext()
{
	return std::make_shared<DeviceContext>(getGraphicsResourceDesc());
}

RefPtr<ShaderBinary> GraphicsDevice::compileShader(const ShaderCompileDesc& desc)
{
	return std::make_shared<ShaderBinary>(desc, getGraphicsResourceDesc());
}

RefPtr<GraphicsPipelineState> GraphicsDevice::createGraphicsPipelineState(const GraphicsPipelineStateDesc& desc)
{
	return std::make_shared<GraphicsPipelineState>(desc, getGraphicsResourceDesc());
}

RefPtr<VertexBuffer> jazzy::GraphicsDevice::createVertexBuffer(const VertexBufferDesc& desc)
{
	return std::make_shared<VertexBuffer>(desc, getGraphicsResourceDesc());
}

RefPtr<VertexShaderSignature> GraphicsDevice::createVertexShaderSignature(const VertexShaderSignatureDesc& desc)
{
	return std::make_shared<VertexShaderSignature>(desc, getGraphicsResourceDesc());
}

RefPtr<ConstantBuffer> GraphicsDevice::createConstantBuffer(const ConstantBufferDesc& desc)
{
	return std::make_shared<ConstantBuffer>(desc, getGraphicsResourceDesc());
}

RefPtr<IndexBuffer> GraphicsDevice::createIndexBuffer(const IndexBufferDesc& desc)
{
	return std::make_shared<IndexBuffer>(desc, getGraphicsResourceDesc());
}

void GraphicsDevice::executeCommandList(DeviceContext& context)
{
	Microsoft::WRL::ComPtr<ID3D11CommandList> list{};
	DX3DGraphicsLogThrowOnFail
	(
		context.m_context->FinishCommandList(false, &list),
		"FinishedCommandList failed."
	);
	m_d3dContext->ExecuteCommandList(list.Get(), false);
}

GraphicsResourceDesc jazzy::GraphicsDevice::getGraphicsResourceDesc() const noexcept
{
	return { {m_logger}, shared_from_this(),  *m_d3dDevice.Get(), *m_dxgiFactory.Get() };
}
