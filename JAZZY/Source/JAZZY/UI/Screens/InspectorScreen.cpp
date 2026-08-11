#include <JAZZY/UI/Screens/InspectorScreen.h>
#include <JAZZY/IMGUI/imgui.h>
#include <JAZZY/IMGUI/imgui_internal.h>
#include <JAZZY/Components/CubeComponent.h>
#include <JAZZY/Components/PlaneComponent.h>
#include <JAZZY/Components/SphereComponent.h>
#include <JAZZY/Components/CylinderComponent.h>
#include <JAZZY/Components/CapsuleComponent.h>
#include <JAZZY/Game/World.h>
#include <JAZZY/Game/GameObject.h>
#include <JAZZY/Components/MeshComponent.h>
#include <JAZZY/Resource/MaterialResource.h>
#include <JAZZY/Resource/ResourceManager.h>
#include <iostream>

jazzy::InspectorScreen::InspectorScreen(World& world, ResourceManager& resource)
    : Screens("Inspector"), m_world(world), m_resource(resource)
{
}

void jazzy::InspectorScreen::draw()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoSavedSettings |
                                   ImGuiWindowFlags_NoNavFocus;

    GameObject* selectedObject = nullptr;
    std::string componentType;

    if (m_world.getSelectedObjectType() == "Cube")
    {
        auto num = 0u;
        auto objects = m_world.getComponents<CubeComponent>(num);

        if (m_world.GetSelectedIndex() < num)
            selectedObject = &objects[m_world.GetSelectedIndex()]->getGameObject();
    }
    else if (m_world.getSelectedObjectType() == "Plane")
    {
        auto num = 0u;
        auto objects = m_world.getComponents<PlaneComponent>(num);

        if (m_world.GetSelectedIndex() < num)
            selectedObject = &objects[m_world.GetSelectedIndex()]->getGameObject();
    }
    else if (m_world.getSelectedObjectType() == "Sphere")
    {
        auto num = 0u;
        auto objects = m_world.getComponents<SphereComponent>(num);

        if (m_world.GetSelectedIndex() < num)
            selectedObject = &objects[m_world.GetSelectedIndex()]->getGameObject();
    }
    else if (m_world.getSelectedObjectType() == "Capsule")
    {
        auto num = 0u;
        auto objects = m_world.getComponents<CapsuleComponent>(num);

        if (m_world.GetSelectedIndex() < num)
            selectedObject = &objects[m_world.GetSelectedIndex()]->getGameObject();
    }
    else if (m_world.getSelectedObjectType() == "Cylinder")
    {
        auto num = 0u;
        auto objects = m_world.getComponents<CylinderComponent>(num);

        if (m_world.GetSelectedIndex() < num)
            selectedObject = &objects[m_world.GetSelectedIndex()]->getGameObject();
    }

    if (selectedObject == nullptr)
    {
        if (ImGui::BeginViewportSideBar("Inspector", viewport, ImGuiDir_Right, 200.0f, windowFlags))
        {
            ImGui::Text("Nothing selected");
            ImGui::End();
        }

        return;
    }

    auto& transform = selectedObject->getTransform();

    float position[3] = {transform.getPosition().x, transform.getPosition().y, transform.getPosition().z };
    float scale[3] = { transform.getScale().x, transform.getScale().y, transform.getScale().z };
    float rotation[3] = { transform.getRotation().x, transform.getRotation().y, transform.getRotation().z };

    if (ImGui::BeginViewportSideBar("Inspector", viewport, ImGuiDir_Right, 200.0f, windowFlags)) {

        ImGui::Text(selectedObject->m_name.c_str());

        bool visible = true;

        if (m_world.getSelectedObjectType() == "Cube")
        {
            visible = !selectedObject->getComponent<CubeComponent>()->getHidden();
        }
        else if (m_world.getSelectedObjectType() == "Plane")
        {
            visible = !selectedObject->getComponent<PlaneComponent>()->getHidden();
        }
        else if (m_world.getSelectedObjectType() == "Sphere")
        {
            visible = !selectedObject->getComponent<SphereComponent>()->getHidden();
        }
        else if (m_world.getSelectedObjectType() == "Capsule")
        {
            visible = !selectedObject->getComponent<CapsuleComponent>()->getHidden();
        }
        else if (m_world.getSelectedObjectType() == "Cylinder")
        {
            visible = !selectedObject->getComponent<CylinderComponent>()->getHidden();
        }


        if (ImGui::Checkbox("Visible", &visible))
        {
            bool hidden = !visible;

            if (m_world.getSelectedObjectType() == "Cube")
            {
                selectedObject->getComponent<CubeComponent>()->setHidden(hidden);
            }
            else if (m_world.getSelectedObjectType() == "Plane")
            {
                selectedObject->getComponent<PlaneComponent>()->setHidden(hidden);
            }
            else if (m_world.getSelectedObjectType() == "Sphere")
            {
                selectedObject->getComponent<SphereComponent>()->setHidden(hidden);
            }
            else if (m_world.getSelectedObjectType() == "Capsule")
            {
                selectedObject->getComponent<CapsuleComponent>()->setHidden(hidden);
            }
            else if (m_world.getSelectedObjectType() == "Cylinder")
            {
                selectedObject->getComponent<CylinderComponent>()->setHidden(hidden);
            }

            setHiddenRecursive(selectedObject, hidden);
        }


        ImGui::Separator();
        ImGui::Text("Transform");

        if (ImGui::InputFloat3("Position", position, "%.2f")) {
            transform.setPosition({ position[0], position[1], position[2] });
        }

        if (ImGui::InputFloat3("Scale", scale, "%.2f")) {
            transform.setScale({ scale[0], scale[1], scale[2] });
        }

        if (ImGui::InputFloat3("Rotation", rotation, "%.2f")) {
            transform.setRotation({ rotation[0], rotation[1], rotation[2] });
        }

        ImGui::Separator();
        ImGui::Text("Material");

        auto marbleTexture = m_resource.createResourceFromFile<jazzy::TextureResource>(L"./Game/Assets/Textures/marble_bust_01_diff_1k.jpg");
        auto marbleMat = m_resource.createResourceFromFile<jazzy::MaterialResource>(L"./Game/Assets/Shaders/BasicShader.hlsl");
        marbleMat->setTexture(0, marbleTexture);

        auto brickTexture = m_resource.createResourceFromFile<jazzy::TextureResource>(L"./Game/Assets/Textures/red_brick_03_diff_1k.jpg");
        auto brickMat = m_resource.createResourceFromFile<jazzy::MaterialResource>(L"./Game/Assets/Shaders/BasicShader.hlsl");
        brickMat->setTexture(0, brickTexture);

        auto stoneTexture = m_resource.createResourceFromFile<jazzy::TextureResource>(L"./Game/Assets/Textures/stone.jpg");
        auto stoneMat = m_resource.createResourceFromFile<jazzy::MaterialResource>(L"./Game/Assets/Shaders/BasicShader.hlsl");
        stoneMat->setTexture(0, stoneTexture);

        auto stoneTileTexture = m_resource.createResourceFromFile<jazzy::TextureResource>(L"./Game/Assets/Textures/stone_tiles_02_diff_1k.jpg");
        auto stoneTileMat = m_resource.createResourceFromFile<jazzy::MaterialResource>(L"./Game/Assets/Shaders/BasicShader.hlsl");
        stoneTileMat->setTexture(0, stoneTileTexture);

        auto woodTexture = m_resource.createResourceFromFile<jazzy::TextureResource>(L"./Game/Assets/Textures/wood.jpg");
        auto woodMat = m_resource.createResourceFromFile<jazzy::MaterialResource>(L"./Game/Assets/Shaders/BasicShader.hlsl");
        woodMat->setTexture(0, woodTexture);

        const char* materialNames[] = { "Marble", "Brick", "Stone", "Stone Tile", "Wood" }; 
        static int selectedMaterial = 0; 
        if (ImGui::Combo("##Material", &selectedMaterial, materialNames, IM_ARRAYSIZE(materialNames))) { 
            switch (selectedMaterial) { 
                case 0: 
                    setMaterial(selectedObject, marbleMat); 
                    break; 
                case 1: setMaterial(selectedObject, brickMat); 
                    break; 
                case 2: setMaterial(selectedObject, stoneMat); 
                    break; 
                case 3: setMaterial(selectedObject, stoneTileMat); 
                    break; 
                case 4: setMaterial(selectedObject, woodMat); 
                    break; 
            } 
        }

        ImGui::Separator();

        if (ImGui::Button("Delete"))
        {
            m_world.deleteGameObject(selectedObject);
            m_world.resetSelectedObject();

            ImGui::End();
            return;
        }

        ImGui::End();

    }
}

void jazzy::InspectorScreen::setHiddenRecursive(GameObject* object, bool hidden)
{
    for (ui32 i = 0; i < object->getChildCount(); i++)
    {
        GameObject* child =object->getChildByIndex(i);

        if (auto* cube = child->getComponent<CubeComponent>())
            cube->setHiddenByParent(hidden);
        
        if (auto* plane =child->getComponent<PlaneComponent>())
            plane->setHiddenByParent(hidden);
        
        if (auto* sphere = child->getComponent<SphereComponent>())
            sphere->setHiddenByParent(hidden);
        
        if (auto* capsule = child->getComponent<CapsuleComponent>())
            capsule->setHiddenByParent(hidden);
        
        if (auto* cylinder = child->getComponent<CylinderComponent>())
            cylinder->setHiddenByParent(hidden);
        
        setHiddenRecursive(child, hidden);
    }
}
void jazzy::InspectorScreen::setMaterial(GameObject* object, RefPtr<MaterialResource> material)
{
    if (!object || !material)
        return;

    if (m_world.getSelectedObjectType() == "Cube")
        object->getComponent<CubeComponent>()->setMaterial(material);

    else if (m_world.getSelectedObjectType() == "Plane")
        object->getComponent<PlaneComponent>()->setMaterial(material);

    else if (m_world.getSelectedObjectType() == "Sphere")
        object->getComponent<SphereComponent>()->setMaterial(material);

    else if (m_world.getSelectedObjectType() == "Capsule")
        object->getComponent<CapsuleComponent>()->setMaterial(material);

    else if (m_world.getSelectedObjectType() == "Cylinder")
        object->getComponent<CylinderComponent>()->setMaterial(material);
}
