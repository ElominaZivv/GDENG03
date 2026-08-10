#pragma once
#include <JAZZY/Resource/Resource.h>

namespace jazzy
{
	class TextureResource final : public jazzy::Resource
	{
	public:
		explicit TextureResource(const TextureResourceDesc& desc);
		jazzy::Texture& getTexture();

	private:
		RefPtr<Texture> m_texture{};
	};
}

