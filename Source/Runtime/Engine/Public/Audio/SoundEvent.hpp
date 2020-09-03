#pragma once

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace FMOD::Studio
{
class EventInstance;
}
#endif

namespace oeng
{
inline namespace engine
{
class AudioSystem;

class ENGINE_API SoundEvent
{
public:
    void Restart() const;
    void Stop(bool allow_fadeout = true) const;

    void SetPaused(bool paused) const;
    void SetVolume(float volume) const;
    void SetPitch(float pitch) const;
    void SetParam(const char* name, float value) const;

    [[nodiscard]] bool IsPaused() const;
    [[nodiscard]] float GetVolume() const;
    [[nodiscard]] float GetPitch() const;
    [[nodiscard]] float GetParam(const char* name) const;

private:
    FMOD::Studio::EventInstance* instance_;
};
}
}
