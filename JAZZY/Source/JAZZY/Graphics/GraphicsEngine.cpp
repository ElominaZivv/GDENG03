#include <JAZZY/Graphics/GraphicsEngine.h>
#include <JAZZY/Graphics/GraphicsDevice.h>
#include <JAZZY/Graphics/DeviceContext.h>
#include <JAZZY/Graphics/SwapChain.h>
#include <JAZZY/Graphics/VertexBuffer.h>	
#include <JAZZY/Math/Vec3.h>
#include <fstream>
using namespace jazzy;

jazzy::GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
	m_graphicsDevice = std::make_shared< GraphicsDevice>(GraphicsDeviceDesc{m_logger});

	auto& device = *m_graphicsDevice;
	m_deviceContext = device.createDeviceContext();

	constexpr char shaderFilePath[] = "JAZZY/Assets/Shaders/Basic.hlsl";
	std::ifstream shaderStream(shaderFilePath);
	if (!shaderStream) DX3DLogThrowError("Failed to open shader file");
	std::string shaderFileData
	{
		std::istreambuf_iterator<char>(shaderStream),
		std::istreambuf_iterator<char>()
	};

	auto shaderSourceCode = shaderFileData.c_str();
	auto shaderSourceCodeSize = shaderFileData.length();

	auto vs = device.compileShader({shaderFilePath, shaderSourceCode, shaderSourceCodeSize, "VSMain", ShaderType::VertexShader});
	auto ps = device.compileShader({shaderFilePath, shaderSourceCode, shaderSourceCodeSize, "PSMain", ShaderType::PixelShader});
	auto vsSig = device.createVertexShaderSignature({ vs });
	//auto cb = device.createConstantBuffer()

	m_pipeline = device.createGraphicsPipelineState({ *vsSig, *ps });

	const Vertex vertexList[] =
	{
		// Rainbow Rectangle
		{{-0.125f, -0.125f, 0.0f}, {1, 0, 0, 1} },
		{{-0.125f, 0.125f, 0.0f}, {0, 1, 0, 1} },
		{{0.125f, 0.125f, 0.0f}, {0, 0, 1, 1} },
		{{0.125f, 0.125f, 0.0f}, {0, 0, 1, 1} },
		{{0.125f, -0.125f, 0.0f}, {1, 0, 1, 1} },
		{{-0.125f, -0.125f, 0.0f}, {1, 0, 0, 1} },

		// Rainbow Triangle
		{ { 0.5 -0.125f, -0.125f, 0.0f }, {1, 0, 0, 1} },
		{{0.5 -0.125f, 0.125f, 0.0f}, {0, 1, 0, 1} },
		{{0.5 + 0.125f, 0.125f, 0.0f}, {0, 0, 1, 1} },

		// Green Rectangle
		{ { -0.5 -0.125f, -0.125f, 0.0f }, {0, 1, 0, 1} },
		{{-0.5 -0.125f, 0.125f, 0.0f}, {0, 1, 0, 1} },
		{{-0.5 + 0.125f, 0.125f, 0.0f}, {0, 1, 0, 1} },
		{{-0.5 + 0.125f, 0.125f, 0.0f}, {0, 1, 0, 1} },
		{{-0.5 + 0.125f, -0.125f, 0.0f}, {0, 1, 0, 1} },
		{{-0.5 -0.125f, -0.125f, 0.0f}, {0, 1, 0, 1} }
	};
	m_vb = device.createVertexBuffer({vertexList, std::size(vertexList), sizeof(Vertex)});
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
