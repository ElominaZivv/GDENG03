#pragma once
#include "JAZZY/Math/Mat4x4.h"
#include "JAZZY/Math/Vec3.h"


struct SAVE_Transform {
	jazzy::Vec3 m_position{ 0.0f, 0.0f, 0.0f };
	jazzy::Vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
	jazzy::Vec3 m_scale{ 1.0f, 1.0f, 1.0f };
};