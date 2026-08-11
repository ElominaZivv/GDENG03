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
        int m_cubeCount = 1;
        int m_sphereCount = 1;
        int m_capsuleCount = 1;
        int m_cylinderCount = 1;
    };

}