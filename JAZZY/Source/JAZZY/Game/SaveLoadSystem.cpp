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
#include "JAZZY/Resource/ResourceManager.h"

#include <fstream>
#include <iostream>
#include <vector>

using std::vector;
using std::string;

using json = nlohmann::json;

jazzy::SaveLoadSystem::SaveLoadSystem(World& gameWorld, ResourceManager& resourceManager) : world(gameWorld), resMan(resourceManager)
{
	GetAllObjectsFromWorld();
}

void jazzy::SaveLoadSystem::SaveScene(string fileName)
{
	string fullFileName = folderPath + fileName;

	if (allObjects.empty()) {
		GetAllObjectsFromWorld();
		if (allObjects.empty())
			return;
	}

	std::ofstream saveFile(fullFileName);

	if (!saveFile.is_open()) {
		std::cout << "Failed to open save file." << std::endl;
		return;
	}
	
	saveFile << "[\n";

	for (int i = 0; i < allObjects.size(); i++) {
		saveFile << EncodeOneObject(i).dump(4);
		if (i == allObjects.size() - 1) saveFile << "\n";
		else saveFile << ",\n";
	}

	saveFile << "]";

	saveFile.close();
}

void jazzy::SaveLoadSystem::LoadScene(string fileName)
{
	string fullFileName = folderPath + fileName;

	
	std::ifstream saveFile(fullFileName);

	if (!saveFile.is_open()) {
		std::cout << "Failed to open save file." << std::endl;
		return;
	}

	json j = json::parse(saveFile);
	for (auto it = j.begin(); it != j.end(); ++it)
	{
		allJson.push_back(it.value());
	}

	for (int i = 0; i < allJson.size(); i++) {
		LoadOneObject(allJson[i]);
	}

	saveFile.close();
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

nlohmann::json jazzy::SaveLoadSystem::EncodeOneObject(int index)
{
	GameObject* obj = allObjects[index];
	auto& transform = obj->getTransform();

	json objJson;

	std::cout << "JSON Object [" + obj->_id + "] made.\n";

	// misc stuff
	objJson["_id"] = obj->_id;
	objJson["_name"] = obj->m_name;

	if (obj->_id != world.ROOTSCENE_ID) {
		objJson["isHidden"] = transform.getHidden();
		objJson["isHiddenByParent"] = transform.getHiddenByParent();
		objJson["parentID"] = obj->getParent()->_id;
	}
	std::cout << "[" + obj->_id + "] misc details saved.\n";

	// save transform
	objJson["transform"] = {
		{"position", {transform.getPosition().x, transform.getPosition().y, transform.getPosition().z }},
		{"scale", {transform.getScale().x, transform.getScale().y, transform.getScale().z }},
		{"rotation", {transform.getRotation().x, transform.getRotation().y, transform.getRotation().z }}
	};

	if (obj->_id == world.ROOTSCENE_ID) {
		std::cout << "[" + obj->_id + "] Root saved.\n";

		return objJson;
	}

	std::cout << "[" + obj->_id + "] transform details saved.\n";

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

	std::cout << "[" + obj->_id + "] mesh details saved.\n";

	// rigid body
	if (obj->getComponent<RigidBodyComponent>()) {
		objJson["rigidBodyActive"] = true;
		objJson["rigidBodyType"] = obj->getComponent<RigidBodyComponent>()->GetRigidBodyType();
		objJson["colliderDimensions"] = {
			obj->getComponent<RigidBodyComponent>()->getColliderDimensions().x,
			obj->getComponent<RigidBodyComponent>()->getColliderDimensions().y,
			obj->getComponent<RigidBodyComponent>()->getColliderDimensions().z
		};
	}

	return objJson;
}

void jazzy::SaveLoadSystem::LoadOneObject(nlohmann::json objJson)
{
	if (static_cast<string>(objJson["_id"]) == world.ROOTSCENE_ID) {
		return;
	}

	string objName = objJson["_name"];
	vector<World::ComponentType> comps{};
	
	if (static_cast<bool>(objJson["rigidBodyActive"]))
		comps.push_back(World::COMP_RigidBody);

	if (static_cast<string>(objJson["meshType"]) == "cube") {
		comps.push_back(World::COMP_Cube);
	} 
	else if(static_cast<string>(objJson["meshType"]) == "capsule") {
		comps.push_back(World::COMP_Capsule);
	}
	else if (static_cast<string>(objJson["meshType"]) == "cylinder") {
		comps.push_back(World::COMP_Cylinder);
	}
	else if (static_cast<string>(objJson["meshType"]) == "plane") {
		comps.push_back(World::COMP_Plane);
	}
	else if (static_cast<string>(objJson["meshType"]) == "sphere") {
		comps.push_back(World::COMP_Sphere);
	}
	else if (static_cast<string>(objJson["meshType"]) == "custom") {
		comps.push_back(World::COMP_Mesh);
	}

	string objId = world.AddGameSceneObject(objName, comps);
	GameObject* thisObj = world.getGameObjectByID(objId);

	if (static_cast<string>(objJson["parentID"]) != world.ROOTSCENE_ID) {

		world.getGameObjectByID(world.ROOTSCENE_ID)->removeChildById(objId);

		GameObject* parent = world.getGameObjectByID(static_cast<string>(objJson["parentID"]));

		thisObj->setParent(parent);
	}

	// transform
	auto& transform = thisObj->getTransform();
	transform.setPosition(
		Vec3(
			static_cast<float>(objJson["transform"]["position"][0]),
			static_cast<float>(objJson["transform"]["position"][1]),
			static_cast<float>(objJson["transform"]["position"][2])
		)
	);
	transform.setRotation(
		Vec3(
			static_cast<float>(objJson["transform"]["rotation"][0]),
			static_cast<float>(objJson["transform"]["rotation"][1]),
			static_cast<float>(objJson["transform"]["rotation"][2])
		)
	);
	transform.setScale(
		Vec3(
			static_cast<float>(objJson["transform"]["scale"][0]),
			static_cast<float>(objJson["transform"]["scale"][1]),
			static_cast<float>(objJson["transform"]["scale"][2])
		)
	);

	// rigidbody 
	auto rb = thisObj->getComponent<RigidBodyComponent>();
	rb->setBodyType(static_cast<BodyType>(objJson["rigidBodyType"]));
	rb->addBoxCollider(
		Vec3(
			static_cast<float>(objJson["colliderDimensions"][0]),
			static_cast<float>(objJson["colliderDimensions"][1]),
			static_cast<float>(objJson["colliderDimensions"][2])
		), {0.0f, 0.0f, 0.0f}
	);

	// textures and mats
	string texFileName = static_cast<string>(objJson["textureFile"]);
	string textFilepath = texFolderPath + texFileName;

	std::wstring wideStr(textFilepath.begin(), textFilepath.end());
	const wchar_t* texFullFilePath = wideStr.c_str();

	auto textureRes = resMan.createResourceFromFile<jazzy::TextureResource>(texFullFilePath);
	auto matRes = resMan.createResourceFromFile<jazzy::MaterialResource>(L"./Game/Assets/Shaders/BasicShader.hlsl");
	matRes->setTexture(0, textureRes);

	if (auto objComp = thisObj->getComponent<jazzy::CubeComponent>()) {
		objComp->setMaterial(matRes);
	}
	else if (auto objComp = thisObj->getComponent<jazzy::CapsuleComponent>()) {
		objComp->setMaterial(matRes);
	}
	else if (auto objComp = thisObj->getComponent<jazzy::CylinderComponent>()) {
		objComp->setMaterial(matRes);
	}
	else if (auto objComp = thisObj->getComponent<jazzy::PlaneComponent>()) {
		objComp->setMaterial(matRes);
	}
	else if (auto objComp = thisObj->getComponent<jazzy::SphereComponent>()) {
		objComp->setMaterial(matRes);
	}
	else if (auto objComp = thisObj->getComponent<jazzy::MeshComponent>()) {
		string meshFileName = static_cast<string>(objJson["meshFile"]);
		auto mesh = world.LoadMesh(meshFileName);

		objComp->setMesh(mesh);
		objComp->setMaterial(0, matRes);
	}
}

jazzy::SaveLoadSystem::~SaveLoadSystem()
{
	allObjects.clear();
}
