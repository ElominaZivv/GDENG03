#pragma once

#include <JAZZY/SaveSystem/json.hpp>
#include "JAZZY/Game/World.h"
#include "JAZZY/Game/GameObject.h"

#include <iostream>
#include <vector>

using std::vector;
using std::string;

namespace jazzy {

class SaveLoadSystem
{
	public:
		SaveLoadSystem(World& gameWorld);

		// main funcs
		void SaveScene(string fileName);
		void LoadScene();

		// helpers :P
		void GetAllObjectsFromWorld();
		string EncodeOneObject(int index);

		~SaveLoadSystem();

	private:
		World& world;
		vector<GameObject*> allObjects;
};

}
