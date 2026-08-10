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
}

jazzy::Texture& jazzy::TextureResource::getTexture()
{
	return *m_texture;
}
