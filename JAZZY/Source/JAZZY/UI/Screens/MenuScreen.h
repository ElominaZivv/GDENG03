#pragma once

#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/UI/Screens/Screens.h>

namespace jazzy
{
    class MenuScreen : public Screens
    {
    public:
       MenuScreen(World& world, ResourceManager& resource, SaveLoadSystem& saveSystem);

        void draw() override;

    private:
        World& m_world;
        ResourceManager& m_resource;
        SaveLoadSystem& m_saveSystem;

        int m_planeCount = 1;
        int m_cubeCount = 0;
        int m_sphereCount = 0;
        int m_capsuleCount = 0;
        int m_cylinderCount = 0;
        int customMeshCount = 0;

        std::string meshFileName = "./Game/Assets/Meshes/";
    };

}