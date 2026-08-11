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

    auto num = 0u;
    auto objects = m_world.getComponents<TransformComponent>(num);

    if (m_world.GetSelectedIndex() < num)
        selectedObject = &objects[m_world.GetSelectedIndex()]->getGameObject();

    // for if nothing is selected
    if (selectedObject == nullptr)
    {
        if (ImGui::BeginViewportSideBar("Inspector", viewport, ImGuiDir_Right, 200.0f, windowFlags))
        {
            ImGui::Text("Nothing selected");
            ImGui::End();
        }

        return;
    }

    // get initial obj info
    auto& transform = selectedObject->getTransform();

    float position[3] = {transform.getPosition().x, transform.getPosition().y, transform.getPosition().z };
    float scale[3] = { transform.getScale().x, transform.getScale().y, transform.getScale().z };
    float rotation[3] = { transform.getRotation().x, transform.getRotation().y, transform.getRotation().z };

    // start sidebar
    if (ImGui::BeginViewportSideBar("Inspector", viewport, ImGuiDir_Right, 200.0f, windowFlags)) {

        // displaying the obj name
        ImGui::Text(selectedObject->m_name.c_str());
        ImGui::Separator();

        // >>>> CHANGE OBJ NAME ----------------------------------------------------
        static char objCurrentName[64] = "";

        // Render the text input widget
        if (selectedObject->_id != m_world.ROOTSCENE_ID) {
            if (ImGui::InputText("Name", objCurrentName, sizeof(objCurrentName))) {
                selectedObject->m_name = objCurrentName;
            }
        }

        // >>>> CHANGE OBJ VISIBILITY ----------------------------------------------
        if (selectedObject->_id != m_world.ROOTSCENE_ID) {
            bool visible = true;

            visible = !selectedObject->getComponent<TransformComponent>()->getHidden();


            if (ImGui::Checkbox("Visible", &visible))
            {
                bool hidden = !visible;

                selectedObject->getComponent<TransformComponent>()->setHidden(hidden);

                setHiddenRecursive(selectedObject, hidden);
            }


            ImGui::Separator();
        }

        // >>>> CHANGE OBJ TRANSFORM -----------------------------------------------
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

        // >>>> CHANGE OBJ MATERIAL ------------------------------------------------
        if (selectedObject->_id != m_world.ROOTSCENE_ID) {
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
        }

        // >>>> DELETE OBJ ---------------------------------------------------------
        if (selectedObject->_id != m_world.ROOTSCENE_ID) {
            if (ImGui::Button("Delete"))
            {
                    m_world.deleteGameObject(selectedObject);
                    m_world.resetSelectedObject();

                ImGui::End();
                return;
            }
        }

        ImGui::End();

    }
}

void jazzy::InspectorScreen::setHiddenRecursive(GameObject* object, bool hidden)
{
    for (ui32 i = 0; i < object->getChildCount(); i++)
    {
        GameObject* child = object->getChildByIndex(i);

        if (auto* object = child->getComponent<TransformComponent>())
            object->setHiddenByParent(hidden);
        
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

    else if (m_world.getSelectedObjectType() == "Mesh")
        object->getComponent<MeshComponent>()->setMaterial(0, material);
}
