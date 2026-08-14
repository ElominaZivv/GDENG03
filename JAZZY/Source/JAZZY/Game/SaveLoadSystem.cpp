#include <JAZZY/SaveSystem/SaveLoadSystem.h>
#include <JAZZY/SaveSystem/json.hpp>

// all the components 
#include <JAZZY/Components/CubeComponent.h>
#include <JAZZY/Components/PlaneComponent.h>
#include <JAZZY/Components/SphereComponent.h>
#include <JAZZY/Components/CylinderComponent.h>
#include <JAZZY/Components/CapsuleComponent.h>
#include <JAZZY/Components/RigidBodyComponent.h>
#include <JAZZY/Components/TransformComponent.h>
#include <JAZZY/Components/MeshComponent.h>

#include <JAZZY/Resource/MaterialResource.h>
#include <JAZZY/Resource/TextureResource.h>
#include <JAZZY/Resource/MeshResource.h>

#include <fstream>
#include <iostream>

using std::vector;
using std::string;

using json = nlohmann::json;

jazzy::SaveLoadSystem::SaveLoadSystem(World& gameWorld) : world(gameWorld)
{
	GetAllObjectsFromWorld();
}

void jazzy::SaveLoadSystem::SaveScene(string fileName)
{

}

void jazzy::SaveLoadSystem::GetAllObjectsFromWorld()
{
	allObjects.clear();

	auto num = 0u;
	auto objs = world.getComponents<TransformComponent>(num);

	for (auto i : std::views::iota(0u, num)) {
		allObjects.push_back(&objs[i]->getGameObject());
	}
}

string jazzy::SaveLoadSystem::EncodeOneObject(int index)
{
	GameObject* obj = allObjects[index];
	auto& transform = obj->getTransform();

	json objJson;

	// misc stuff
	

	// save transform
	objJson["transform"] = {
		{"position", {transform.getPosition().x, transform.getPosition().y, transform.getPosition().z }},
		{"scale", {transform.getScale().x, transform.getScale().y, transform.getScale().z }},
		{"rotation", {transform.getRotation().x, transform.getRotation().y, transform.getRotation().z }}
	};

	// shape/mesh and texture
	if (obj->getComponent<CubeComponent>()) {
		objJson["meshType"] = "cube";
		objJson["textureFile"] = obj->getComponent<CubeComponent>()->getMaterial()->getTexture(0)->GetTextureFileName();
	} 
	else if (obj->getComponent<CapsuleComponent>()) {
		objJson["meshType"] = "capsule";
		objJson["textureFile"] = obj->getComponent<CapsuleComponent>()->getMaterial()->getTexture(0)->GetTextureFileName();
	}
	else if (obj->getComponent<CylinderComponent>()) {
		objJson["meshType"] = "cylinder";
		objJson["textureFile"] = obj->getComponent<CylinderComponent>()->getMaterial()->getTexture(0)->GetTextureFileName();
	}
	else if (obj->getComponent<PlaneComponent>()) {
		objJson["meshType"] = "plane";
		objJson["textureFile"] = obj->getComponent<PlaneComponent>()->getMaterial()->getTexture(0)->GetTextureFileName();
	}
	else if (obj->getComponent<SphereComponent>()) {
		objJson["meshType"] = "sphere";
		objJson["textureFile"] = obj->getComponent<SphereComponent>()->getMaterial()->getTexture(0)->GetTextureFileName();
	}
	else if (obj->getComponent<MeshComponent>()) {
		objJson["meshType"] = "custom";
		objJson["textureFile"] = obj->getComponent<MeshComponent>()->getMaterial(0)->getTexture(0)->GetTextureFileName();
		objJson["meshFile"] = obj->getComponent<MeshComponent>()->getMesh()->GetMeshFileName();
	}
}

jazzy::SaveLoadSystem::~SaveLoadSystem()
{
	allObjects.clear();
}
