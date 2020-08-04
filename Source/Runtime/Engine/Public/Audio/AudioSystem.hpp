#pragma once
#include "Core.hpp"
#include "MathFwd.hpp"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace FMOD
{
	namespace Studio
	{
		class System;
	}
	class System;
}
#endif

ENGINE_BEGIN

class ENGINE_API AudioSystem
{
public:
	DELETE_CPMV(AudioSystem);
	
	AudioSystem();
	~AudioSystem();

	void Update(Float delta_seconds) const;

private:
	FMOD::Studio::System* system_;
	FMOD::System* core_system_;
};

ENGINE_END
