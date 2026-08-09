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

	private:
		GraphicsDevice& m_graphicsDevice;

		RefPtr<GraphicsPipelineLayout> m_layout{};
		RefPtr<GraphicsPipelineState> m_pipeline{};
	};
}
