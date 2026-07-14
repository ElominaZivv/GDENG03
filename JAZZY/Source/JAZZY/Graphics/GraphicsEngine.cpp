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

#include "JAZZY/Components/CubeComponent.h"
#include "JAZZY/EditorCamera/EditorCamera.h"
#include "JAZZY/Game/World.h"

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
	f32 cubeSize = 0.25f;
	Vec4 orange(0.99f, 0.16f, 0.01f, 1.0f);
	Vec4 mikublue(0.03f, 0.74f, 0.68f, 1.0f);
	Vec4 ourple(0.34f, 0.0f, 0.94f, 1.0f);
	Vec4 black(0.0f, 0.0f, 0.0f, 1.0f);
	Vertex vertexList[] =
	{
		// Front Face
		Vertex{ {-cubeSize,-cubeSize,-cubeSize}, orange},
		Vertex{ {-cubeSize,cubeSize,-cubeSize}, mikublue},
		Vertex{ {cubeSize,cubeSize,-cubeSize}, ourple},
		Vertex{ {cubeSize,-cubeSize,-cubeSize}, black},
		// Back Face
		Vertex{ {cubeSize,-cubeSize,cubeSize}, orange},
		Vertex{ {cubeSize,cubeSize,cubeSize}, mikublue},
		Vertex{ {-cubeSize,cubeSize,cubeSize}, ourple},
		Vertex{ {-cubeSize,-cubeSize,cubeSize}, black}
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

	// Editor Camera
	m_editorCamera = desc.editorCamera;
}

jazzy::GraphicsEngine::~GraphicsEngine()
{
}

jazzy::GraphicsDevice& jazzy::GraphicsEngine::getGraphicsDevice() noexcept
{
	return *m_graphicsDevice;
}

void GraphicsEngine::render(World& world, SwapChain& swapChain, f32 deltaTime)
{
	auto& context = *m_deviceContext;
	context.clearAndSetBackBuffer(swapChain, { 0.0549, 0.07, 0.109, 1 });
	context.setGraphicsPipelineState(*m_pipeline);

	context.setViewportSize(swapChain.getSize());

	auto numComponents = 0u;
	{
		auto components = world.getComponents<CubeComponent>(numComponents);
		for (auto i : std::views::iota(0u, numComponents))
		{
			auto component = components[i];

			// Constant Data
			ConstantData data{};

			// Time
			m_time += deltaTime;
			data.m_time = m_time;

			// World
			Mat4x4 worldMat{};
			worldMat = Mat4x4::identity();
			worldMat = worldMat * Mat4x4::scale({ 1.0f, 1.0f, 1.0f});
			worldMat = worldMat * Mat4x4::translation( {0.0f, 0.0f, -3.0f });
			data.m_world = worldMat;

			// View
			Mat4x4 worldCam{};
			worldCam = m_editorCamera->getViewMat();
			data.m_view = worldCam;

			// Projection View
			Mat4x4 projection{};
			projection = m_editorCamera->getProjectionViewMat();
			data.m_projection = projection;

			auto& cb = *m_cb;
			context.updateConstantBuffer(cb, &data);
			context.setConstantBuffer(cb);

			context.setVertexBuffer(component->getVertexBuffer());
			context.setIndexBuffer(component->getIndexBuffer());
			context.drawIndexedTriangleList(component->getIndexBuffer().getIndexListSize(), 0u, 0u);
		}
	}

	auto& device = *m_graphicsDevice;
	device.executeCommandList(context);
	swapChain.present();
}
