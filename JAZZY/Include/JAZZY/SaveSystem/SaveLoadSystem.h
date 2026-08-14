#pragma once

#include <JAZZY/SaveSystem/json.hpp>
#include "JAZZY/Game/World.h"
#include "JAZZY/Resource/ResourceManager.h"
#include "JAZZY/Game/GameObject.h"

#include <iostream>
#include <vector>

using std::vector;
using std::string;

namespace jazzy {

class SaveLoadSystem
{
	public:
		SaveLoadSystem(World& gameWorld, ResourceManager& resourceManager);

		// main funcs
		void SaveScene(string fileName);
		void LoadScene(string fileName);

		// helpers :P
		void GetAllObjectsFromWorld();
		nlohmann::json EncodeOneObject(int index);
		void LoadOneObject(nlohmann::json objJson);

		~SaveLoadSystem();

	private:
		World& world;
		ResourceManager& resMan;
		vector<GameObject*> allObjects;
		vector<nlohmann::json> allJson;

		string folderPath = "./Game/Saves/";
		string texFolderPath = "./Game/Assets/Textures/";
};

}
