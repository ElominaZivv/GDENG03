#include <JAZZY/Graphics/GraphicsEngine.h>
#include <JAZZY/Graphics/GraphicsDevice.h>
#include <JAZZY/Graphics/DeviceContext.h>
#include <JAZZY/Graphics/SwapChain.h>
#include <JAZZY/Graphics/VertexBuffer.h>	
#include <JAZZY/Math/Vec3.h>
using namespace jazzy;

jazzy::GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
	m_graphicsDevice = std::make_shared< GraphicsDevice>(GraphicsDeviceDesc{m_logger});

	auto& device = *m_graphicsDevice;
	m_deviceContext = device.createDeviceContext();


	constexpr char shaderSourceCode[] =
		R"(
float4 VSMain(float3 pos: POSITION): SV_Position
{
return float4(pos.xyz, 1.0);
}
float4 PSMain(): SV_Target
{
return float4(0.086, 0.85, 0.529, 0.125);
}
)";
	constexpr char shaderSourceName[] = "Basic";
	constexpr auto shaderSourceCodeSize = std::size(shaderSourceCode);

	auto vs = device.compileShader({shaderSourceName, shaderSourceCode, shaderSourceCodeSize, "VSMain", ShaderType::VertexShader});
	auto ps = device.compileShader({shaderSourceName, shaderSourceCode, shaderSourceCodeSize, "PSMain", ShaderType::PixelShader});
	
	m_pipeline = device.createGraphicsPipelineState({ *vs, *ps });

	const Vec3 vertexList[] =
	{
		{-0.25f, -0.25f, 0.0f},
		{0.0f, 0.5f, 0.0f},
		{0.25f, -0.25f, 0.0f}
	};
	m_vb = device.createVertexBuffer({vertexList, std::size(vertexList), sizeof(Vec3)});
}

jazzy::GraphicsEngine::~GraphicsEngine()
{
}

jazzy::GraphicsDevice& jazzy::GraphicsEngine::getGraphicsDevice() noexcept
{
	return *m_graphicsDevice;
}

void GraphicsEngine::render(SwapChain& swapChain)
{
	auto& context = *m_deviceContext;
	context.clearAndSetBackBuffer(swapChain, { 0.0549, 0.07, 0.109, 1 });
	context.setGraphicsPipelineState(*m_pipeline);

	context.setViewportSize(swapChain.getSize());

	auto& vb = *m_vb;
	context.setVertexBuffer(vb);
	context.drawTriangleList(vb.getVertexListSize(), 0u);

	auto& device = *m_graphicsDevice;
	device.executeCommandList(context);
	swapChain.present();

}
