#include <JAZZY/Graphics/GraphicsEngine.h>
#include <JAZZY/Graphics/GraphicsDevice.h>
#include <JAZZY/Graphics/DeviceContext.h>
#include <JAZZY/Graphics/SwapChain.h>
using namespace jazzy;

jazzy::GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
	m_graphicsDevice = std::make_shared< GraphicsDevice>(GraphicsDeviceDesc{m_logger});

	auto& device = *m_graphicsDevice;
	m_deviceContext = device.createDeviceContext();


	constexpr char shaderSourceCode[] =
		R"(
void VSMain()
{
}
void PSMain()
{
}
)";
	constexpr char shaderSourceName[] = "Basic";
	constexpr char shaderSourceCodeSize = std::size(shaderSourceCode);

	auto vs = device.compileShader({shaderSourceName, shaderSourceCode, shaderSourceCodeSize, "VSMain", ShaderType::VertexShader});
	auto ps = device.compileShader({shaderSourceName, shaderSourceCode, shaderSourceCodeSize, "PSMain", ShaderType::PixelShader});
	
	m_pipeline = device.createGraphicsPipelineState({ *vs, *ps });
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
	context.clearAndSetBackBuffer(swapChain, { 1, 0, 0, 1 });
	context.setGraphicsPipelineState(*m_pipeline);

	auto& device = *m_graphicsDevice;
	device.executeCommandList(context);
	swapChain.present();

}
