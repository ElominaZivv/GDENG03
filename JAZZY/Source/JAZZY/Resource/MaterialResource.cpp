#include <filesystem>
#include <JAZZY/Resource/MaterialResource.h>

#include <fstream>
#include <filesystem>

jazzy::MaterialResource::MaterialResource(const MaterialResourceDesc& desc) : Resource(desc.base), m_graphicsDevice(desc.graphicsDevice)
{
	std::filesystem::path shaderFile = desc.base.path;

	auto shaderFileStr = shaderFile.string();
	std::ifstream shaderStream(shaderFile);
	if (!shaderStream) DX3DLogThrowError("Failed to open shader file {}.", shaderFileStr.c_str());

}

jazzy::MaterialResource::MaterialResource(const MaterialResource& material, const MaterialResourceDesc& desc): Resource(desc.base), m_graphicsDevice(desc.graphicsDevice)
{
}
