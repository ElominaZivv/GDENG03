#include <JAZZY/Graphics/GraphicsEngine.h>
#include <JAZZY/Graphics/GraphicsDevice.h>
#include <JAZZY/Graphics/DeviceContext.h>
#include <JAZZY/Graphics/SwapChain.h>
#include <JAZZY/Graphics/VertexBuffer.h>	
#include <JAZZY/Math/Vec3.h>
#include <fstream>
using namespace jazzy;

jazzy::GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc) : Base(desc.base)
{
	m_graphicsDevice = std::make_shared< GraphicsDevice>(GraphicsDeviceDesc{ m_logger });

	auto& device = *m_graphicsDevice;
	m_deviceContext = device.createDeviceContext();

	// Original
	/*constexpr char shaderFilePath[] = "JAZZY/Assets/Shaders/Basic.hlsl";
	std::ifstream shaderStream(shaderFilePath);
	if (!shaderStream) DX3DLogThrowError("Failed to open shader file");
	std::string shaderFileData
	{
		std::istreambuf_iterator<char>(shaderStream),
		std::istreambuf_iterator<char>()
	};

	auto shaderSourceCode = shaderFileData.c_str();
	auto shaderSourceCodeSize = shaderFileData.length();*/

	// VertexShader
	constexpr char vShaderFilePath[] = "JAZZY/Assets/Shaders/BasicVertexShader.hlsl";
	std::ifstream vShaderStream(vShaderFilePath);
	if (!vShaderStream) DX3DLogThrowError("Failed to open vertex shader file");
	std::string vShaderFileData
	{
		std::istreambuf_iterator<char>(vShaderStream),
		std::istreambuf_iterator<char>()
	};

	auto vShaderSourceCode = vShaderFileData.c_str();
	auto vShaderSourceCodeSize = vShaderFileData.length();

	// PixelShader
	constexpr char pShaderFilePath[] = "JAZZY/Assets/Shaders/BasicPixelShader.hlsl";
	std::ifstream pShaderStream(pShaderFilePath);
	if (!pShaderStream) DX3DLogThrowError("Failed to open pixel shader file");
	std::string pShaderFileData
	{
		std::istreambuf_iterator<char>(pShaderStream),
		std::istreambuf_iterator<char>()
	};

	auto pShaderSourceCode = pShaderFileData.c_str();
	auto pShaderSourceCodeSize = pShaderFileData.length();

	auto vs = device.compileShader({ vShaderFilePath, vShaderSourceCode, vShaderSourceCodeSize, "main", ShaderType::VertexShader });
	auto ps = device.compileShader({ pShaderFilePath, pShaderSourceCode, pShaderSourceCodeSize, "main", ShaderType::PixelShader });
	auto vsSig = device.createVertexShaderSignature({ vs });

	m_pipeline = device.createGraphicsPipelineState({ *vsSig, *ps });

	float myOffset = 0.25;

	Quad rainbowRectangle
	(
		{{-0.125f - myOffset, 0.125f, 0.0f}, {0, 1, 0, 1} },
		{{0.125f - myOffset, 0.125f, 0.0f}, {0, 0, 1, 1} },
		{{0.125f - myOffset, -0.125f, 0.0f}, {1, 0, 1, 1} },
		{ {-0.125f - myOffset, -0.125f, 0.0f}, {1, 0, 0, 1} }
	);
	addQuadIntoVertexList(rainbowRectangle);

	Triangle rainbowTriangle
	(
		{ {0, 0.125f, 0.0f }, {1, 0, 0, 1} },
		{{0.1f, -0.125f, 0.0f}, {0, 1, 0, 1} },
		{{-0.1, -0.125f, 0.0f}, {0, 0, 1, 1} }
	);
	addTriangleIntoVertexList(rainbowTriangle);

	Quad greenRectangle
	(
		{ {-0.125f + myOffset, 0.125f, 0.0f}, {0, 1, 0, 1} },
		{ {+0.125f + myOffset, 0.125f, 0.0f}, {0, 1, 0, 1} },
		{ {+0.125f + myOffset, -0.125f, 0.0f}, {0, 1, 0, 1} },
		{ {-0.125f + myOffset, -0.125f, 0.0f}, {0, 1, 0, 1} }
	);
	addQuadIntoVertexList(greenRectangle);

	m_vb = device.createVertexBuffer({vertexList.data(), static_cast<ui32>(vertexList.size()), sizeof(Vertex) });
	
	//const Vertex vertexList[] =
	//{
	//	// Rainbow Rectangle
	//	{{-0.125f, -0.125f, 0.0f}, {1, 0, 0, 1} },
	//	{{-0.125f, 0.125f, 0.0f}, {0, 1, 0, 1} },
	//	{{0.125f, 0.125f, 0.0f}, {0, 0, 1, 1} },
	//	{{0.125f, 0.125f, 0.0f}, {0, 0, 1, 1} },
	//	{{0.125f, -0.125f, 0.0f}, {1, 0, 1, 1} },
	//	{{-0.125f, -0.125f, 0.0f}, {1, 0, 0, 1} },

	//	// Rainbow Triangle
	//	{ { 0.5 -0.125f, -0.125f, 0.0f }, {1, 0, 0, 1} },
	//	{{0.5 -0.125f, 0.125f, 0.0f}, {0, 1, 0, 1} },
	//	{{0.5 + 0.125f, 0.125f, 0.0f}, {0, 0, 1, 1} },

	//	// Green Rectangle
	//	{ { -0.5 -0.125f, -0.125f, 0.0f }, {0, 1, 0, 1} },
	//	{{-0.5 -0.125f, 0.125f, 0.0f}, {0, 1, 0, 1} },
	//	{{-0.5 + 0.125f, 0.125f, 0.0f}, {0, 1, 0, 1} },
	//	{{-0.5 + 0.125f, 0.125f, 0.0f}, {0, 1, 0, 1} },
	//	{{-0.5 + 0.125f, -0.125f, 0.0f}, {0, 1, 0, 1} },
	//	{{-0.5 -0.125f, -0.125f, 0.0f}, {0, 1, 0, 1} }
	//};
	
	//m_vb = device.createVertexBuffer({vertexList, std::size(vertexList), sizeof(Vertex)});
	

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

void GraphicsEngine::addTriangleIntoVertexList(Triangle triangle)
{
	vertexList.push_back(triangle.p1);
	vertexList.push_back(triangle.p2);
	vertexList.push_back(triangle.p3);
}

void GraphicsEngine::addQuadIntoVertexList(Quad quad)
{
	addTriangleIntoVertexList(quad.leftTriangle);
	addTriangleIntoVertexList(quad.rightTriangle);
}
