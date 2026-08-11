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
#include <JAZZY/ComponentS/MeshComponent.h>
#include <JAZZY/Components/CapsuleComponent.h>
#include <JAZZY/Components/CylinderComponent.h>
#include <JAZZY/Components/PlaneComponent.h>
#include <JAZZY/Components/SphereComponent.h>

#include <JAZZY/Resource/MaterialResource.h>
#include <JAZZY/Resource/TextureResource.h>
#include <JAZZY/Resource/MeshResource.h>
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
	m_materialCb = device.createConstantBuffer({ {}, jazzy::MaterialResource::MaxDataSize});
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
	auto& materialCb = *m_materialCb;

	// Camera
	{
		CameraData cameraData{};
		cameraData.view = editorCamera.getViewMat();
		editorCamera.setDisplayRect(size);
		cameraData.proj = editorCamera.getProjectionViewMat();
		cameraData.position = Vec3(editorCamera.getViewMat().row(3));
		context.updateConstantBuffer(cameraCb, std::as_bytes(std::span{ &cameraData, 1 }));
	}

	// Cubes
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
				context.updateConstantBuffer(materialCb, material->getData());
				ConstantBuffer* cbs[] = { &objectCb, &cameraCb, &materialCb };
				context.setConstantBuffers(std::span<ConstantBuffer*>{cbs});

				m_textures.clear();
				m_textures.resize(material->getNumTextures());
				for (auto t: std::views::iota(0u, m_textures.size()))
				{
					auto tex = material->getTexture(t);
					if (tex) m_textures[t] = &tex->getTexture();
				}
				context.setTextures(std::span<Texture*>{m_textures});

				context.setVertexBuffer(component->getVertexBuffer());
				context.setIndexBuffer(component->getIndexBuffer());
				context.drawIndexedTriangleList(component->getIndexBuffer().getIndexListSize(), 0u, 0u);
			}
		}
	}

	// Planes
	{
		ObjectData objectData{};
		auto components = world.getComponents<PlaneComponent>(numComponents);
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
				context.updateConstantBuffer(materialCb, material->getData());
				ConstantBuffer* cbs[] = { &objectCb, &cameraCb, &materialCb };
				context.setConstantBuffers(std::span<ConstantBuffer*>{cbs});

				m_textures.clear();
				m_textures.resize(material->getNumTextures());
				for (auto t : std::views::iota(0u, m_textures.size()))
				{
					auto tex = material->getTexture(t);
					if (tex) m_textures[t] = &tex->getTexture();
				}
				context.setTextures(std::span<Texture*>{m_textures});

				context.setVertexBuffer(component->getVertexBuffer());
				context.setIndexBuffer(component->getIndexBuffer());
				context.drawIndexedTriangleList(component->getIndexBuffer().getIndexListSize(), 0u, 0u);
			}
		}
	}

	// Spheres
	{
		ObjectData objectData{};
		auto components = world.getComponents<SphereComponent>(numComponents);
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
				context.updateConstantBuffer(materialCb, material->getData());
				ConstantBuffer* cbs[] = { &objectCb, &cameraCb, &materialCb };
				context.setConstantBuffers(std::span<ConstantBuffer*>{cbs});

				m_textures.clear();
				m_textures.resize(material->getNumTextures());
				for (auto t : std::views::iota(0u, m_textures.size()))
				{
					auto tex = material->getTexture(t);
					if (tex) m_textures[t] = &tex->getTexture();
				}
				context.setTextures(std::span<Texture*>{m_textures});

				context.setVertexBuffer(component->getVertexBuffer());
				context.setIndexBuffer(component->getIndexBuffer());
				context.drawIndexedTriangleList(component->getIndexBuffer().getIndexListSize(), 0u, 0u);
			}
		}
	}

	// Cylinders
	{
		ObjectData objectData{};
		auto components = world.getComponents<CylinderComponent>(numComponents);
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
				context.updateConstantBuffer(materialCb, material->getData());
				ConstantBuffer* cbs[] = { &objectCb, &cameraCb, &materialCb };
				context.setConstantBuffers(std::span<ConstantBuffer*>{cbs});

				m_textures.clear();
				m_textures.resize(material->getNumTextures());
				for (auto t : std::views::iota(0u, m_textures.size()))
				{
					auto tex = material->getTexture(t);
					if (tex) m_textures[t] = &tex->getTexture();
				}
				context.setTextures(std::span<Texture*>{m_textures});

				context.setVertexBuffer(component->getVertexBuffer());
				context.setIndexBuffer(component->getIndexBuffer());
				context.drawIndexedTriangleList(component->getIndexBuffer().getIndexListSize(), 0u, 0u);
			}
		}
	}

	// Capsules
	{
		ObjectData objectData{};
		auto components = world.getComponents<CapsuleComponent>(numComponents);
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
				context.updateConstantBuffer(materialCb, material->getData());
				ConstantBuffer* cbs[] = { &objectCb, &cameraCb, &materialCb };
				context.setConstantBuffers(std::span<ConstantBuffer*>{cbs});

				m_textures.clear();
				m_textures.resize(material->getNumTextures());
				for (auto t : std::views::iota(0u, m_textures.size()))
				{
					auto tex = material->getTexture(t);
					if (tex) m_textures[t] = &tex->getTexture();
				}
				context.setTextures(std::span<Texture*>{m_textures});

				context.setVertexBuffer(component->getVertexBuffer());
				context.setIndexBuffer(component->getIndexBuffer());
				context.drawIndexedTriangleList(component->getIndexBuffer().getIndexListSize(), 0u, 0u);
			}
		}
	}

	// Meshes
	{
		ObjectData objectData{};
		auto components = world.getComponents<MeshComponent>(numComponents);
		for (auto i : std::views::iota(0u, numComponents))
		{
			auto comp = components[i];
			auto meshRes = comp->getMesh();
			if (!meshRes) continue;
			auto& mesh = *meshRes;

			objectData.affineWorld = comp->getGameObject().getTransform().getAffineWorldMatrix();
			objectData.rigidWorld = comp->getGameObject().getTransform().getRigidWorldMatrix();


			context.setVertexBuffer(mesh.getVertexBuffer());
			context.setIndexBuffer(mesh.getIndexBuffer());

			auto numSlots = 0u;
			auto slots = mesh.getMaterialSlots(numSlots);

			for (auto u : std::views::iota(0u, numSlots))
			{
				auto slot = slots[u];
				auto material = comp->getMaterial(u);
				if (!material) continue;
				auto numTexs = material->getNumTextures();

				context.setGraphicsPipelineState(material->getGraphicsPipelineState());
				context.updateConstantBuffer(objectCb, std::as_bytes(std::span{ &objectData, 1 }));
				context.updateConstantBuffer(materialCb, material->getData());
				ConstantBuffer* cbs[] = { &objectCb, &cameraCb, &materialCb };
				context.setConstantBuffers(std::span<ConstantBuffer*>{cbs});

				m_textures.clear();
				m_textures.resize(material->getNumTextures());
				for (auto t : std::views::iota(0u, m_textures.size()))
				{
					auto tex = material->getTexture(t);
					if (tex) m_textures[t] = &tex->getTexture();
				}
				context.setTextures(std::span<Texture*>{m_textures});

				context.drawIndexedTriangleList(slot.indexCount, 0, slot.startIndex);
			}
		}
	}

	m_graphicsDevice.executeCommandList(context);
	m_graphicsDevice.setBackBuffer(swapChain);
	uiManager.draw();
	swapChain.present();
}
