#pragma once
#include <JAZZY/Core/Base.h>
#include <JAZZY/Math/Vec3.h>

namespace jazzy
{
	class EditorCamera : public Base
	{
	public:
		EditorCamera(const EditorCameraDesc& desc);
	private:
		InputSystemPtr m_inputSystem{};
	};
}

