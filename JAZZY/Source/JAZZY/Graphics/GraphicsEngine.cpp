#include <JAZZY/Graphics/GraphicsEngine.h>
#include <JAZZY/Graphics/GraphicsDevice.h>
#include <JAZZY/Graphics/DeviceContext.h>
#include <JAZZY/Graphics/SwapChain.h>
#include <JAZZY/Graphics/VertexBuffer.h>	
#include <JAZZY/Graphics/IndexBuffer.h>	
#include <JAZZY/Math/Vec3.h>
#include <JAZZY/Math/Vertex.h>
#include "JAZZY/Input/InputSystem.h"
#include <fstream>
#include <numbers>
#include <ranges>
#include <string>
#include <random>


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

	// Generate the polygon vertex list and index list
	generatePolygonVerticesAndIndexLists(0.45, 16);

	m_vb = device.createVertexBuffer({ polygonVertexList.data(), static_cast<ui32>(polygonVertexList.size()), sizeof(Vertex) });

	m_ib = device.createIndexBuffer({ polygonIndexList.data(), static_cast<ui32>(polygonIndexList.size()) });

	// Constant Buffer
	m_cb = device.createConstantBuffer({ {}, sizeof(ConstantData) });

	// TEMPORARY DEPENDECY FOR DEBUGGING
	m_inputSystem = desc.inputSystem;
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
	//context.clearAndSetBackBuffer(swapChain, { 0.0549, 0.07, 0.109, 1 });
	context.clearAndSetBackBuffer(swapChain, { 0.0f, 0.0f, 0.0f, 1 });
	context.setGraphicsPipelineState(*m_pipeline);

	context.setViewportSize(swapChain.getSize());

	for (auto i : std::views::iota(0u, balls.size()))
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

		// Update Ball Position
		balls[i].position = Vec3
		( 
			balls[i].position.x + (balls[i].velocity.x * deltaTime), 
			balls[i].position.y + (balls[i].velocity.y * deltaTime), 
			0.0f
		);
		// Collision Detection and Resolution
		if (balls[i].position.x > 1.175f)
		{
			balls[i].position.x = 1.175f;
			balls[i].velocity.x = -balls[i].velocity.x;
		}
		if (balls[i].position.y > 0.85f)
		{
			balls[i].position.y = 0.85f;
			balls[i].velocity.y = -balls[i].velocity.y;
		}
		if (balls[i].position.x < -1.175f)
		{
			balls[i].position.x = -1.175f;
			balls[i].velocity.x = -balls[i].velocity.x;
		}
		if (balls[i].position.y < -0.85f)
		{
			balls[i].position.y = -0.85f;
			balls[i].velocity.y = -balls[i].velocity.y;
		}
		// Feed Constant data with updated values
		updateConstantData(data, balls[i]);
		//DX3DLogInfo((std::to_string(data.m_time)).c_str());

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

std::vector<Ball>* GraphicsEngine::getBallObjects()
{
	return &balls;
}

void GraphicsEngine::updateConstantData(ConstantData& data, Ball ball)
{
	// World
	Mat4x4 temp{};
	temp = Mat4x4::identity();

	roty = m_time /1000.0f;
	temp = temp * Mat4x4::rotateX(rotx / 10.0f);
	temp = temp * Mat4x4::rotateY(roty / 10.0f);
	temp = temp * Mat4x4::rotateZ(rotz / 10.0f);

	f32 scale = ball.radius;
	temp = temp * Mat4x4::scale(Vec3{ scale, scale, scale});
	temp = temp * Mat4x4::translation(ball.position);
	data.m_world = temp;

	// View
	data.m_view = Mat4x4::identity();

	// Orthographic View
	int zzWindowDisplayHeight = 768;	// Originally 720
	int zzWindowDisplayWidth = 1024;	// Originally 1280
	data.m_projection = Mat4x4::orthoLH
	(	
		// Instead of hardcoding the resolution, find a way to access the Rect size{} of the window
		// Moreover, you can also update the Rect size{} by using the Window msg to check whenever the size is changed and update the Rect size accordingly
		zzWindowDisplayWidth/400.0f,
		zzWindowDisplayHeight/400.0f,
		-4.0f,
		4.0f
	);
}

void GraphicsEngine::generatePolygonVerticesAndIndexLists(f32 radius, ui32 num_vertices)
{
	// Formula taken from https://faun.pub/draw-circle-in-opengl-c-2da8d9c2c103

	f32 angle = 360.f / num_vertices;

	polygonVertexList.push_back(Vertex({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.15f, 0.0f, 1.0f }));
	for (int i = 0; i < num_vertices; i++)
	{
		f32 currentAngle = angle * i;
		f32 x = radius * sin(currentAngle * std::numbers::pi / 180.0);
		f32 y = radius * cos(currentAngle * std::numbers::pi / 180.0);
		f32 z = 0.0f;

		polygonVertexList.push_back(Vertex({x,y,z},{ 0.1f, 0.3f, 0.2f, 1.0f }));
	}
	for (auto i : std::views::iota(0u, num_vertices))
	{
		polygonIndexList.push_back(0);
		polygonIndexList.push_back(1+i);
		if (i == num_vertices-1)
		{
			polygonIndexList.push_back(1);
		}
		else
		{
			polygonIndexList.push_back(2+i);
		}
	}
}
