#include <JAZZY/Game/WorldRenderer.h>
#include <JAZZY/Graphics/GraphicsDevice.h>
#include <JAZZY/Graphics/DeviceContext.h>
#include <JAZZY/Graphics/SwapChain.h>
#include <JAZZY/Graphics/VertexBuffer.h>
#include <JAZZY/Graphics/IndexBuffer.h>

#include <JAZZY/Game/World.h>
#include <JAZZY/Game/Component.h>
#include <JAZZY/Game/GameObject.h>

#include <JAZZY/ComponentS/TransformComponent.h>
#include <JAZZY/ComponentS/CubeComponent.h>

#include <JAZZY/Resource/MaterialResource.h>
#include <JAZZY/EditorCamera/EditorCamera.h>
#include <JAZZY/UI/UIManager.h>

#include <JAZZY/Math/Vec3.h>
#include <fstream>
#include <ranges>

jazzy::WorldRenderer::WorldRenderer(const WorldRendererDesc& desc): Base (desc.base), m_graphicsDevice(desc.engine)
{
	auto& device = m_graphicsDevice;
	m_deviceContext = device.createDeviceContext();

	m_objectCb = device.createConstantBuffer({ {}, sizeof(ObjectData) });
	m_cameraCb = device.createConstantBuffer({ {}, sizeof(CameraData) });
}

void jazzy::WorldRenderer::render(const World& world, SwapChain& swapChain, EditorCamera& editorCamera, UIManager& uiManager, f32 deltaTime)
{
	auto size = swapChain.getSize();

	auto& context = *m_deviceContext;
	context.clearAndSetBackBuffer(swapChain, { 0.0549f, 0.07f, 0.109f, 1.0f });
	context.setViewportSize(size);

	auto numComponents = 0u;

	auto& cameraCb = *m_cameraCb;
	auto& objectCb = *m_objectCb;

	//cameras
	{
		CameraData cameraData{};
		cameraData.view = editorCamera.getViewMat();
		editorCamera.setDisplayRect(size);
		cameraData.proj = editorCamera.getProjectionViewMat();
		Vec4 translation4 = editorCamera.getViewMat().row(3);
		Vec3 translation = Vec3(translation4.x, translation4.y, translation4.z);
		cameraData.position = translation;
		context.updateConstantBuffer(cameraCb, std::as_bytes(std::span{ &cameraData, 1 }));
	}

	//cubes
	{
		ObjectData objectData{};
		auto components = world.getComponents<CubeComponent>(numComponents);
		for (auto i : std::views::iota(0u, numComponents))
		{
			auto component = components[i];
			auto& transform = component->getGameObject().getTransform();
			auto material = component->getMaterial();

			if (material)
			{
				objectData.affineWorld = transform.getAffineWorldMatrix();
				objectData.rigidWorld = transform.getRigidWorldMatrix();

				context.setGraphicsPipelineState(material->getGraphicsPipelineState());
				context.updateConstantBuffer(objectCb, std::as_bytes(std::span{ &objectData, 1 }));
				ConstantBuffer* cbs[] = { &objectCb, &cameraCb };
				context.setConstantBuffers(std::span<ConstantBuffer*>{cbs});

				context.setVertexBuffer(component->getVertexBuffer());
				context.setIndexBuffer(component->getIndexBuffer());
				context.drawIndexedTriangleList(component->getIndexBuffer().getIndexListSize(), 0u, 0u);
			}
		}
	}
	m_graphicsDevice.executeCommandList(context);
	m_graphicsDevice.setBackBuffer(swapChain);
	uiManager.draw();
	swapChain.present();
}
