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

    auto numPlane = 0u;
    auto planes = m_world.getComponents<PlaneComponent>(numPlane);

    auto numCube = 0u;
    auto cubes = m_world.getComponents<CubeComponent>(numCube);

    auto numSphere = 0u;
    auto spheres = m_world.getComponents<SphereComponent>(numSphere);

    auto numCapsule = 0u;
    auto capsules = m_world.getComponents<CapsuleComponent>(numCapsule);

    auto numCylinder = 0u;
    auto cylinders = m_world.getComponents<CylinderComponent>(numCylinder);

    if (ImGui::BeginViewportSideBar("Hierarchy", viewport, ImGuiDir_Left, 150.0f, windowFlags))
    {
        ImGui::Text("Game Objects");

        for (auto i : std::views::iota(0u, numPlane))
        {
            auto* object = &planes[i]->getGameObject();

            if (object->getParent() == nullptr)
                DrawObjectHierarchy(object, treeFlags);
        }

        for (auto i : std::views::iota(0u, numCube))
        {
            auto* object = &cubes[i]->getGameObject();

            if (object->getParent() == nullptr)
                DrawObjectHierarchy(object, treeFlags);
        }

        for (auto i : std::views::iota(0u, numSphere))
        {
            auto* object = &spheres[i]->getGameObject();

            if (object->getParent() == nullptr)
                DrawObjectHierarchy(object, treeFlags);
        }

        for (auto i : std::views::iota(0u, numCapsule))
        {
            auto* object = &capsules[i]->getGameObject();

            if (object->getParent() == nullptr)
                DrawObjectHierarchy(object, treeFlags);
        }

        for (auto i : std::views::iota(0u, numCylinder))
        {
            auto* object = &cylinders[i]->getGameObject();

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
        m_world.SetSelectedObject(obj->m_name);
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
                    oldParent->removeChildByName(
                        draggedObject->m_name
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
