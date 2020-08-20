#pragma once
#include <variant>

namespace oeng::renderer
{
	using Uniform = std::variant<
		float, int,
	
		Vec2, Vec3, Vec4,
		Vector<int, 2>, Vector<int, 3>, Vector<int, 4>,
	
		Mat2, Mat3, Mat4,
		Matrix<Float, 2, 3>, Matrix<Float, 2, 4>,
		Matrix<Float, 3, 2>, Matrix<Float, 3, 4>,
		Matrix<Float, 4, 2>, Matrix<Float, 4, 3>
	>;
}
