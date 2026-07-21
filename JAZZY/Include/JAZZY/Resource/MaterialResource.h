#pragma once

#pragma once
#include <JAZZY/Resource/Resource.h>
#include <vector>
#include <span>

namespace jazzy
{
	class MaterialResource final : public Resource
	{
	public:
		MaterialResource(const MaterialResourceDesc& desc);
		MaterialResource(const MaterialResource& material, const MaterialResourceDesc& desc);

	private:
		GraphicsDevice& m_graphicsDevice;
	};
}
