#pragma once
#include "Math.hpp"
#include "Interfaces/Base.hpp"

namespace oeng
{
	class IDirLight
	{
		OE_INTERFACE_BODY(IDirLight);
		
		struct Data
		{
			UVec3 dir;
			Vec3 color;
		};
		
		[[nodiscard]] virtual const Data& GetData() const noexcept = 0;
	};

	class ISkyLight
	{
		OE_INTERFACE_BODY(ISkyLight);
		
		[[nodiscard]] virtual const Vec3& GetColor() const noexcept = 0;
	};

	class IPointLight
	{
		OE_INTERFACE_BODY(IPointLight);
		
		struct Data
		{
			Vec3 pos;
			Vec3 color;
			Float radius;
		};

		[[nodiscard]] virtual const Data& GetData() const noexcept = 0;
	};

	class ISpotLight
	{
		OE_INTERFACE_BODY(ISpotLight);
		
		struct Data
		{
			Vec3 pos;
			Vec3 color;
			UVec3 dir;
			Rad angle;
			Float radius;
		};

		[[nodiscard]] virtual const Data& GetData() const noexcept = 0;
	};
}
