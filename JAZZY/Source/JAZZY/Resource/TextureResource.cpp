#include <JAZZY/Resource/TextureResource.h>
#include <JAZZY/Resource/ResourceManager.h>
#include <JAZZY/Graphics/GraphicsDevice.h>
#include <fstream>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb-image/stb_image.h>

jazzy::TextureResource::TextureResource(const TextureResourceDesc& desc) : Resource(desc.base)
{
	std::filesystem::path textureFile = desc.base.path;
	auto textureFileStr = textureFile.string();

	auto width{ 0 }, height{ 0 }, channels{ 0 };
	auto pixels = stbi_load(
		textureFileStr.c_str(),
		&width,
		&height,
		&channels,
		STBI_rgb_alpha
	);

	if (!pixels) DX3DLogThrowError("Failed to load texture file {}", textureFileStr.c_str());
	m_texture = desc.graphicsDevice.createTexture({ {width, height}, pixels });

	// saving the file name
	size_t lastSlash = textureFileStr.find_last_of('/');

	if (lastSlash != std::string::npos) {
		fileName = textureFileStr.substr(lastSlash + 1);
		DX3DLogInfo("Loaded texture file '{}'", fileName);
	}
	else {
		DX3DLogError("Could not save TextureResource file name.");
	}
}

jazzy::Texture& jazzy::TextureResource::getTexture()
{
	return *m_texture;
}

std::string jazzy::TextureResource::GetTextureFileName()
{
	return fileName;
}
