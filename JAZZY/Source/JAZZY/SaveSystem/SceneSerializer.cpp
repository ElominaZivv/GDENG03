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
#include <memory>
#include <ranges>
#include <unordered_map>
#include <vector>

using namespace jazzy;

SceneSerializer::SceneSerializer(const SceneSerializerDesc& desc)
	: Base(desc.base), m_levelDirectory(desc.levelDirectory)
{
}

void SceneSerializer::save(const World& world, const std::string& levelName)
{
	std::error_code filesystemError;
	std::filesystem::create_directories(m_levelDirectory, filesystemError);
	if (filesystemError)
	{
		DX3DLogError("Could not create the level directory: {}", filesystemError.message());
		return;
	}

	Json::Value level(Json::objectValue);
	level["format"] = LevelFormat;
	level["version"] = LevelVersion;
	level["objects"] = Json::Value(Json::arrayValue);

	auto count = 0u;
	auto transforms = world.getComponents<TransformComponent>(count);
	for (auto index : std::views::iota(0u, count))
	{
		auto& object = transforms[index]->getGameObject();
		if (object._id == world.ROOTSCENE_ID) continue;

		Json::Value saved(Json::objectValue);
		saved["name"] = object.m_name;
		saved["id"] = object._id;
		saved["parentId"] = object.getParent() ? object.getParent()->_id : world.ROOTSCENE_ID;
		saved["components"] = Json::Value(Json::arrayValue);
		for (const auto component : getComponentTypes(object))
			saved["components"].append(componentTypeToString(component));

		saved["transform"]["position"] = vec3ToJson(transforms[index]->getPosition());
		saved["transform"]["rotation"] = vec3ToJson(transforms[index]->getRotation());
		saved["transform"]["scale"] = vec3ToJson(transforms[index]->getScale());

		saved["resources"]["mesh"] = "";
		saved["resources"]["material"]["shader"] = "";
		saved["resources"]["material"]["textures"] = Json::Value(Json::arrayValue);
		if (auto* meshComponent = object.getComponent<MeshComponent>())
			saved["resources"]["mesh"] = getResourcePath(meshComponent->getMesh());
		if (auto* material = getMaterial(object))
		{
			saved["resources"]["material"]["shader"] = getResourcePath(material);
			for (size_t textureIndex = 0; textureIndex < material->getNumTextures(); ++textureIndex)
				saved["resources"]["material"]["textures"].append(getResourcePath(material->getTexture(textureIndex)));
		}

		level["objects"].append(std::move(saved));
	}

	std::ofstream output(getLevelPath(levelName));
	if (!output)
	{
		DX3DLogThrowError("Could not open level for writing: {}", getLevelPath(levelName).string());
		return;
	}

	Json::StreamWriterBuilder writerBuilder;
	writerBuilder["indentation"] = "  ";
	std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
	writer->write(level, &output);
	if (!output) DX3DLogThrowError("Failed while writing level: {}", getLevelPath(levelName).string());
}

void SceneSerializer::load(World& world, const std::string& levelName)
{
	const auto levelPath = getLevelPath(levelName);
	std::ifstream input(levelPath);
	if (!input)
	{
		DX3DLogThrowError("Could not open level for reading: {}", levelPath.string());
		return;
	}

	Json::CharReaderBuilder readerBuilder;
	Json::Value level;
	std::string errors;
	if (!Json::parseFromStream(readerBuilder, input, &level, &errors))
	{
		DX3DLogThrowError("Could not parse JSON level {}. {}", levelPath.string(), errors);
		return;
	}

	if (!level.isObject() || level["format"].asString() != LevelFormat ||
		level["version"].asInt() != LevelVersion || !level["objects"].isArray())
	{
		DX3DLogThrowError("Invalid JSON level format: {}", levelPath.string());
		return;
	}

	std::vector<SavedObject> savedObjects;
	savedObjects.reserve(level["objects"].size());
	for (const auto& record : level["objects"])
	{
		if (!record.isObject() || !record["name"].isString() || !record["id"].isString() ||
			!record["parentId"].isString() || !record["components"].isArray() ||
			!record["transform"].isObject())
		{
			DX3DLogThrowError("Invalid object record in level: {}", levelPath.string());
			return;
		}

		SavedObject saved;
		saved.name = record["name"].asString();
		saved.id = record["id"].asString();
		saved.parentId = record["parentId"].asString();
		for (const auto& componentName : record["components"])
		{
			ComponentType component;
			if (!componentName.isString() || !componentTypeFromString(componentName.asString(), component))
			{
				DX3DLogThrowError("Unknown component in level: {}", levelPath.string());
				return;
			}
			saved.components.push_back(component);
		}

		const auto& transform = record["transform"];
		if (!jsonToVec3(transform["position"], saved.position) ||
			!jsonToVec3(transform["rotation"], saved.rotation) ||
			!jsonToVec3(transform["scale"], saved.scale))
		{
			DX3DLogThrowError("Invalid transform in level: {}", levelPath.string());
			return;
		}

		const auto& resources = record["resources"];
		if (resources.isObject())
		{
			if (resources["mesh"].isString()) saved.meshPath = resources["mesh"].asString();
			const auto& material = resources["material"];
			if (material.isObject())
			{
				if (material["shader"].isString()) saved.materialPath = material["shader"].asString();
				if (material["textures"].isArray())
				{
					for (const auto& texture : material["textures"])
						if (texture.isString()) saved.texturePaths.push_back(texture.asString());
				}
			}
		}
		savedObjects.push_back(std::move(saved));
	}

	std::vector<std::string> sceneObjectIds;
	auto count = 0u;
	auto transforms = world.getComponents<TransformComponent>(count);
	for (auto index : std::views::iota(0u, count))
	{
		const auto& id = transforms[index]->getGameObject()._id;
		if (id != world.ROOTSCENE_ID) sceneObjectIds.push_back(id);
	}
	for (const auto& id : sceneObjectIds)
		if (auto* object = world.getGameObjectByID(id)) world.deleteGameObject(object);

	std::unordered_map<std::string, GameObject*> loadedObjects;
	for (const auto& saved : savedObjects)
	{
		const auto newId = world.AddGameSceneObject(saved.name, saved.components, saved.position);
		auto* object = world.getGameObjectByID(newId);
		if (!object)
		{
			DX3DLogThrowError("Failed to create object while loading: {}", saved.name);
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
		auto parent = loadedObjects.find(saved.parentId);
		if (child != loadedObjects.end() && parent != loadedObjects.end()) child->second->setParent(parent->second);
	}
}

const std::filesystem::path& SceneSerializer::getLevelDirectory() const noexcept
{
	return m_levelDirectory;
}

const char* SceneSerializer::componentTypeToString(ComponentType component)
{
	switch (component)
	{
	case ComponentType::COMP_Cube: return "Cube";
	case ComponentType::COMP_Plane: return "Plane";
	case ComponentType::COMP_Capsule: return "Capsule";
	case ComponentType::COMP_Cylinder: return "Cylinder";
	case ComponentType::COMP_Sphere: return "Sphere";
	case ComponentType::COMP_Mesh: return "Mesh";
	case ComponentType::COMP_RigidBody: return "RigidBody";
	}
	return "Unknown";
}

bool SceneSerializer::componentTypeFromString(const std::string& name, ComponentType& component)
{
	if (name == "Cube") component = ComponentType::COMP_Cube;
	else if (name == "Plane") component = ComponentType::COMP_Plane;
	else if (name == "Capsule") component = ComponentType::COMP_Capsule;
	else if (name == "Cylinder") component = ComponentType::COMP_Cylinder;
	else if (name == "Sphere") component = ComponentType::COMP_Sphere;
	else if (name == "Mesh") component = ComponentType::COMP_Mesh;
	else if (name == "RigidBody") component = ComponentType::COMP_RigidBody;
	else return false;
	return true;
}

std::vector<ComponentType> SceneSerializer::getComponentTypes(GameObject& object)
{
	std::vector<ComponentType> components;
	if (object.getComponent<CubeComponent>()) components.push_back(ComponentType::COMP_Cube);
	if (object.getComponent<PlaneComponent>()) components.push_back(ComponentType::COMP_Plane);
	if (object.getComponent<CapsuleComponent>()) components.push_back(ComponentType::COMP_Capsule);
	if (object.getComponent<CylinderComponent>()) components.push_back(ComponentType::COMP_Cylinder);
	if (object.getComponent<SphereComponent>()) components.push_back(ComponentType::COMP_Sphere);
	if (object.getComponent<MeshComponent>()) components.push_back(ComponentType::COMP_Mesh);
	if (object.getComponent<RigidBodyComponent>()) components.push_back(ComponentType::COMP_RigidBody);
	return components;
}

std::string SceneSerializer::getResourcePath(const Resource* resource)
{
	return resource ? std::filesystem::path(resource->getPath()).string() : std::string{};
}

MaterialResource* SceneSerializer::getMaterial(GameObject& object)
{
	if (auto* component = object.getComponent<CubeComponent>()) return component->getMaterial();
	if (auto* component = object.getComponent<PlaneComponent>()) return component->getMaterial();
	if (auto* component = object.getComponent<CapsuleComponent>()) return component->getMaterial();
	if (auto* component = object.getComponent<CylinderComponent>()) return component->getMaterial();
	if (auto* component = object.getComponent<SphereComponent>()) return component->getMaterial();
	if (auto* component = object.getComponent<MeshComponent>()) return component->getMaterial(0);
	return nullptr;
}

void SceneSerializer::setMaterial(GameObject& object, const RefPtr<MaterialResource>& material)
{
	if (auto* component = object.getComponent<CubeComponent>()) component->setMaterial(material);
	else if (auto* component = object.getComponent<PlaneComponent>()) component->setMaterial(material);
	else if (auto* component = object.getComponent<CapsuleComponent>()) component->setMaterial(material);
	else if (auto* component = object.getComponent<CylinderComponent>()) component->setMaterial(material);
	else if (auto* component = object.getComponent<SphereComponent>()) component->setMaterial(material);
	else if (auto* component = object.getComponent<MeshComponent>()) component->setMaterial(0, material);
}

Json::Value SceneSerializer::vec3ToJson(const Vec3& vector)
{
	Json::Value value(Json::arrayValue);
	value.append(vector.x);
	value.append(vector.y);
	value.append(vector.z);
	return value;
}

bool SceneSerializer::jsonToVec3(const Json::Value& value, Vec3& vector)
{
	if (!value.isArray() || value.size() != 3 ||
		!value[0].isNumeric() || !value[1].isNumeric() || !value[2].isNumeric()) return false;
	vector = { value[0].asFloat(), value[1].asFloat(), value[2].asFloat() };
	return true;
}

std::filesystem::path SceneSerializer::getLevelPath(const std::string& levelName) const noexcept
{
	auto fileName = std::filesystem::path(levelName).filename();
	if (fileName.extension() != ".lvl") fileName.replace_extension(".lvl");
	return m_levelDirectory / fileName;
}
