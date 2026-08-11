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
    auto numComp = 0u;
    auto shape = m_world.getComponents<CubeComponent>(numComp);

    static int selectedNode = m_world.GetSelectedIndex();
    if (obj->isSelected) treeFlags |= ImGuiTreeNodeFlags_Selected;
    if (obj->getChildCount() == 0) treeFlags |= ImGuiTreeNodeFlags_Leaf;

    bool isOpen = ImGui::TreeNodeEx(obj->m_name.c_str(), treeFlags);

    if (ImGui::IsItemClicked())
    {
        m_world.SetSelectedObject(obj->m_name);
    }

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        ImGui::SetDragDropPayload("PARENTING_PAYLOAD", obj->m_name.c_str(), sizeof(CubeComponent*));

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
                if (shape[i]->getGameObject().m_name == selected && selected != obj->m_name) {
                    shape[i]->getGameObject().getParent()->removeChildByName(shape[i]->getGameObject().m_name);
                    shape[i]->getGameObject().setParent(nullptr);
                    shape[i]->getGameObject().setParent(obj);
                }
            }
            std::cout << "attempted to parent" << std::endl;
        }
        ImGui::EndDragDropTarget();
    }

    if (isOpen)
    {
        for (int i = 0; i < obj->getChildCount(); i++) {
            auto child = obj->getChildByIndex(i)->createOrGetComponent<CubeComponent>();
            DrawObjectHierarchy(&child->getGameObject(), treeFlags);
        }

        ImGui::TreePop();
    }
}
