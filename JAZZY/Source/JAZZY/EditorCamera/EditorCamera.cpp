#include<JAZZY/EditorCamera/EditorCamera.h>

#include "JAZZY/Input/InputSystem.h"
using namespace jazzy;

EditorCamera::EditorCamera(const EditorCameraDesc& desc): Base(desc.base), m_inputSystem(desc.inputSystem)
{
	m_viewMat = Mat4x4::identity();
}

void EditorCamera::update()
{
	if (m_inputSystem->isKeyPressed(KeyCode::Q)) m_projection = Projection::PERSPECTIVE;
	if (m_inputSystem->isKeyPressed(KeyCode::E)) m_projection = Projection::ORTHOGRAPHIC;
}

void EditorCamera::setDisplayRect(Rect _rect)
{
	if (m_viewportSize == _rect) return;
	m_viewportSize = _rect;
}

Mat4x4 EditorCamera::getViewMat()
{
	return Mat4x4::inverse(m_viewMat) ;
}

Mat4x4 EditorCamera::getOrthographicViewMat()
{
	Mat4x4 out{};
	out = Mat4x4::orthoLH
	(
		// Instead of hardcoding the resolution, find a way to access the Rect size{} of the window
		// Moreover, you can also update the Rect size{} by using the Window msg to check whenever the size is changed and update the Rect size accordingly
		(f32)m_viewportSize.width / 400.0f,
		(f32)m_viewportSize.height / 400.0f,
		-4.0f,
		4.0f
	);
	return out;
}

Mat4x4 EditorCamera::getPerspectiveViewMat()
{
	Mat4x4 out{};
	out = Mat4x4::perspectiveFovLH
	(
		m_fov,
		(f32)m_viewportSize.width / (f32)m_viewportSize.height,
		m_zNear,
		m_zFar
	);
	return out;
}


Mat4x4 EditorCamera::getProjectionViewMat()
{
	Mat4x4 out{};
	switch (m_projection)
	{
	case Projection::NONE:
		out = Mat4x4::identity();
		break;
	case Projection::ORTHOGRAPHIC:
		out = getOrthographicViewMat();
		break;
	case Projection::PERSPECTIVE:
		out = getPerspectiveViewMat();
		break;
	default:
		out = Mat4x4::identity();
	}
	return out;
}
