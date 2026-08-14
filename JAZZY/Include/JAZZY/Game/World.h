#pragma once
#include <JAZZY/Core/Common.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/Core/Identifiable.h>
#include <JAZZY/Game/Component.h>
#include <unordered_map>
#include <vector>

#include "JAZZY/Components/TransformComponent.h"
#include "JAZZY/Recorder/RecordHolder.h"
#include <JAZZY/SaveSystem/SaveStructs.h>
#include <JAZZY/SaveSystem/SceneSerializer.h>

namespace jazzy
{
	class World final : public Base
	{
	public:
		explicit World(const WorldDesc& desc);

		template<typename T>
		T* createGameObject(std::string name = "GameObject")
			requires isRegistered<GameObject, T>
		{
			UniquePtr<GameObject> e = std::make_unique<T>(
				GameObjectDesc{
					{m_logger},
					m_gameContext,
					*this,
					name,
					m_physicsCommon,
					m_worldPhysics
				});

			return static_cast<T*>(createGameObjectInternal(e));
		}

		template<typename T>
		T* const* getComponents(ui32& numComponents) const noexcept
		{
			return reinterpret_cast<T* const*>(getComponentsInternal(T::GetTypeId(), &numComponents));
		}

		void update(f32 deltaTime);
		void syncPhysicsComponents();

		// Objects
		void SetSelectedObject(const std::string& name);
		void resetSelectedObject();
		void deleteGameObject(GameObject* object);
		void removeComponent(Component* component);
		ui32 GetSelectedIndex();
		std::string getSelectedObjectType();
		GameObject* getGameObjectByName(const std::string& name) noexcept;
		GameObject* getGameObjectByID(const std::string& name) noexcept;

		RecordHolder& getRecordHolder() noexcept;

		RefPtr<MeshResource> LoadMesh(std::string filepath);
		RefPtr<MaterialResource> LoadMaterial(std::string filepath);
		RefPtr<TextureResource> LoadTexture(std::string filepath);

		// Scene persistence. Level names are read from and written to Game/Assets/Levels.
		void saveScene(const std::string& levelName);
		void loadScene(const std::string& levelName);

		// Hierarchy
		std::string ROOTSCENE_ID = "ROOTSCENE_0";
		enum ComponentType {
			COMP_Cube = 0,
			COMP_Plane,
			COMP_Capsule,
			COMP_Cylinder,
			COMP_Sphere,
			COMP_Mesh,
			COMP_RigidBody
		};
		std::string AddGameSceneObject(std::string name, std::vector<ComponentType> compsToAdd);
		std::string AddGameSceneObject(std::string name, std::vector<ComponentType> compsToAdd, Vec3 position);

		// Scene states
		enum SceneState {
			EDIT_MODE = 0,
			PLAY_MODE,
			PAUSED_MODE
		};
		void ChangeSceneState(SceneState newState);
		void ProceedWhenPaused();
		SceneState currentSceneState = EDIT_MODE;

	private:
		enum class EventType
		{
			Create = 0
		};
		struct GameObjectEvents
		{
			GameObject* object{};
			size_t pendingObjectIndex{};
			EventType eventType{};
		};

	private:
		GameObject* createGameObjectInternal(UniquePtr<GameObject>& object);
		void addComponentInternal(Component& component);
		void addDirtyTransformInternal(TransformComponent& component);

		Component* const* getComponentsInternal(size_t typeId, ui32* numComponents) const noexcept;

		void SaveCurrentTransforms();
		void ResetTransforms();

		void SetPhysics();

	private:
		GameContext m_gameContext;
		WorldPhysics& worldPhysicsObj;
		reactphysics3d::PhysicsCommon* m_physicsCommon{};
		reactphysics3d::PhysicsWorld* m_worldPhysics{};

		// size_t is the typeId of the GameObject which maps to a list of GameObjects of the same typeId?
		std::unordered_map<size_t, std::vector<UniquePtr<GameObject>>> m_objects{};
		std::unordered_map<size_t, std::vector<Component*>> m_components{};

		std::vector<TransformComponent*> m_dirtyTransforms{};

		std::vector<UniquePtr<GameObject>> m_pendingObjects;
		std::vector<UniquePtr<GameObject>> m_pendingObjectSwapBuffer;
		std::vector<GameObjectEvents> m_events{};
		std::vector<GameObjectEvents> m_eventsSwapBuffer{};
	
		RecordHolder m_recordHolder;
		SceneSerializer& m_sceneSerializer;

		// selection
		ui32 selectedObjIndex = 0;
		std::string selectedObjectType = "";
		int SCENE_OBJ_NUM = 0;

		// scene states again
		std::vector<SAVE_Transform> EDIT_savedTransforms{};

		friend class GameObject;
		friend class TransformComponent;

	};
}
