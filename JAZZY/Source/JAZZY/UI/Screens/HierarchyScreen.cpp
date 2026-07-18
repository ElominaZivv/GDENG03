#include <JAZZY/UI/Screens/HierarchyScreen.h>
#include <JAZZY/IMGUI/imgui.h>
#include <JAZZY/IMGUI/imgui_internal.h>
#include <JAZZY/Components/CubeComponent.h>
#include <JAZZY/Game/World.h>
#include <JAZZY/Game/GameObject.h>
#include <ranges>
#include <iostream>


jazzy::HierarchyScreen::HierarchyScreen(World & world)
    : Screens("Hierarchy"), m_world(world)
{
}

void jazzy::HierarchyScreen::draw()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoNavFocus;

    ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow |                   ImGuiTreeNodeFlags_OpenOnDoubleClick |
      ImGuiTreeNodeFlags_DefaultOpen;

    auto numComp = 0u;
    auto cubes = m_world.getComponents<CubeComponent>(numComp);

    for (auto i : std::views::iota(0u, numComp)) {
        if (cubes[i]->getGameObject().isSelected) m_world.SetSelectedObject(i);
    }

    if (ImGui::BeginViewportSideBar("Hierarchy", viewport, ImGuiDir_Left, 150.0f, windowFlags)) {

        

        ImGui::Text("Game Objects");
        
        DrawObjectHierarchy(cubes[0], treeFlags);
        
        

        ImGui::End();
    }
}

void jazzy::HierarchyScreen::DrawObjectHierarchy(CubeComponent* obj, ImGuiTreeNodeFlags treeFlags)
{

    static int selectedNode = m_world.GetSelectedIndex();
    if (obj->getGameObject().isSelected) treeFlags |= ImGuiTreeNodeFlags_Selected;

    bool isOpen = ImGui::TreeNodeEx("ROOT", treeFlags, "World Plane");

    if (ImGui::IsItemClicked())
    {
        auto numComp = 0u;
        auto cubes = m_world.getComponents<CubeComponent>(numComp);

        for (auto i : std::views::iota(0u, numComp)) {
            if (cubes[i]->getGameObject().isSelected) cubes[i]->getGameObject().isSelected = false;
        }

        obj->getGameObject().isSelected = true;
    }

    if (isOpen)
    {
        for (int i = 0; i < obj->getGameObject().getChildCount(); i++) {
            auto child = obj->getGameObject().getChildByIndex(i)->createOrGetComponent<CubeComponent>();
            DrawObjectHierarchy(child, treeFlags);
        }
        ImGui::Text("Cube");
        ImGui::TreePop();
    }
}
