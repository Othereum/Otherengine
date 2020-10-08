#pragma once
#include "Geometry.hpp"

namespace otm
{
	struct Transform
	{
		static const Transform identity;
		
		Vec3 pos;
		Quat rot;
		Vec3 scale = Vec3::One();

		constexpr Transform() noexcept = default;
		explicit constexpr Transform(const Vec3& pos) noexcept: Transform{pos, {}} {}
		explicit constexpr Transform(const Quat& rot) noexcept: Transform{{}, rot} {}
		
		constexpr Transform(const Vec3& pos, const Quat& rot, const Vec3& scale = Vec3::One()) noexcept
			:pos{pos}, rot{rot}, scale{scale}
		{
		}

		explicit Transform(const Mat4& m) noexcept
			:pos{m[3]}, scale{m.Row<3>(0).Len(), m.Row<3>(1).Len(), m.Row<3>(2).Len()}
		{
			Mat3 rm{m};
			rm[0] /= scale[0];
			rm[1] /= scale[1];
			rm[2] /= scale[2];
			rot = Quat{rm};
		}

		[[nodiscard]] constexpr Mat4 ToMatrix() const noexcept
		{
			return MakeScale<4>(scale) * MakeRotation<4>(rot) * MakeTranslation(pos);
		}
	};

	inline const Transform Transform::identity;
}
