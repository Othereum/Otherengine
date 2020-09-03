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
    void SetVolume(Float volume) const;
    void SetPitch(Float pitch) const;
    void SetParam(const char* name, Float value) const;

    [[nodiscard]] bool IsPaused() const;
    [[nodiscard]] Float GetVolume() const;
    [[nodiscard]] Float GetPitch() const;
    [[nodiscard]] Float GetParam(const char* name) const;

private:
    FMOD::Studio::EventInstance* instance_;
};
}
}
