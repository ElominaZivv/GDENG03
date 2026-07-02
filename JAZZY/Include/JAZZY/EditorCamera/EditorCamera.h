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
		void update(f32 _deltaTime);
		void setDisplayRect(Rect _rect);
		Mat4x4 getViewMat();
		Mat4x4 getProjectionViewMat();
	public:
		Projection m_projection{ Projection::PERSPECTIVE };
	private:
		Mat4x4 getOrthographicViewMat();
		Mat4x4 getPerspectiveViewMat();
		void updatePosition(f32 _deltaTime);
	private:
		Mat4x4 m_viewMat{};
		Rect m_viewportSize{};
		f32 m_fov = 1.3f;
		f32 m_zNear = 0.01f;
		f32 m_zFar = 100.0f;
		InputSystemPtr m_inputSystem{};

		f32 m_moveSpeed = 5.0f;
		Vec3 m_velocity{};
	};
}

