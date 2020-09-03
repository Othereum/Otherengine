#include "Audio/SoundEvent.hpp"
#include "fmod_studio.hpp"

namespace oeng
{
inline namespace engine
{
void SoundEvent::Restart() const
{
    instance_->start();
}

void SoundEvent::Stop(bool allow_fadeout) const
{
    instance_->stop(allow_fadeout ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
}

void SoundEvent::SetPaused(bool paused) const
{
    instance_->setPaused(paused);
}

void SoundEvent::SetVolume(Float volume) const
{
    instance_->setVolume(volume);
}

void SoundEvent::SetPitch(Float pitch) const
{
    instance_->setPitch(pitch);
}

void SoundEvent::SetParam(const char* name, Float value) const
{
    instance_->setParameterByName(name, value);
}

bool SoundEvent::IsPaused() const
{
    bool paused;
    instance_->getPaused(&paused);
    return paused;
}

Float SoundEvent::GetVolume() const
{
    float volume;
    instance_->getVolume(&volume);
    return volume;
}

Float SoundEvent::GetPitch() const
{
    float pitch;
    instance_->getPitch(&pitch);
    return pitch;
}

Float SoundEvent::GetParam(const char* name) const
{
    float value;
    instance_->getParameterByName(name, &value);
    return value;
}
}
}
