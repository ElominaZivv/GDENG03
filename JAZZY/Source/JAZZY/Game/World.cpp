#include <ranges>
#include <JAZZY/Game/World.h>
#include <JAZZY/Components/RigidBodyComponent.h>
#include <JAZZY/Game/GameObject.h>
#include <JAZZY/Components/CubeComponent.h>
#include <JAZZY/Components/PlaneComponent.h>
#include <JAZZY/Components/SphereComponent.h>
#include <JAZZY/Components/CylinderComponent.h>
#include <JAZZY/Components/CapsuleComponent.h>
#include <JAZZY/Components/MeshComponent.h>

#include <JAZZY/SaveSystem/SaveStructs.h>
#include <JAZZY/Resource/ResourceManager.h>
#include <JAZZY/Game/WorldPhysics.h>

jazzy::World::World(const WorldDesc& desc) : Base(desc.base), m_gameContext(desc.gameContext), worldPhysicsObj(desc.worldPhysics), m_recordHolder(*this)
{
	SetPhysics();
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

void jazzy::World::syncPhysicsComponents()
{
	auto numComponents = 0u;
	auto components = getComponents<RigidBodyComponent>(numComponents);
	for (auto i : std::views::iota(0u, numComponents))
	{
		components[i]->syncPhysicsToTransform();
	}
}

void jazzy::World::SetSelectedObject(const std::string& id)
{
	auto num = 0u;
	auto objs = getComponents<TransformComponent>(num);

	for (auto i : std::views::iota(0u, num)) {
		if (objs[i]->getGameObject().isSelected) {
			objs[i]->getGameObject().isSelected = false;
			break;
		}
	}

	for (auto i : std::views::iota(0u, num))
	{
		if (objs[i]->getGameObject()._id == id)
		{
			selectedObjIndex = i;

			if (objs[i]->getGameObject().getComponent<CubeComponent>())
				selectedObjectType = "Cube";
			else if (objs[i]->getGameObject().getComponent<PlaneComponent>())
				selectedObjectType = "Plane";
			else if (objs[i]->getGameObject().getComponent<SphereComponent>())
				selectedObjectType = "Sphere";
			else if (objs[i]->getGameObject().getComponent<CapsuleComponent>())
				selectedObjectType = "Capsule";
			else if (objs[i]->getGameObject().getComponent<CylinderComponent>())
				selectedObjectType = "Cylinder";
			else if (objs[i]->getGameObject().getComponent<MeshComponent>())
				selectedObjectType = "Mesh";

			objs[i]->getGameObject().isSelected = true;

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

	for (int i = 0; i < object->getChildCount(); i++) {
		deleteGameObject(object->getChildByIndex(i));
	}

	if (GameObject* parent = object->getParent())
	{
		if (parent)
			parent->removeChildById(object->_id);
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

	if (auto* component = object->getComponent<MeshComponent>())
		removeComponent(component);

	if (auto* component = object->getComponent<RigidBodyComponent>())
		removeComponent(component);

	removeComponent(object->getComponent<TransformComponent>());

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

void jazzy::World::SaveCurrentTransforms()
{
	auto num = 0u;
	auto objs = getComponents<TransformComponent>(num);

	for (auto i : std::views::iota(0u, num)) {

		SAVE_Transform save;
		save.m_position = objs[i]->getPosition();
		save.m_rotation = objs[i]->getRotation();
		save.m_scale = objs[i]->getScale();

		EDIT_savedTransforms.push_back(save);
	}
}

void jazzy::World::ResetTransforms()
{
	auto num = 0u;
	auto objs = getComponents<TransformComponent>(num);

	for (auto i : std::views::iota(0u, num)) {
		objs[i]->setPosition(EDIT_savedTransforms[i].m_position);
		objs[i]->setRotation(EDIT_savedTransforms[i].m_rotation);
		objs[i]->setScale(EDIT_savedTransforms[i].m_scale);

	}
}

jazzy::RecordHolder& jazzy::World::getRecordHolder() noexcept
{
	return m_recordHolder;
}

jazzy::RefPtr<jazzy::MeshResource> jazzy::World::LoadMesh(std::string filepath)
{
	std::wstring wideStr(filepath.begin(), filepath.end());
	const wchar_t* filepathFull = wideStr.c_str();

	ResourceManager& resMan = m_gameContext.resourceManager;
	return resMan.createResourceFromFile<jazzy::MeshResource>(filepathFull);
}

std::string jazzy::World::AddGameSceneObject(std::string name, std::vector<ComponentType> compsToAdd)
{
	// Make object
	auto newObj = createGameObject<jazzy::GameObject>(name);
	newObj->_id = name + "_" + std::to_string(SCENE_OBJ_NUM);

	newObj->createOrGetComponent<jazzy::TransformComponent>();

	// Set root
	auto numObj = 0u;
	auto allObjs = getComponents<TransformComponent>(numObj);
	newObj->setParent(&allObjs[0]->getGameObject());

	if (!compsToAdd.empty()) {
		// Add other comps
		bool bodyCompAdded = false;

		for (int i = 0; i < compsToAdd.size(); i++) {

			if (compsToAdd[i] < COMP_RigidBody && bodyCompAdded) {
				DX3DLogWarning("Attempted to add incomptabile components. Object cannot be multiple mesh types.");
				continue;
			}

			switch (compsToAdd[i]) {
				case (COMP_Cube):
					newObj->createOrGetComponent<jazzy::CubeComponent>();
					bodyCompAdded = true;
					break;
				case (COMP_Plane):
					newObj->createOrGetComponent<jazzy::PlaneComponent>();
					bodyCompAdded = true;
					break;
				case (COMP_Capsule):
					newObj->createOrGetComponent<jazzy::CapsuleComponent>();
					bodyCompAdded = true;
					break;
				case (COMP_Cylinder):
					newObj->createOrGetComponent<jazzy::CylinderComponent>();
					bodyCompAdded = true;
					break;
				case (COMP_Sphere):
					newObj->createOrGetComponent<jazzy::SphereComponent>();
					bodyCompAdded = true;
					break;
				case (COMP_Mesh):
					newObj->createOrGetComponent<jazzy::MeshComponent>();
					bodyCompAdded = true;
					break;
				case (COMP_RigidBody):
					newObj->createOrGetComponent<jazzy::RigidBodyComponent>();
					break;
			}
		}
	}

	SCENE_OBJ_NUM++;

	return newObj->_id;
}

std::string jazzy::World::AddGameSceneObject(std::string name, std::vector<ComponentType> compsToAdd, Vec3 position)
{
	// Make object
	auto newObj = createGameObject<jazzy::GameObject>(name);
	newObj->_id = name + "_" + std::to_string(SCENE_OBJ_NUM);

	newObj->createOrGetComponent<jazzy::TransformComponent>();
	newObj->getComponent<TransformComponent>()->setPosition(position);

	// Set root
	auto numObj = 0u;
	auto allObjs = getComponents<TransformComponent>(numObj);
	newObj->setParent(&allObjs[0]->getGameObject());

	if (!compsToAdd.empty()) {
		// Add other comps
		bool bodyCompAdded = false;

		for (int i = 0; i < compsToAdd.size(); i++) {

			if (compsToAdd[i] < COMP_RigidBody && bodyCompAdded) {
				DX3DLogWarning("Attempted to add incomptabile components. Object cannot be multiple mesh types.");
				continue;
			}

			switch (compsToAdd[i]) {
			case (COMP_Cube):
				newObj->createOrGetComponent<jazzy::CubeComponent>();
				bodyCompAdded = true;
				break;
			case (COMP_Plane):
				newObj->createOrGetComponent<jazzy::PlaneComponent>();
				bodyCompAdded = true;
				break;
			case (COMP_Capsule):
				newObj->createOrGetComponent<jazzy::CapsuleComponent>();
				bodyCompAdded = true;
				break;
			case (COMP_Cylinder):
				newObj->createOrGetComponent<jazzy::CylinderComponent>();
				bodyCompAdded = true;
				break;
			case (COMP_Sphere):
				newObj->createOrGetComponent<jazzy::SphereComponent>();
				bodyCompAdded = true;
				break;
			case (COMP_Mesh):
				newObj->createOrGetComponent<jazzy::MeshComponent>();
				bodyCompAdded = true;
				break;
			case (COMP_RigidBody):
				newObj->createOrGetComponent<jazzy::RigidBodyComponent>();
				break;
			}
		}
	}

	SCENE_OBJ_NUM++;

	return newObj->_id;
}

void jazzy::World::ChangeSceneState(SceneState newState)
{
	if (currentSceneState != newState) {
		switch (newState) {
			case (EDIT_MODE):
				worldPhysicsObj.physicsActive = false;
				ResetTransforms();
				EDIT_savedTransforms.clear();
				currentSceneState = EDIT_MODE;
				break;	
			case (PLAY_MODE):
				SaveCurrentTransforms();
				worldPhysicsObj.physicsActive = true;
				if (worldPhysicsObj.isPaused) {
					worldPhysicsObj.isPaused = false;
					worldPhysicsObj.proceed = false;
				}
				currentSceneState = PLAY_MODE;
				break;	
			case (PAUSED_MODE):
				worldPhysicsObj.isPaused = true;
				currentSceneState = PAUSED_MODE;
				break;	
		}
	}
}

void jazzy::World::ProceedWhenPaused()
{
	if (currentSceneState = PAUSED_MODE) worldPhysicsObj.proceed = true;
}

jazzy::GameObject* jazzy::World::getGameObjectByName(const std::string& name) noexcept
{
	auto num = 0u;
	auto allObjs = getComponents<TransformComponent>(num);

	for (auto i : std::views::iota(0u, num))
	{
		auto* obj = allObjs[i];

		if (obj->getGameObject().m_name == name)
			return &obj->getGameObject();
	}

	return nullptr;
}

jazzy::GameObject* jazzy::World::getGameObjectByID(const std::string& name) noexcept
{
	auto num = 0u;
	auto allObjs = getComponents<TransformComponent>(num);

	for (auto i : std::views::iota(0u, num))
	{
		auto* obj = allObjs[i];

		if (obj->getGameObject()._id == name)
			return &obj->getGameObject();
	}

	return nullptr;
}

void jazzy::World::SetPhysics() {
	m_physicsCommon = worldPhysicsObj.getCommon();
	m_worldPhysics = worldPhysicsObj.getWorld();
}