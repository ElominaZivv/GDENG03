#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Game/Component.h>

namespace jazzy
{
	class SphereComponent final : public Component
	{
		dx3d_typeid(SphereComponent)

	public:
		explicit SphereComponent(const ComponentDesc& data);

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
