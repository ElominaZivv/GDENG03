#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/Math/Vec3.h>
#include <JAZZY/Math/Vec4.h>
#include <JAZZY/Math/Vec2.h>
#include <JAZZY/Math/Mat4x4.h>
#include <vector>

namespace jazzy
{
	class WorldRenderer final : public Base
	{
	public:
		explicit WorldRenderer(const WorldRendererDesc& desc);
		void render(const World& world, SwapChain& swapChain, f32 deltaTime);

	private:
		struct alignas(16) ObjectData
		{
			Mat4x4 affineWorld{};
			Mat4x4 rigidWorld{};
		};
		struct alignas(16) CameraData
		{
			Mat4x4 view{};
			Mat4x4 proj{};
			Vec3 position{};
		};
	private:
		GraphicsDevice& m_graphicsDevice;
		RefPtr<DeviceContext> m_deviceContext{};

		RefPtr<ConstantBuffer> m_cameraCb{};
		RefPtr<ConstantBuffer> m_objectCb{};
	};
}

