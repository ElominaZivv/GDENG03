#include <JAZZY/Graphics/GraphicsEngine.h>
#include <JAZZY/Graphics/GraphicsDevice.h>
#include <JAZZY/Graphics/DeviceContext.h>
#include <JAZZY/Graphics/SwapChain.h>
#include <JAZZY/Graphics/VertexBuffer.h>	
#include <JAZZY/Graphics/IndexBuffer.h>	
#include "JAZZY/Input/InputSystem.h"
#include <JAZZY/Math/Vec3.h>
#include <JAZZY/Math/Vertex.h>
#include <fstream>
#include <string>
#include <ranges>

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

	// Graphics Pipeline
	m_pipeline = device.createGraphicsPipelineState({ *vsSig, *ps });

	/*
	Vertex vertexList[] =
	{
	{ {0, 0.5f, 0.0f }, {1, 0, 0, 1} },
	{{0.35f, -0.5f, 0.0f}, {0, 1, 0, 1} },
	{{-0.35, -0.5f, 0.0f}, {0, 0, 1, 1} }
	};
	*/

	// Vertex Buffer
	f32 cubeSize = 0.5f;
	Vertex vertexList[] =
	{
		// Front Face
		Vertex{ {-cubeSize,-cubeSize,-cubeSize}, {0.99f, 0.16f, 0.01f, 1.0f} },
		Vertex{ {-cubeSize,cubeSize,-cubeSize}, {0.03f, 0.74f, 0.68f, 1.0f} },
		Vertex{ {cubeSize,cubeSize,-cubeSize}, {0.0f, 0.0f, 0.0f, 1.0f} },
		Vertex{ {cubeSize,-cubeSize,-cubeSize}, {0.34f, 0.0f, 0.94f, 1.0f} },
		// Back Face
		Vertex{ {cubeSize,-cubeSize,cubeSize}, {0.0f, 0.0f, 0.0f, 1.0f} },
		Vertex{ {cubeSize,cubeSize,cubeSize}, {0.34f, 0.0f, 0.94f, 1.0f} },
		Vertex{ {-cubeSize,cubeSize,cubeSize}, {0.99f, 0.16f, 0.01f, 1.0f} },
		Vertex{ {-cubeSize,-cubeSize,cubeSize}, {0.03f, 0.74f, 0.68f, 1.0f} }
	};

	m_vb = device.createVertexBuffer({ vertexList, std::size(vertexList), sizeof(Vertex) });

	// Index Buffer
	ui32 indexList[] =
	{
		// Front Side
		0,1,2,
		2,3,0,
		// Back Side
		4,5,6,
		6,7,4,
		// Top Side
		1,6,5,
		5,2,1,
		// Bottom Side
		7,0,3,
		3,4,7,
		// Left Side
		3,2,5,
		5,4,3,
		// Right Side
		7,6,1,
		1,0,7
	};
	m_ib = device.createIndexBuffer({indexList, std::size(indexList)});

	// Constant Buffer
	m_cb = device.createConstantBuffer({ {}, sizeof(ConstantData) });

	// TEMPORARY DEPENDECY FOR DEBUGGING
	m_inputSystem = desc.inputSystem;
	m_TempWorldCam = Mat4x4::translation(Vec3(0.0f, 0.0f, -2.0f));
}

jazzy::GraphicsEngine::~GraphicsEngine()
{
}

jazzy::GraphicsDevice& jazzy::GraphicsEngine::getGraphicsDevice() noexcept
{
	return *m_graphicsDevice;
}

void GraphicsEngine::render(f32 deltaTime, SwapChain& swapChain)
{
	auto& context = *m_deviceContext;
	context.clearAndSetBackBuffer(swapChain, { 0.0549, 0.07, 0.109, 1 });
	context.setGraphicsPipelineState(*m_pipeline);

	context.setViewportSize(swapChain.getSize());

	for (auto i : std::views::iota(0u, cubes.size()))
	{
		auto& vb = *m_vb;
		context.setVertexBuffer(vb);

		auto& ib = *m_ib;
		context.setIndexBuffer(ib);

		// Constant Buffer
		// Get the ConstantBuffer,
		auto& cb = *m_cb;

		// Declare Constant Data
		ConstantData data{};

		// Feed Constant data with updated values
		updateConstantData(deltaTime, data, i);
		//DX3DLogInfo((std::to_string(deltaTime)).c_str());

		// Update it with the data, 
		context.updateConstantBuffer(cb, &data);

		// then send it to the vertex and pixel shader
		context.setConstantBuffer(cb);

		context.drawIndexedTriangleList(ib.getIndexListSize(), 0u, 0u);
		//context.drawTriangleList(vb.getVertexListSize(), 0u);
	}

	auto& device = *m_graphicsDevice;
	device.executeCommandList(context);
	swapChain.present();

}

void GraphicsEngine::updateConstantData(f32 deltaTime, ConstantData& data, ui32 index)
{
	// Time
	m_time += deltaTime;
	data.m_time = m_time;

	// World
	Mat4x4 worldMat{};
	worldMat = Mat4x4::identity();
	worldMat = worldMat * Mat4x4::scale(cubes[index].scale);
	worldMat = worldMat * Mat4x4::translation(cubes[index].position);
	data.m_world = worldMat;

	// View
	Mat4x4 worldCam{};
	worldCam = Mat4x4::identity();

	m_inputSystem->setCursorLocked(true);
	rotx += m_inputSystem->getMouseDelta().y;
	roty += m_inputSystem->getMouseDelta().x;

	worldCam = worldCam * Mat4x4::rotateX(rotx / 100.0f);
	worldCam = worldCam * Mat4x4::rotateY(roty / 100.0f);
	worldCam = worldCam * Mat4x4::rotateZ(rotz / 100.0f);

	f32 speed = 2.5f;
	if (m_inputSystem->isKeyDown(KeyCode::W)) forward += deltaTime * speed;
	if (m_inputSystem->isKeyDown(KeyCode::S)) forward -= deltaTime * speed;
	//if (m_inputSystem->isKeyDown(KeyCode::D)) right += deltaTime * speed;
	//if (m_inputSystem->isKeyDown(KeyCode::A)) right -= deltaTime * speed;


	Vec3 tempWorldCamPos({ m_TempWorldCam.row(3).x, m_TempWorldCam.row(3).y, m_TempWorldCam.row(3).z });
	Vec3 camForward({ worldCam.row(2).x, worldCam.row(2).y, worldCam.row(2).z });
	//Vec3 camRight({ worldCam.row(0).x, worldCam.row(0).y, worldCam.row(0).z });
	//camRight *= right;

	Vec3 newPos = tempWorldCamPos + camForward * forward;
	//newPos += camRight;
	worldCam = worldCam * Mat4x4::translation(newPos);
	//worldCam = worldCam * Mat4x4::translation(Vec3{ 0.0f, 0.0f, -2.0f });
	worldCam = Mat4x4::inverse(worldCam);

	data.m_view = worldCam;

	// Orthographic View
	/*
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
	*/

	// Perspective View
	int zzWindowDisplayHeight = 400;	// Originally 720
	int zzWindowDisplayWidth = zzWindowDisplayHeight * 1.78;	// Originally 1280
	data.m_projection = Mat4x4::perspectiveFovLH
	(
		1.57f,
		(f32)zzWindowDisplayWidth / (f32)zzWindowDisplayHeight,
		0.1f,
		100.0f
	);
}

std::vector<Cube>* GraphicsEngine::getCubes()
{
	return &cubes;
}
