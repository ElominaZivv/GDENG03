#include<JAZZY/EditorCamera/EditorCamera.h>

#include "JAZZY/Input/InputSystem.h"
using namespace jazzy;

EditorCamera::EditorCamera(const EditorCameraDesc& desc): Base(desc.base), m_inputSystem(desc.inputSystem)
{
	m_viewMat = Mat4x4::identity();
}

void EditorCamera::update(f32 _deltaTime)
{
	if (m_inputSystem->isKeyPressed(KeyCode::P)) m_projection = Projection::PERSPECTIVE;
	if (m_inputSystem->isKeyPressed(KeyCode::O)) m_projection = Projection::ORTHOGRAPHIC;

	f32 moveSpeed = m_moveSpeed;
	if (m_inputSystem->isKeyDown(KeyCode::Shift)) moveSpeed = m_fasterMoveSpeed;

	if (m_inputSystem->isKeyDown(KeyCode::W)) m_forward = 1.0f;
	if (m_inputSystem->isKeyDown(KeyCode::S)) m_forward = -1.0f;
	if (m_inputSystem->isKeyDown(KeyCode::D)) m_right = 1.0f;
	if (m_inputSystem->isKeyDown(KeyCode::A)) m_right = -1.0f;
	if (m_inputSystem->isKeyDown(KeyCode::E)) m_up = 1.0f;
	if (m_inputSystem->isKeyDown(KeyCode::Q)) m_up = -1.0f;

	Mat4x4 world_cam{};
	world_cam = Mat4x4::identity();

	m_inputSystem->setCursorLocked(false);
	m_inputSystem->setCursorVisible(true);
	// Rotation
	if (m_inputSystem->isKeyDown(KeyCode::MouseRight))
	{
		m_inputSystem->setCursorLocked(true);
		m_inputSystem->setCursorVisible(false);
		m_rotX += m_inputSystem->getMouseDelta().y * m_sensitivity * _deltaTime;
		m_rotY += m_inputSystem->getMouseDelta().x * m_sensitivity * _deltaTime;
	}
	world_cam = world_cam * Mat4x4::rotateX(m_rotX);
	world_cam = world_cam * Mat4x4::rotateY(m_rotY);

	// Translation
	Vec4 translation4 = m_viewMat.row(3);
	Vec3 translation = Vec3(translation4.x, translation4.y, translation4.z);

	// Forward
	Vec4 worldCamDirZ4(world_cam.row(2));
	Vec3 worldCamDirZ3(worldCamDirZ4.x, worldCamDirZ4.y, worldCamDirZ4.z);
	Vec3 newPos = translation + worldCamDirZ3 * (m_forward * moveSpeed * _deltaTime);

	// Right
	Vec4 worldCamDirX4(world_cam.row(0));
	Vec3 worldCamDirX3(worldCamDirX4.x, worldCamDirX4.y, worldCamDirX4.z);
	newPos = newPos + worldCamDirX3 * (m_right * moveSpeed * _deltaTime);

	// Up
	Vec4 worldCamDirY4(world_cam.row(1));
	Vec3 worldCamDirY3(worldCamDirY4.x, worldCamDirY4.y, worldCamDirY4.z);
	newPos = newPos + worldCamDirY3 * (m_up * moveSpeed * _deltaTime);

	world_cam = world_cam * Mat4x4::translation(newPos);

	m_viewMat = world_cam;
	m_forward = 0.0f;
	m_right = 0.0f;
	m_up = 0.0f;
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
		-50.0f,
		50.0f
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
