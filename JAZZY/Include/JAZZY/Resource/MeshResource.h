#pragma once
#include <JAZZY/Resource/Resource.h>
#include <vector>

namespace jazzy
{
	class MeshResource final : public Resource
	{
	public:
		explicit MeshResource(const MeshResourceDesc& desc);
		const MaterialSlot* getMaterialSlots(ui32& numSlots) const noexcept;

		ui32 getNumMaterialSlots() const noexcept;
		const VertexBuffer& getVertexBuffer() const noexcept;
		const IndexBuffer& getIndexBuffer() const noexcept;
	private:
		RefPtr<VertexBuffer> m_vertexBuffer{};
		RefPtr<IndexBuffer> m_indexBuffer{};
		std::vector<MaterialSlot> m_matSlots{};
	};
}

