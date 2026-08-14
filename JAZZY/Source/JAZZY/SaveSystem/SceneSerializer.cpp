#include <JAZZY/SaveSystem/SceneSerializer.h>

#include <JAZZY/Components/CapsuleComponent.h>
#include <JAZZY/Components/CubeComponent.h>
#include <JAZZY/Components/CylinderComponent.h>
#include <JAZZY/Components/MeshComponent.h>
#include <JAZZY/Components/PlaneComponent.h>
#include <JAZZY/Components/RigidBodyComponent.h>
#include <JAZZY/Components/SphereComponent.h>
#include <JAZZY/Components/TransformComponent.h>
#include <JAZZY/Game/GameObject.h>
#include <JAZZY/Game/World.h>
#include <JAZZY/Resource/MaterialResource.h>
#include <JAZZY/Resource/MeshResource.h>
#include <JAZZY/Resource/TextureResource.h>

#include <fstream>
#include <iomanip>
#include <ranges>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace jazzy;

struct SavedObject
{
	std::string name;
	std::string id;
	std::string parentId;
	std::vector<World::ComponentType> components;
	std::string meshPath;
	std::string materialPath;
	std::vector<std::string> texturePaths;
	Vec3 position;
	Vec3 rotation;
	Vec3 scale;
};

std::vector<World::ComponentType> getComponentTypes(GameObject& object)
{
	std::vector<World::ComponentType> components;
	if (object.getComponent<CubeComponent>()) components.push_back(World::COMP_Cube);
	if (object.getComponent<PlaneComponent>()) components.push_back(World::COMP_Plane);
	if (object.getComponent<CapsuleComponent>()) components.push_back(World::COMP_Capsule);
	if (object.getComponent<CylinderComponent>()) components.push_back(World::COMP_Cylinder);
	if (object.getComponent<SphereComponent>()) components.push_back(World::COMP_Sphere);
	if (object.getComponent<MeshComponent>()) components.push_back(World::COMP_Mesh);
	if (object.getComponent<RigidBodyComponent>()) components.push_back(World::COMP_RigidBody);
	return components;
}

bool isValidComponentType(int value)
{
	return value >= World::COMP_Cube && value <= World::COMP_RigidBody;
}

std::string getResourcePath(const Resource* resource)
{
	return resource ? std::filesystem::path(resource->getPath()).string() : std::string{};
}

MaterialResource* getMaterial(GameObject& object)
{
	if (auto* component = object.getComponent<CubeComponent>()) return component->getMaterial();
	if (auto* component = object.getComponent<PlaneComponent>()) return component->getMaterial();
	if (auto* component = object.getComponent<CapsuleComponent>()) return component->getMaterial();
	if (auto* component = object.getComponent<CylinderComponent>()) return component->getMaterial();
	if (auto* component = object.getComponent<SphereComponent>()) return component->getMaterial();
	if (auto* component = object.getComponent<MeshComponent>()) return component->getMaterial(0);
	return nullptr;
}

void setMaterial(GameObject& object, const RefPtr<MaterialResource>& material)
{
	if (auto* component = object.getComponent<CubeComponent>()) component->setMaterial(material);
	else if (auto* component = object.getComponent<PlaneComponent>()) component->setMaterial(material);
	else if (auto* component = object.getComponent<CapsuleComponent>()) component->setMaterial(material);
	else if (auto* component = object.getComponent<CylinderComponent>()) component->setMaterial(material);
	else if (auto* component = object.getComponent<SphereComponent>()) component->setMaterial(material);
	else if (auto* component = object.getComponent<MeshComponent>()) component->setMaterial(0, material);
}

SceneSerializer::SceneSerializer(const SceneSerializerDesc& desc) : Base(desc.base), m_levelDirectory(desc.levelDirectory)
{
}

void jazzy::SceneSerializer::save(const World& world, const std::string& levelName)
{
	std::error_code filesystemError;
	std::filesystem::create_directories(m_levelDirectory, filesystemError);
	if (filesystemError)
	{
		DX3DLogError("Could not create the level directory: {}", filesystemError.message());
		return;
	}

	const auto levelPath = getLevelPath(levelName);
	std::ofstream output(levelPath);
	if (!output)
	{
		DX3DLogThrowError("Could not open level for writing: {}", levelPath.string())
		return;
	}

	std::vector<SavedObject> objects;
	auto count = 0u;
	auto transforms = world.getComponents<TransformComponent>(count);
	for (auto i  : std::views::iota(0u, count))
	{
		auto& object = transforms[i]->getGameObject();
		if (object._id == world.ROOTSCENE_ID) continue;

		SavedObject saved;
		saved.name = object.m_name;
		saved.id = object._id;
		saved.parentId = object.getParent() ? object.getParent()->_id : world.ROOTSCENE_ID;
		saved.components = getComponentTypes(object);
		if (auto* meshComponent = object.getComponent<MeshComponent>())
			saved.meshPath = getResourcePath(meshComponent->getMesh());
		if (auto* material = getMaterial(object))
		{
			saved.materialPath = getResourcePath(material);
			for (size_t textureIndex = 0; textureIndex < material->getNumTextures(); ++textureIndex)
				saved.texturePaths.push_back(getResourcePath(material->getTexture(textureIndex)));
		}
		saved.position = transforms[i]->getPosition();
		saved.rotation = transforms[i]->getRotation();
		saved.scale = transforms[i]->getScale();
		objects.push_back(std::move(saved));
	}

	output << "JAZZY_LEVEL 2\n";
	output << "objects " << objects.size() << '\n';
	output << std::setprecision(9);
	for (const auto& object : objects)
	{
		output << "object " << std::quoted(object.name) << ' ' << std::quoted(object.id) << ' '
			<< std::quoted(object.parentId) << ' ' << object.components.size();
		for (const auto component : object.components) output << ' ' << static_cast<int>(component);
		output << ' ' << std::quoted(object.meshPath) << ' ' << std::quoted(object.materialPath)
			<< ' ' << object.texturePaths.size();
		for (const auto& texturePath : object.texturePaths) output << ' ' << std::quoted(texturePath);
		output << ' ' << object.position.x << ' ' << object.position.y << ' ' << object.position.z
			<< ' ' << object.rotation.x << ' ' << object.rotation.y << ' ' << object.rotation.z
			<< ' ' << object.scale.x << ' ' << object.scale.y << ' ' << object.scale.z << '\n';
	}

	if (!output)
	{
		DX3DLogThrowError("Failed while writing level: {}", levelPath.string());
		return;
	}
}

void jazzy::SceneSerializer::load(World& world, const std::string& levelName)
{
	const auto levelPath = getLevelPath(levelName);
	std::ifstream input(levelPath);
	if (!input)
	{
		DX3DLogThrowError("Could not open level for reading: {}", levelPath.string());
		return;
	}

	std::string header;
	int version = 0;
	std::string objectLabel;
	std::size_t objectCount = 0;
	if (!(input >> header >> version) || header != "JAZZY_LEVEL" || (version != 1 && version != 2) ||
		!(input >> objectLabel >> objectCount) || objectLabel != "objects")
	{
		DX3DLogThrowError("Invalid level header: {}", levelPath.string())
		return;
	}

	std::vector<SavedObject> savedObjects;
	savedObjects.reserve(objectCount);
	for (auto i : std::views::iota(0u, objectCount))
	{
		std::string recordLabel;
		SavedObject saved;
		std::size_t componentCount = 0;
		if (!(input >> recordLabel >> std::quoted(saved.name) >> std::quoted(saved.id) >> std::quoted(saved.parentId)
			>> componentCount) || recordLabel != "object")
		{
			DX3DLogThrowError("Invalid object record in level: {}", levelPath.string())
			return;
		}

		for (auto i : std::views::iota(0u, componentCount))
		{
			int componentValue = 0;
			if (!(input >> componentValue) || !isValidComponentType(componentValue))
			{
				DX3DLogThrowError("Invalid component type in level: {}", levelPath.string())
				return;
			}
			saved.components.push_back(static_cast<World::ComponentType>(componentValue));
		}

		if (version >= 2)
		{
			std::size_t textureCount = 0;
			if (!(input >> std::quoted(saved.meshPath) >> std::quoted(saved.materialPath) >> textureCount))
			{
				DX3DLogThrowError("Invalid resource record in level: {}", levelPath.string())
				return;
			}
			for (auto i : std::views::iota(0u, textureCount))
			{
				std::string texturePath;
				if (!(input >> std::quoted(texturePath)))
				{
					DX3DLogThrowError("Invalid texture record in level: {}", levelPath.string())
					return;
				}
				saved.texturePaths.push_back(std::move(texturePath));
			}
		}

		if (!(input >> saved.position.x >> saved.position.y >> saved.position.z
			>> saved.rotation.x >> saved.rotation.y >> saved.rotation.z
			>> saved.scale.x >> saved.scale.y >> saved.scale.z))
		{
			DX3DLogThrowError("Invalid transform in level: {}", levelPath.string())
			return;
		}
		savedObjects.push_back(std::move(saved));
	}

	std::vector<std::string> sceneObjectIds;
	auto count = 0u;
	auto transforms = world.getComponents<TransformComponent>(count);
	for (auto i : std::views::iota(0u, count))
	{
		const auto& id = transforms[i]->getGameObject()._id;
		if (id != world.ROOTSCENE_ID) sceneObjectIds.push_back(id);
	}
	for (const auto& id : sceneObjectIds)
	{
		if (auto* object = world.getGameObjectByID(id)) world.deleteGameObject(object);
	}

	std::unordered_map<std::string, GameObject*> loadedObjects;
	for (const auto& saved : savedObjects)
	{
		const auto newId = world.AddGameSceneObject(saved.name, saved.components, saved.position);
		auto* object = world.getGameObjectByID(newId);
		if (!object)
		{
			DX3DLogThrowError("Failed to create object while loading: {}", saved.name)
			return;
		}
		object->getTransform().setRotation(saved.rotation);
		object->getTransform().setScale(saved.scale);
		if (auto* meshComponent = object->getComponent<MeshComponent>(); meshComponent && !saved.meshPath.empty())
			meshComponent->setMesh(world.LoadMesh(saved.meshPath));
		if (!saved.materialPath.empty())
		{
			auto material = world.LoadMaterial(saved.materialPath);
			if (material)
			{
				for (size_t textureIndex = 0; textureIndex < saved.texturePaths.size(); ++textureIndex)
				{
					auto texture = world.LoadTexture(saved.texturePaths[textureIndex]);
					if (texture) material->setTexture(textureIndex, texture);
				}
				setMaterial(*object, material);
			}
		}
		loadedObjects.emplace(saved.id, object);
	}

	for (const auto& saved : savedObjects)
	{
		auto child = loadedObjects.find(saved.id);
		if (child == loadedObjects.end()) continue;

		auto parent = loadedObjects.find(saved.parentId);
		if (parent != loadedObjects.end()) child->second->setParent(parent->second);
	}
}

const std::filesystem::path& jazzy::SceneSerializer::getLevelDirectory() const noexcept
{
	return m_levelDirectory;
}

std::filesystem::path jazzy::SceneSerializer::getLevelPath(const std::string& levelName) const noexcept
{
	auto fileName = std::filesystem::path(levelName).filename();
	if (fileName.extension() != ".lvl") fileName.replace_extension(".lvl");
	return m_levelDirectory / fileName;
}
