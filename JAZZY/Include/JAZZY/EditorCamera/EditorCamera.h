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
	private:
		Mat4x4 m_viewMat{};
		Rect m_viewportSize{};
		f32 m_fov = 1.3f;
		f32 m_zNear = 0.01f;
		f32 m_zFar = 100.0f;
		RefPtr<InputSystem> m_inputSystem{};

		// Controls
		Mat4x4 m_worldCam{};
		f32 m_forward = 0.0f;
		f32 m_right = 0.0f;
		f32 m_up = 0.0f;

		f32 m_moveSpeed = 5.0f;
		f32 m_fasterMoveSpeed = m_moveSpeed * 2.0f;
		f32 m_sensitivity = 1.0f;
		f32 m_rotX = 0.0f;
		f32 m_rotY = 0.0f;
	};
}

