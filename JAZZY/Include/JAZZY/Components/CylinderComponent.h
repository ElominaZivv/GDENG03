#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Game/Component.h>

namespace jazzy
{
	class CylinderComponent final : public Component
	{
		dx3d_typeid(CylinderComponent)

	public:
		explicit CylinderComponent(const ComponentDesc& data);

		void setMaterial(const RefPtr<MaterialResource>& material);
		MaterialResource* getMaterial();

		VertexBuffer& getVertexBuffer();
		IndexBuffer& getIndexBuffer();

	private:
		RefPtr<MaterialResource> m_material{};
		RefPtr<VertexBuffer> m_vb{};
		RefPtr<IndexBuffer> m_ib{};
	};
}
