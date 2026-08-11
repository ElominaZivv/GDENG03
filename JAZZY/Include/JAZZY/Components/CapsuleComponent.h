#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Game/Component.h>

namespace jazzy
{
	class CapsuleComponent final : public Component
	{
		dx3d_typeid(CapsuleComponent)

	public:
		explicit CapsuleComponent(const ComponentDesc& data);

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
