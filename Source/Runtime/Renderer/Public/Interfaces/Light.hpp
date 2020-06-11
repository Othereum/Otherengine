#pragma once
#include "Math.hpp"

namespace oeng
{
	class IDirLight
	{
	public:
		struct Data
		{
			UVec3 dir;
			Vec3 color;
		};
		
		[[nodiscard]] virtual const Data& GetData() const noexcept = 0;
		
		constexpr IDirLight() noexcept = default;
		virtual ~IDirLight() = default;
		
		IDirLight(const IDirLight&) = delete;
		IDirLight(IDirLight&&) = delete;
		IDirLight& operator=(const IDirLight&) = delete;
		IDirLight& operator=(IDirLight&&) = delete;
	};

	class ISkyLight
	{
	public:
		[[nodiscard]] virtual const Vec3& GetColor() const noexcept = 0;
		
		constexpr ISkyLight() noexcept = default;
		virtual ~ISkyLight() = default;
		
		ISkyLight(const ISkyLight&) = delete;
		ISkyLight(ISkyLight&&) = delete;
		ISkyLight& operator=(const ISkyLight&) = delete;
		ISkyLight& operator=(ISkyLight&&) = delete;
	};
}