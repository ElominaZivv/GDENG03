#pragma once

#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/UI/Screens/Screens.h>

namespace jazzy
{
    class MenuScreen : public Screens
    {
    public:
       MenuScreen(World& world, ResourceManager& resource);

        void draw() override;

    private:
        World& m_world;
        ResourceManager& m_resource;
        int m_planeCount = 1;
        int m_cubeCount = 0;
        int m_sphereCount = 0;
        int m_capsuleCount = 0;
        int m_cylinderCount = 0;
        int customMeshCount = 0;

        std::string meshFileName = "./Game/Assets/Meshes/";
        bool m_showSaveSceneDialog = false;
        char m_levelName[100] = "Untitled";
        std::string m_saveSceneStatus;
        bool m_saveSceneSucceeded = false;
        bool m_showLoadSceneDialog = false;
        char m_loadLevelName[100] = "Untitled";
        std::string m_loadSceneStatus;
        bool m_loadSceneSucceeded = false;
    };

}
