#pragma once
#include "Math.hpp"

namespace oeng
{
	class IDirLight
	{
	public:
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
	public:
		OE_INTERFACE_BODY(ISkyLight);
		
		[[nodiscard]] virtual const Vec3& GetColor() const noexcept = 0;
	};

	class IPointLight
	{
	public:
		OE_INTERFACE_BODY(IPointLight);
		
		struct Data
		{
			Vec3 pos;
			Vec3 color;
			Float radius;
		};

		[[nodiscard]] virtual bool ShouldAffect() const noexcept = 0;
		[[nodiscard]] virtual const Data& GetData() const noexcept = 0;
	};

	class ISpotLight
	{
	public:
		OE_INTERFACE_BODY(ISpotLight);
		
		struct Data
		{
			Vec3 pos;
			Vec3 color;
			UVec3 dir;
			Float radius;
			
			struct Cone
			{
				Cone(Rad inner, Rad outer) :inner{Cos(inner)}, outer{Min(this->inner, Cos(outer))} {}
				Float inner;
				Float outer;
			} angle_cos;
		};

		[[nodiscard]] virtual bool ShouldAffect() const noexcept = 0;
		[[nodiscard]] virtual const Data& GetData() const noexcept = 0;
	};
}
