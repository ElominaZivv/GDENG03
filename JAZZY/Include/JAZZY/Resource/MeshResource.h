#pragma once
#include <JAZZY/Resource/Resource.h>
#include <vector>

namespace jazzy
{
	class MeshResource final : public Resource
	{
	public:
		explicit MeshResource(const MeshResourceDesc& desc);
	};
}

