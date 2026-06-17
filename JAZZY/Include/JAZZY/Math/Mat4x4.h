#pragma once
#include <cassert>
#include <JAZZY/Core/Core.h>
#include <JAZZY/Math/Vec3.h>

namespace jazzy
{
	class Mat4x4
	{
	public:
		Mat4x4() = default;

		// 
		static Mat4x4 identity() noexcept
		{
			Mat4x4 res{};
			res.m_data[0][0] = 1;
			res.m_data[1][1] = 1;
			res.m_data[2][2] = 1;
			res.m_data[3][3] = 1;
			return res;
		}

		static Mat4x4 translation(const Vec3& translation) noexcept
		{
			auto res = Mat4x4::identity();
			res.m_data[3][0] = translation.x;
			res.m_data[3][1] = translation.y;
			res.m_data[3][2] = translation.z;
			return res;
		}

		static Mat4x4 scale(const Vec3& scale) noexcept
		{
			Mat4x4 res{};
			res.m_data[0][0] = scale.x;
			res.m_data[1][1] = scale.y;
			res.m_data[2][2] = scale.z;
			res.m_data[3][3] = 1;
			return res;
		}

		static Mat4x4 orthoLH(f32 width, f32 height, f32 zNear, f32 zFar) noexcept
		{
			assert(width != 0.0f && "OrhtoLH: width must not be zero");
			assert(height != 0.0f && "OrhtoLH: height must not be zero");
			assert(zFar != zNear && "OrhtoLH: zNear and zFar must not be equal");

			Mat4x4 res{};
			res.m_data[0][0] = 2.0f / width;
			res.m_data[1][1] = 2.0f / height;
			res.m_data[2][2] = 1.0f / (zFar - zNear);
			res.m_data[3][2] = -(zNear / (zFar - zNear));
			res.m_data[3][3] = 1;
			return res;
		}

		// Matrix Multiplication
		Mat4x4 operator *(const Mat4x4& rhs) const noexcept
		{
			Mat4x4 res{};
			for (auto row = 0u; row < 4u; ++row)
			{
				for (auto k = 0u; k < 4u; ++k)
				{
					auto temp = m_data[row][k];
					for (auto col = 0u; col < 4u; ++col)
					{
						res.m_data[row][col] += temp * rhs.m_data[k][col];
					}
					
				}
			}

			/*	Original Implementation where the rhs modifies the lhs
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					m_data[i][j] =
						m_data[i][0] * rhs.m_data[0][j] +
						m_data[i][1] * rhs.m_data[1][j] +
						m_data[i][2] * rhs.m_data[2][j] +
						m_data[i][3] * rhs.m_data[3][j];
				}
			}
			*/
			return res;
		}

	private:
		f32 m_data[4][4]{};
	};
}
