#include <filesystem>
#include <JAZZY/Resource/MaterialResource.h>

#include <fstream>
#include <filesystem>

#include "JAZZY/Graphics/GraphicsDevice.h"

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
	// WARNING we need to make separate shader file paths for the vertex and pixel :(((((((((((
	auto vsBinary = m_graphicsDevice.compileShader({ shaderFileStr.c_str(), shaderCode.c_str(), shaderCode.size(), "main", ShaderType::VertexShader });
	auto psBinary = m_graphicsDevice.compileShader({ shaderFileStr.c_str(), shaderCode.c_str(), shaderCode.size(), "main", ShaderType::PixelShader });
}

jazzy::MaterialResource::MaterialResource(const MaterialResource& material, const MaterialResourceDesc& desc): Resource(desc.base), m_graphicsDevice(desc.graphicsDevice)
{
}
