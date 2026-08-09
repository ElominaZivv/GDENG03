#include <JAZZY/Resource/MaterialResource.h>
#include <JAZZY/Graphics/GraphicsDevice.h>
#include <JAZZY/Resource/ResourceManager.h>
#include <JAZZY/Graphics/GraphicsPipelineLayout.h>

#include <fstream>
#include <filesystem>


jazzy::MaterialResource::MaterialResource(const MaterialResourceDesc& desc) : Resource(desc.base), m_graphicsDevice(desc.graphicsDevice)
{
	std::filesystem::path shaderFile = desc.base.path;

	auto shaderFileStr = shaderFile.string();
	std::ifstream shaderStream(shaderFile);
	if (!shaderStream) DX3DLogThrowError("Failed to open shader file {}.", shaderFileStr.c_str());
	std::string shaderCode{
		std::istreambuf_iterator<char>(shaderStream),
		std::istreambuf_iterator<char>()
	};

	// WARNING this might have issues later if the entrypoint is not "main"
	auto vsBinary = m_graphicsDevice.compileShader({ shaderFileStr.c_str(), shaderCode.c_str(), shaderCode.size(), "VSMain", ShaderType::VertexShader });
	auto psBinary = m_graphicsDevice.compileShader({ shaderFileStr.c_str(), shaderCode.c_str(), shaderCode.size(), "PSMain", ShaderType::PixelShader });

	m_layout = m_graphicsDevice.createGraphicsPipelineLayout({vsBinary, psBinary});
	m_pipeline = m_graphicsDevice.createGraphicsPipelineState({ *m_layout });
}

jazzy::MaterialResource::MaterialResource(const MaterialResource& material, const MaterialResourceDesc& desc): Resource(desc.base), m_graphicsDevice(desc.graphicsDevice)
{
	m_layout = material.m_layout;
	m_pipeline = material.m_pipeline;
}

const jazzy::GraphicsPipelineState& jazzy::MaterialResource::getGraphicsPipelineState() const noexcept
{
	return *m_pipeline;
}
