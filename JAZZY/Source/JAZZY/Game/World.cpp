#include <ranges>
#include <JAZZY/Game/World.h>
#include <JAZZY/Game/GameObject.h>
#include <JAZZY/Components/CubeComponent.h>
#include <JAZZY/Components/PlaneComponent.h>
#include <JAZZY/Components/SphereComponent.h>
#include <JAZZY/Components/CylinderComponent.h>
#include <JAZZY/Components/CapsuleComponent.h>

jazzy::World::World(const WorldDesc& desc) : Base(desc.base), m_gameContext(desc.gameContext), m_recordHolder(*this)
{
}

void jazzy::World::update(f32 deltaTime)
{
	// GameObject Creation Event
	if (m_events.size())
	{
		std::swap(m_events, m_eventsSwapBuffer);
		std::swap(m_pendingObjects, m_pendingObjectSwapBuffer);

		for (auto& e : m_eventsSwapBuffer)
		{
			auto objTypeId = e.object->getTypeId();
			auto pendingObjIndex = e.pendingObjectIndex;

			if (e.eventType == EventType::Create)
			{
				auto& obj = m_pendingObjectSwapBuffer[pendingObjIndex];
				auto ptr = obj.get();
				// Gets from the unordered map the type of object (objTypeId) then 
				// pushes back that object into a vector of that typeId
				m_objects[objTypeId].push_back(std::move(obj));
				ptr->onCreate();
			}
		}

		m_pendingObjectSwapBuffer.clear();
		m_eventsSwapBuffer.clear();
	}

	// Update of Individual Transform Components
	for (auto& comp : m_dirtyTransforms)
	{
		comp->updateWorldMatrix();
	}
	m_dirtyTransforms.clear();
}

void jazzy::World::SetSelectedObject(const std::string& name)
{
	auto numCube = 0u;
	auto cubes = getComponents<CubeComponent>(numCube);

	for (ui32 i = 0; i < numCube; ++i)
	{
		if (cubes[i]->getGameObject().m_name == name)
		{
			selectedObjIndex = i;
			selectedObjectType = "Cube";
			return;
		}
	}

	auto numPlane = 0u;
	auto planes = getComponents<PlaneComponent>(numPlane);

	for (ui32 i = 0; i < numPlane; ++i)
	{
		if (planes[i]->getGameObject().m_name == name)
		{
			selectedObjIndex = i;
			selectedObjectType = "Plane";
			return;
		}
	}

	auto numSphere = 0u;
	auto spheres = getComponents<SphereComponent>(numSphere);

	for (ui32 i = 0; i < numSphere; ++i)
	{
		if (spheres[i]->getGameObject().m_name == name)
		{
			selectedObjIndex = i;
			selectedObjectType = "Sphere";
			return;
		}
	}

	auto numCapsule = 0u;
	auto capsules = getComponents<CapsuleComponent>(numCapsule);

	for (ui32 i = 0; i < numCapsule; ++i)
	{
		if (capsules[i]->getGameObject().m_name == name)
		{
			selectedObjIndex = i;
			selectedObjectType = "Capsule";
			return;
		}
	}

	auto numCylinder = 0u;
	auto cylinders = getComponents<CylinderComponent>(numCylinder);

	for (ui32 i = 0; i < numCylinder; ++i)
	{
		if (cylinders[i]->getGameObject().m_name == name)
		{
			selectedObjIndex = i;
			selectedObjectType = "Cylinder";
			return;
		}
	}
}

void jazzy::World::resetSelectedObject()
{
	selectedObjIndex = 0;
	selectedObjectType = "";
}

void jazzy::World::deleteGameObject(GameObject* object)
{
	if (!object)
		return;

	while (object->getChildCount() > 0)
	{
		GameObject* child = object->getChildByIndex(0);
		deleteGameObject(child);
	}

	if (GameObject* parent = object->getParent())
	{
		parent->removeChildByName(object->m_name);
	}
	if (auto* component = object->getComponent<CubeComponent>())
		removeComponent(component);

	if (auto* component = object->getComponent<PlaneComponent>())
		removeComponent(component);

	if (auto* component = object->getComponent<SphereComponent>())
		removeComponent(component);

	if (auto* component = object->getComponent<CapsuleComponent>())
		removeComponent(component);

	if (auto* component = object->getComponent<CylinderComponent>())
		removeComponent(component);

	for (auto& [typeId, objects] : m_objects)
	{
		for (auto it = objects.begin(); it != objects.end(); ++it)
		{
			if (it->get() == object)
			{
				objects.erase(it);
				return;
			}
		}
	}
}

void jazzy::World::removeComponent(Component* component)
{
	if (!component)
		return;

	auto it = m_components.find(component->getTypeId());

	if (it == m_components.end())
		return;

	auto& components = it->second;

	components.erase(std::remove(components.begin(), components.end(), component), components.end());
}

jazzy::ui32 jazzy::World::GetSelectedIndex()
{
	return selectedObjIndex;
}

std::string jazzy::World::getSelectedObjectType()
{
	return selectedObjectType;
}

jazzy::GameObject* jazzy::World::createGameObjectInternal(UniquePtr<GameObject>& object)
{
	if (!object) return{};

	auto ptr = object.get();
	auto index = m_pendingObjects.size();
	m_pendingObjects.push_back(std::move(object));
	m_events.push_back({ ptr, index, EventType::Create });

	return ptr;
}

void jazzy::World::addComponentInternal(Component& component)
{
	auto typeId = component.getTypeId();
	m_components[typeId].push_back(&component);
}

void jazzy::World::addDirtyTransformInternal(TransformComponent& component)
{
	m_dirtyTransforms.push_back(&component);
}

jazzy::Component* const* jazzy::World::getComponentsInternal(size_t typeId, ui32* numComponents) const noexcept
{
	auto it = m_components.find(typeId);
	if (it != m_components.end())
	{
		// This modifies the numComponents =0u in WorldRenderer/GraphicsEngine into the number of instances of components in the world
		*numComponents = static_cast<ui32>(it->second.size());
		return it->second.data();
	}
	*numComponents = 0u;
	return {};
}

jazzy::RecordHolder& jazzy::World::getRecordHolder() noexcept
{
	return m_recordHolder;
}

jazzy::GameObject* jazzy::World::getGameObjectByName(const std::string& name) noexcept
{
	auto numPlane = 0u;
	auto planes = getComponents<PlaneComponent>(numPlane);

	auto numCube = 0u;
	auto cubes = getComponents<CubeComponent>(numCube);

	auto numSphere = 0u;
	auto spheres = getComponents<SphereComponent>(numSphere);

	auto numCapsule = 0u;
	auto capsules = getComponents<CapsuleComponent>(numCapsule);

	auto numCylinder = 0u;
	auto cylinders = getComponents<CylinderComponent>(numCylinder);

	for (ui32 i = 0; i < numPlane; ++i)
	{
		auto* plane = planes[i];

		if (plane->getGameObject().m_name == name)
			return &plane->getGameObject();
	}

	for (ui32 i = 0; i < numCube; ++i)
	{
		auto* cube = cubes[i];

		if (cube->getGameObject().m_name == name)
			return &cube->getGameObject();
	}

	for (ui32 i = 0; i < numSphere; ++i)
	{
		auto* sphere = spheres[i];

		if (sphere->getGameObject().m_name == name)
			return &sphere->getGameObject();
	}

	for (ui32 i = 0; i < numCapsule; ++i)
	{
		auto* capsule = capsules[i];

		if (capsule->getGameObject().m_name == name)
			return &capsule->getGameObject();
	}

	for (ui32 i = 0; i < numCylinder; ++i)
	{
		auto* cylinder = cylinders[i];

		if (cylinder->getGameObject().m_name == name)
			return &cylinder->getGameObject();
	}
	return nullptr;
}