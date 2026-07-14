#pragma once

#include <JAZZY/Core/Core.h>
#include <JAZZY/Game/Component.h>

namespace jazzy
{
	class CubeComponent final : public Component
	{
		dx3d_typeid(CubeComponent)

	public:
		explicit CubeComponent(const ComponentDesc& data);

		VertexBuffer& getVertexBuffer();
		IndexBuffer& getIndexBuffer();

	private:
		RefPtr<VertexBuffer> m_vb{};
		RefPtr<IndexBuffer> m_ib{};
	};
}
