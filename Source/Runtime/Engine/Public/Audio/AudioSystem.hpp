#pragma once
#include "SoundEvent.hpp"

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
	class AudioSystemError : public std::runtime_error {};
	class FModError : public AudioSystemError {};
	class BankAlreadyLoaded : public AudioSystemError {};
	
	class ENGINE_API AudioSystem
	{
	public:
		DELETE_CPMV(AudioSystem);
		
		AudioSystem();
		~AudioSystem();

		void Update(Float delta_seconds) const;

		/**
		 * @brief Load the bank and its sample data and events.
		 * @param path Bank file path. Must be valid.
		 * @return False if already loaded. True if successful.
		 * @throw std::runtime_error If failed to load the bank.
		 */
		bool LoadBank(Path path);

		/**
		 * @brief Unload the bank and its sample data and events.
		 * @param path Bank file path.
		 * @return False if already unloaded. True if successful.
		 */
		bool UnloadBank(Path path);

		/**
		 * @brief Unload all banks, sample data and events.
		 */
		void UnloadAllBanks();

		/**
		 * @brief Create an event instance and start playback.
		 * @param name Event name. Must be valid.
		 * @return Sound event.
		*/
		SoundEvent PlayEvent(Name name);

	private:
		void UnloadBank(FMOD::Studio::Bank& bank);
		
		FMOD::Studio::System* system_;
		FMOD::System* core_system_;

		HashMap<Path, FMOD::Studio::Bank*> banks_;
		HashMap<Name, FMOD::Studio::EventDescription*> events_;

		unsigned next_id_ = 0;
		HashMap<unsigned, FMOD::Studio::EventInstance*> event_instances_;
	};
}
