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

    

    if (ImGui::BeginViewportSideBar("Hierarchy", viewport, ImGuiDir_Left, 150.0f, windowFlags)) {

        

        ImGui::Text("Game Objects");
        
        DrawObjectHierarchy(cubes[0], treeFlags);
        
        

        ImGui::End();
    }
}

void jazzy::HierarchyScreen::DrawObjectHierarchy(CubeComponent* obj, ImGuiTreeNodeFlags treeFlags)
{
    auto numComp = 0u;
    auto cubes = m_world.getComponents<CubeComponent>(numComp);

    static int selectedNode = m_world.GetSelectedIndex();
    if (obj->getGameObject().isSelected) treeFlags |= ImGuiTreeNodeFlags_Selected;
    if (obj->getGameObject().getChildCount() == 0) treeFlags |= ImGuiTreeNodeFlags_Leaf;

    bool isOpen = ImGui::TreeNodeEx(obj->getGameObject().m_name.c_str(), treeFlags);

    if (ImGui::IsItemClicked())
    {
        for (auto i : std::views::iota(0u, numComp)) {
            if (cubes[i]->getGameObject().isSelected) cubes[i]->getGameObject().isSelected = false;
        }

        obj->getGameObject().isSelected = true;

        for (auto i : std::views::iota(0u, numComp)) {
            if (cubes[i]->getGameObject().isSelected) {
                m_world.SetSelectedObject(i);
                std::cout << "selected: " << i << std::endl;
            }
        }
    }

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        ImGui::SetDragDropPayload("PARENTING_PAYLOAD", obj->getGameObject().m_name.c_str(), sizeof(CubeComponent*));

        ImGui::Text("Assign Parent");
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        // Accept the specific payload type
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PARENTING_PAYLOAD"))
        {
            IM_ASSERT(payload->DataSize == sizeof(CubeComponent*));
            
            const char* selected = (const char*)payload->Data;

            for (auto i : std::views::iota(0u, numComp)) {
                if (cubes[i]->getGameObject().m_name == selected && selected != obj->getGameObject().m_name) {
                    cubes[i]->getGameObject().setParent(&obj->getGameObject());
                }
            }
            std::cout << "attempted to parent" << std::endl;
        }
        ImGui::EndDragDropTarget();
    }

    if (isOpen)
    {
        for (int i = 0; i < obj->getGameObject().getChildCount(); i++) {
            auto child = obj->getGameObject().getChildByIndex(i)->createOrGetComponent<CubeComponent>();
            DrawObjectHierarchy(child, treeFlags);
        }

        ImGui::TreePop();
    }
}
