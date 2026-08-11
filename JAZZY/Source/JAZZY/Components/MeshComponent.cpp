#include <JAZZY/Components/MeshComponent.h>
#include <JAZZY/Game/GameObject.h>
#include <JAZZY/Game/World.h>
#include <JAZZY/Game/Game.h>
#include <JAZZY/Graphics/GraphicsDevice.h>
#include <JAZZY/Resource/MaterialResource.h>
#include <JAZZY/Resource/MeshResource.h>

jazzy::MeshComponent::MeshComponent(const ComponentDesc& data) : Component(data)
{
}

void jazzy::MeshComponent::setMesh(const RefPtr<MeshResource>& mesh)
{
	m_mesh = mesh;
	if (m_mesh) m_materials.resize(m_mesh->getNumMaterialSlots());
	else m_materials.resize(0);
}

jazzy::MeshResource* jazzy::MeshComponent::getMesh() const noexcept
{
	return m_mesh.get();
}

void jazzy::MeshComponent::setMaterial(ui32 index, const RefPtr<MaterialResource>& material)
{
	if (index >= m_materials.size())
	{
		DX3DLogError("Index {} is out of bounds for the materials list (size: {}). Ensure setMesh() has been called before setting materials.", index, m_materials.size())
		return;
	}

	m_materials[index] = material;
}

jazzy::MaterialResource* jazzy::MeshComponent::getMaterial(ui32 index) const noexcept
{
	if (index >= m_materials.size())
	{
		// What error?
		//DX3DLogError("Index {} is out of bounds for the materials list (size: {}).", index, m_materials.size())
		return {};
	}
	return m_materials[index].get();
}
