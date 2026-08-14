#pragma once
#include <JAZZY/Resource/Resource.h>

namespace jazzy
{
	class TextureResource final : public jazzy::Resource
	{
	public:
		explicit TextureResource(const TextureResourceDesc& desc);
		jazzy::Texture& getTexture();

		std::string GetTextureFileName();

	private:
		RefPtr<Texture> m_texture{};

		std::string fileName{};
	};
}

