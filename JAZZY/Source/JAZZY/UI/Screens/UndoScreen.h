#pragma once

#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/UI/Screens/Screens.h>

namespace jazzy
{
    class UndoScreen : public Screens
    {
    public:
        UndoScreen(World& world);

        void draw() override;

    private:
        World& m_world;
    };

}