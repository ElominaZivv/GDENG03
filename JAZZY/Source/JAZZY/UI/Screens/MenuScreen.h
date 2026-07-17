#pragma once

#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/UI/Screens/Screens.h>

namespace jazzy
{
    class MenuScreen : public Screens
    {
    public:
       MenuScreen(World& world);

        void draw() override;

    private:
        World& m_world;
        int m_cubeCount = 1;
    };

}