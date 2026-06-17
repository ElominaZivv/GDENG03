#include <JAZZY/Graphics/GraphicsEngine.h>
#include <JAZZY/Graphics/GraphicsDevice.h>
#include <JAZZY/Graphics/DeviceContext.h>
#include <JAZZY/Graphics/SwapChain.h>
#include <JAZZY/Graphics/VertexBuffer.h>	
#include <JAZZY/Math/Vec3.h>
#include <fstream>
#include <string>
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

	float myOffset = 0.3;

	Quad rainbowRectangle
	(
		{{-0.125f - myOffset, 0.125f, 0.0f}, {0, 1, 0, 1} },
		{{0.125f - myOffset, 0.125f, 0.0f}, {0, 0, 1, 1} },
		{{0.125f - myOffset, -0.125f, 0.0f}, {1, 0, 1, 1} },
		{ {-0.125f - myOffset, -0.125f, 0.0f}, {1, 0, 0, 1} }
	);
	addQuadIntoVertexList(rainbowRectangle);

	/*Triangle rainbowTriangle
	(
		{ {0, 0.125f, 0.0f }, {1, 0, 0, 1} },
		{{0.1f, -0.125f, 0.0f}, {0, 1, 0, 1} },
		{{-0.1, -0.125f, 0.0f}, {0, 0, 1, 1} }
	);
	addTriangleIntoVertexList(rainbowTriangle);*/

	Quad middleRectangle
	(
		{ {-0.125f, 0.125f, 0.0f}, {1, 0, 0, 1} },
		{ {+0.125f, 0.125f, 0.0f}, {0, 1, 0, 1} },
		{ {+0.125f, -0.125f, 0.0f}, {1, 0, 0, 1} },
		{ {-0.125f, -0.125f, 0.0f}, {0, 1, 0, 1} }
	);
	addQuadIntoVertexList(middleRectangle);

	Quad greenRectangle
	(
		{ {-0.125f + myOffset, 0.125f, 0.0f}, {0, 1, 0, 1} },
		{ {+0.125f + myOffset, 0.125f, 0.0f}, {0, 1, 0, 1} },
		{ {+0.125f + myOffset, -0.125f, 0.0f}, {0, 1, 0, 1} },
		{ {-0.125f + myOffset, -0.125f, 0.0f}, {0, 1, 0, 1} }
	);
	addQuadIntoVertexList(greenRectangle);

	m_vb = device.createVertexBuffer({ vertexList.data(), static_cast<ui32>(vertexList.size()), sizeof(Vertex) });

	m_cb = device.createConstantBuffer({ {}, sizeof(ConstantData) });
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

	updateConstantBuffer(context);

	context.drawTriangleList(vb.getVertexListSize(), 0u);

	auto& device = *m_graphicsDevice;
	device.executeCommandList(context);
	swapChain.present();

}

void GraphicsEngine::updateConstantBuffer(DeviceContext& context)
{
	// Compute for delta time
	time_curr = ::GetTickCount();
	delta_time = time_curr - time_prev;
	if (delta_time < 100)
	{
		time += delta_time;
	}
	time_prev = time_curr;

	// Declare ConstantData data
	ConstantData data{};

	// Manipulate the constant data here
	// Time
	data.m_time = time;
	// World
	data.m_world = Mat4x4::scale(Vec3{1.0f, 1.0f, 1.0f });

	Mat4x4 temp{};
	temp = Mat4x4::translation(Vec3{ 0.0f, 0.0f, 0.0f });

	data.m_world = data.m_world * temp;

	// View
	data.m_view = Mat4x4::identity();
	// Orthographic View
	int zzWindowDisplayHeight = 400;	// Originally 720
	int zzWindowDisplayWidth = zzWindowDisplayHeight * 1.78;	// Originally 1280
	data.m_projection = Mat4x4::orthoLH
	(	
		// Instead of hardcoding the resolution, find a way to access the Rect size{} of the window
		// Moreover, you can also update the Rect size{} by using the Window msg to check whenever the size is changed and update the Rect size accordingly
		zzWindowDisplayWidth / 400.0f,
		zzWindowDisplayHeight / 400.0f,
		-4.0f,
		4.0f
	);

	// Get the ConstantBuffer,
	auto& cb = *m_cb;
	// Update it with the data, 
	context.updateConstantBuffer(cb, &data);
	// then send it to the vertex and pixel shader
	context.setConstantBuffer(cb);
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
