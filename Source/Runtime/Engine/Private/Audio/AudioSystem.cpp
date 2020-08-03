#include "Audio/AudioSystem.hpp"
#include "fmod_studio.hpp"
#include "Format.hpp"

namespace oeng::core
{
	AudioSystem::AudioSystem()
		:system_{}, core_system_{}
	{
		auto result = FMOD::Studio::System::create(&system_);
		if (result != FMOD_OK) Throw(u8"Failed to create FMOD system: {}"sv, result);

		result = system_->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
		if (result != FMOD_OK) Throw(u8"Failed to initialize FMOD system: {}"sv, result);

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
}
