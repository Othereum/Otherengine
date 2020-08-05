#pragma once

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace FMOD
{
	namespace Studio
	{
		class System;
		class Bank;
		class EventDescription;
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

		/**
		 * Load the bank and its sample data and events.
		 * @return False if already loaded. True if successful.
		 * @throw std::runtime_error If failed to load the bank.
		 */
		bool LoadBank(Path path);

		/**
		 * Unload the bank and its sample data and events.
		 * @return False if already unloaded. True if successful.
		 */
		bool UnloadBank(Path path);

		/**
		 * Unload all banks, sample data and events.
		 */
		void UnloadAllBanks();

	private:
		void UnloadBank(FMOD::Studio::Bank& bank);
		
		FMOD::Studio::System* system_;
		FMOD::System* core_system_;

		HashMap<Path, FMOD::Studio::Bank*> banks_;
		HashMap<Name, FMOD::Studio::EventDescription*> events_;
	};
}
