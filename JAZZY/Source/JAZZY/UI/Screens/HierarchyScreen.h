#pragma once

#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/IMGUI/imgui.h>
#include <JAZZY/Game/GameObject.h>
#include <JAZZY/UI/Screens/Screens.h>



namespace jazzy
{
    class HierarchyScreen : public Screens
    {
    public:
        HierarchyScreen(World& world);

        void draw() override;

        void DrawObjectHierarchy(GameObject* obj, ImGuiTreeNodeFlags treeFlags);
    private:
        World& m_world;
        int m_cubeCount = 1;
    };

}


