#pragma once
#include <JAZZY/Core/Common.h>
#include <JAZZY/Graphics/GraphicsResource.h>

namespace jazzy
{
	class Texture final : public GraphicsResource
	{
	public:
		Texture(const TextureDesc& desc, const GraphicsResourceDesc& gDesc);
	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture{};
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv{};
		friend class DeviceContext;
	};
}
