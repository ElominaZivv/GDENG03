#pragma once
#include <JAZZY/Core/Base.h>
#include <JAZZY/Math/Vec3.h>
#include <JAZZY/Math/Mat4x4.h>

namespace jazzy
{
	class EditorCamera : public Base
	{
		enum class Projection
		{
			NONE = 0,
			ORTHOGRAPHIC,
			PERSPECTIVE
		};

	public:
		EditorCamera(const EditorCameraDesc& desc);
		void update();
		Mat4x4 getViewMat();
		Mat4x4 getProjectionViewMat();
	public:
		Projection m_projection{ Projection::PERSPECTIVE };
	private:
		Mat4x4 getOrthographicViewMat();
		Mat4x4 getPerspectiveViewMat();
	private:
		Mat4x4 m_viewMat{};
		InputSystemPtr m_inputSystem{};
	};
}

