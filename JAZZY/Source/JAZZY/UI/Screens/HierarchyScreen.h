#pragma once

#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/UI/Screens/Screens.h>

namespace jazzy
{
    class HierarchyScreen : public Screens
    {
    public:
        HierarchyScreen(World& world);

        void draw() override;

    private:
        World& m_world;
        int m_cubeCount = 1;
    };

}


