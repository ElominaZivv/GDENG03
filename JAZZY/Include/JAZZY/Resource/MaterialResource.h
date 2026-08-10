#pragma once
#include <JAZZY/Resource/Resource.h>
#include <vector>
#include <span>

namespace jazzy
{
	class MaterialResource final : public Resource
	{
	public:
		explicit MaterialResource(const MaterialResourceDesc& desc);
		MaterialResource(const MaterialResource& material, const MaterialResourceDesc& desc);

		const GraphicsPipelineState& getGraphicsPipelineState() const noexcept;
		void setData(const std::span<const std::byte>& data);
		const std::span<const std::byte> getData() const noexcept;

		TextureResource* getTexture(size_t index);
		size_t getNumTextures() const noexcept;

	public:
		static constexpr std::size_t MaxDataSize{ 256 };
	private:
		GraphicsDevice& m_graphicsDevice;

		RefPtr<GraphicsPipelineLayout> m_layout{};
		RefPtr<GraphicsPipelineState> m_pipeline{};

		std::vector<RefPtr<TextureResource>> m_textures{};

		std::byte m_data[MaxDataSize]{};
		size_t m_dataSize{};
	};
}
