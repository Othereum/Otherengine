#include "Audio/AudioSystem.hpp"

#include "FModError.hpp"
#include "fmod_studio.hpp"
#include "Format.hpp"

namespace oeng::engine
{
	static String8 GetName(const FMOD::Studio::EventDescription& event);

	template <class Fn>
	static void ForEachEvent(const FMOD::Studio::Bank& bank, Fn&& fn)
	{
		int num_events;
		bank.getEventCount(&num_events);
		
		DyArr<FMOD::Studio::EventDescription*> events(num_events);
		bank.getEventList(events.data(), num_events, &num_events);

		for (auto* const event : events)
		{
			fn(*event);
		}
	}


	AudioSystem::AudioSystem()
		:system_{}, core_system_{}
	{
		auto result = FMOD::Studio::System::create(&system_);
		FModCheck(result, u8"Failed to create FMOD system"sv);

		result = system_->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
		FModCheck(result, u8"Failed to initialize FMOD system"sv);

		system_->getCoreSystem(&core_system_);
	}

	AudioSystem::~AudioSystem()
	{
		system_->release();
	}

	void AudioSystem::Update(Float) const
	{
		system_->update();
	}

	bool AudioSystem::LoadBank(Path path)
	{
		if (banks_.contains(path)) return false;

		FMOD::Studio::Bank* bank = nullptr;
		const auto result = system_->loadBankFile(
			path.Str<char>().c_str(),
			FMOD_STUDIO_LOAD_BANK_NORMAL,
			&bank
		);

		FModCheck(result, u8"Failed to load bank from file '{}'", path.Str());

		banks_.try_emplace(path, bank);
		bank->loadSampleData();

		ForEachEvent(*bank, [this](FMOD::Studio::EventDescription& event)
		{
			auto [it, inserted] = events_.try_emplace(GetName(event), &event);
			if (!inserted) log::Warn(u8"Event name duplicated: {}"sv, *it->first);
		});

		return true;
	}

	bool AudioSystem::UnloadBank(Path path)
	{
		const auto bank_it = banks_.find(path);
		if (bank_it == banks_.end()) return false;

		UnloadBank(*bank_it->second);
		banks_.erase(bank_it);
		
		return true;
	}

	void AudioSystem::UnloadAllBanks()
	{
		for (auto& [path, bank] : banks_)
		{
			UnloadBank(*bank);
		}

		banks_.clear();
	}

	SoundEvent AudioSystem::PlayEvent(Name name)
	{
		
	}

	void AudioSystem::UnloadBank(FMOD::Studio::Bank& bank)
	{
		ForEachEvent(bank, [this](FMOD::Studio::EventDescription& event)
		{
			events_.erase(GetName(event));
		});

		bank.unload();
	}


	String8 GetName(const FMOD::Studio::EventDescription& event)
	{
		String8 event_name;

		int buf_size;
		event.getPath(nullptr, 0, &buf_size);
		
		event_name.resize(buf_size - 1);
		event.getPath(AsString(event_name).data(), buf_size, nullptr);

		return event_name;
	}
	
}
