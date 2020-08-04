#pragma once

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

namespace oeng::engine
{
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
}
