#include <JAZZY/UI/Screens/HierarchyScreen.h>
#include <JAZZY/IMGUI/imgui.h>
#include <JAZZY/IMGUI/imgui_internal.h>
#include <JAZZY/Components/CubeComponent.h>
#include <JAZZY/Components/PlaneComponent.h>
#include <JAZZY/Components/SphereComponent.h>
#include <JAZZY/Components/CylinderComponent.h>
#include <JAZZY/Components/CapsuleComponent.h>
#include <JAZZY/Game/World.h>
#include <JAZZY/Game/GameObject.h>
#include <ranges>
#include <iostream>
#include "DebugConsole.h"


jazzy::HierarchyScreen::HierarchyScreen(World & world)
    : Screens("Hierarchy"), m_world(world)
{
}

void jazzy::HierarchyScreen::draw()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoNavFocus;

    ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
        ImGuiTreeNodeFlags_DefaultOpen;

    auto numObj = 0u;
    auto allObjs = m_world.getComponents<TransformComponent>(numObj);

    if (ImGui::BeginViewportSideBar("Hierarchy", viewport, ImGuiDir_Left, 150.0f, windowFlags))
    {
        ImGui::Text("Game Objects");

        for (auto i : std::views::iota(0u, numObj))
        {
            auto* object = &allObjs[i]->getGameObject();

            if (object->getParent() == nullptr)
                DrawObjectHierarchy(object, treeFlags);
        }

        ImGui::End();
    }
}

void jazzy::HierarchyScreen::DrawObjectHierarchy(GameObject* obj, ImGuiTreeNodeFlags treeFlags)
{
    if (obj->isSelected)
        treeFlags |= ImGuiTreeNodeFlags_Selected;

    if (obj->getChildCount() == 0)
        treeFlags |= ImGuiTreeNodeFlags_Leaf;

    bool isOpen = ImGui::TreeNodeEx(
        obj->m_name.c_str(),
        treeFlags
    );

    if (ImGui::IsItemClicked())
    {
        m_world.SetSelectedObject(obj->_id);
    }

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        GameObject* draggedObject = obj;

        ImGui::SetDragDropPayload("PARENTING_PAYLOAD", &draggedObject, sizeof(GameObject*));

        ImGui::Text("Assign Parent");
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload =
            ImGui::AcceptDragDropPayload("PARENTING_PAYLOAD"))
        {
            IM_ASSERT(payload->DataSize == sizeof(GameObject*));

            GameObject* draggedObject =
                *(GameObject**)payload->Data;

            if (draggedObject && draggedObject != obj && draggedObject->getParent() != obj)
            {
                if (GameObject* oldParent = draggedObject->getParent())
                {
                    oldParent->removeChildById(
                        draggedObject->_id
                    );
                }
                draggedObject->setParent(obj);
            }
        }

        ImGui::EndDragDropTarget();
    }

    if (isOpen)
    {
        for (ui32 i = 0; i < obj->getChildCount(); ++i)
        {
            GameObject* child = obj->getChildByIndex(i);
            DrawObjectHierarchy(child, treeFlags);
        }

        ImGui::TreePop();
    }
}
