#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Game/Component.h>
#include <vector>

namespace jazzy
{
	class MeshComponent final : public Component
	{
		dx3d_typeid(MeshComponent)
	public:
		explicit MeshComponent(const ComponentDesc& data);

		void setMesh(const RefPtr<MeshResource>& mesh);
		MeshResource* getMesh() const noexcept;

		void setMaterial(ui32 index, const RefPtr<MaterialResource>& material);
		MaterialResource* getMaterial(ui32 index) const noexcept;
	private:
		RefPtr<MeshResource> m_mesh{};
		std::vector<RefPtr<MaterialResource>> m_materials{};
	};

}