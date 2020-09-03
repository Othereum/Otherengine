#include "FModError.hpp"
#include "fmod.hpp"

namespace oeng
{
inline namespace engine
{
std::u8string_view FModErrorString(FMOD_RESULT result)
{
    switch (result)
    {
    case FMOD_OK:
        return u8"No errors."sv;

    case FMOD_ERR_BADCOMMAND:
        return
            u8"Tried to call a function on a data type that does not allow this type of functionality (ie calling Sound::lock on a streaming sound)."sv;

    case FMOD_ERR_CHANNEL_ALLOC:
        return u8"Error trying to allocate a channel."sv;

    case FMOD_ERR_CHANNEL_STOLEN:
        return u8"The specified channel has been reused to play another sound."sv;

    case FMOD_ERR_DMA:
        return u8"DMA Failure. See debug output for more information."sv;

    case FMOD_ERR_DSP_CONNECTION:
        return
            u8"DSP connection error. Connection possibly caused a cyclic dependency or connected dsps with incompatible buffer counts."sv;

    case FMOD_ERR_DSP_DONTPROCESS:
        return
            u8"DSP return code from a DSP process query callback. Tells mixer not to call the process callback and therefore not consume CPU. Use this to optimize the DSP graph."sv;

    case FMOD_ERR_DSP_FORMAT:
        return
            u8"DSP Format error. A DSP unit may have attempted to connect to this network with the wrong format, or a matrix may have been set with the wrong size if the target unit has a specified channel map."sv;

    case FMOD_ERR_DSP_INUSE:
        return u8"DSP is already in the mixer's DSP network. It must be removed before being reinserted or released."sv;

    case FMOD_ERR_DSP_NOTFOUND:
        return u8"DSP connection error. Couldn't find the DSP unit specified."sv;

    case FMOD_ERR_DSP_RESERVED:
        return u8"DSP operation error. Cannot perform operation on this DSP as it is reserved by the system."sv;

    case FMOD_ERR_DSP_SILENCE:
        return
            u8"DSP return code from a DSP process query callback. Tells mixer silence would be produced from read, so go idle and not consume CPU. Use this to optimize the DSP graph."sv;

    case FMOD_ERR_DSP_TYPE:
        return u8"DSP operation cannot be performed on a DSP of this type."sv;

    case FMOD_ERR_FILE_BAD:
        return u8"Error loading file."sv;

    case FMOD_ERR_FILE_COULDNOTSEEK:
        return
            u8"Couldn't perform seek operation. This is a limitation of the medium (ie netstreams) or the file format."sv;

    case FMOD_ERR_FILE_DISKEJECTED:
        return u8"Media was ejected while reading."sv;

    case FMOD_ERR_FILE_EOF:
        return u8"End of file unexpectedly reached while trying to read essential data (truncated?)."sv;

    case FMOD_ERR_FILE_ENDOFDATA:
        return u8"End of current chunk reached while trying to read data."sv;

    case FMOD_ERR_FILE_NOTFOUND:
        return u8"File not found."sv;

    case FMOD_ERR_FORMAT:
        return u8"Unsupported file or audio format."sv;

    case FMOD_ERR_HEADER_MISMATCH:
        return
            u8"There is a version mismatch between the FMOD header and either the FMOD Studio library or the FMOD Core library."sv;

    case FMOD_ERR_HTTP:
        return u8"A HTTP error occurred. This is a catch-all for HTTP errors not listed elsewhere."sv;

    case FMOD_ERR_HTTP_ACCESS:
        return u8"The specified resource requires authentication or is forbidden."sv;

    case FMOD_ERR_HTTP_PROXY_AUTH:
        return u8"Proxy authentication is required to access the specified resource."sv;

    case FMOD_ERR_HTTP_SERVER_ERROR:
        return u8"A HTTP server error occurred."sv;

    case FMOD_ERR_HTTP_TIMEOUT:
        return u8"The HTTP request timed out."sv;

    case FMOD_ERR_INITIALIZATION:
        return u8"FMOD was not initialized correctly to support this function."sv;

    case FMOD_ERR_INITIALIZED:
        return u8"Cannot call this command after System::init."sv;

    case FMOD_ERR_INTERNAL:
        return u8"An error occurred that wasn't supposed to. Contact support."sv;

    case FMOD_ERR_INVALID_FLOAT:
        return u8"Value passed in was a NaN, Inf or denormalized float."sv;

    case FMOD_ERR_INVALID_HANDLE:
        return u8"An invalid object handle was used."sv;

    case FMOD_ERR_INVALID_PARAM:
        return u8"An invalid parameter was passed to this function."sv;

    case FMOD_ERR_INVALID_POSITION:
        return u8"An invalid seek position was passed to this function."sv;

    case FMOD_ERR_INVALID_SPEAKER:
        return u8"An invalid speaker was passed to this function based on the current speaker mode."sv;

    case FMOD_ERR_INVALID_SYNCPOINT:
        return u8"The syncpoint did not come from this sound handle."sv;

    case FMOD_ERR_INVALID_THREAD:
        return u8"Tried to call a function on a thread that is not supported."sv;

    case FMOD_ERR_INVALID_VECTOR:
        return u8"The vectors passed in are not unit length, or perpendicular."sv;

    case FMOD_ERR_MAXAUDIBLE:
        return u8"Reached maximum audible playback count for this sound's soundgroup."sv;

    case FMOD_ERR_MEMORY:
        return u8"Not enough memory or resources."sv;

    case FMOD_ERR_MEMORY_CANTPOINT:
        return
            u8"Can't use FMOD_OPENMEMORY_POINT on non PCM source data, or non mp3/xma/adpcm data if FMOD_CREATECOMPRESSEDSAMPLE was used."sv;

    case FMOD_ERR_NEEDS3D:
        return u8"Tried to call a command on a 2d sound when the command was meant for 3d sound."sv;

    case FMOD_ERR_NEEDSHARDWARE:
        return u8"Tried to use a feature that requires hardware support."sv;

    case FMOD_ERR_NET_CONNECT:
        return u8"Couldn't connect to the specified host."sv;

    case FMOD_ERR_NET_SOCKET_ERROR:
        return u8"A socket error occurred. This is a catch-all for socket-related errors not listed elsewhere."sv;

    case FMOD_ERR_NET_URL:
        return u8"The specified URL couldn't be resolved."sv;

    case FMOD_ERR_NET_WOULD_BLOCK:
        return u8"Operation on a non-blocking socket could not complete immediately."sv;

    case FMOD_ERR_NOTREADY:
        return u8"Operation could not be performed because specified sound/DSP connection is not ready."sv;

    case FMOD_ERR_OUTPUT_ALLOCATED:
        return
            u8"Error initializing output device, but more specifically, the output device is already in use and cannot be reused."sv;

    case FMOD_ERR_OUTPUT_CREATEBUFFER:
        return u8"Error creating hardware sound buffer."sv;

    case FMOD_ERR_OUTPUT_DRIVERCALL:
        return
            u8"A call to a standard soundcard driver failed, which could possibly mean a bug in the driver or resources were missing or exhausted."sv;

    case FMOD_ERR_OUTPUT_FORMAT:
        return u8"Soundcard does not support the specified format."sv;

    case FMOD_ERR_OUTPUT_INIT:
        return u8"Error initializing output device."sv;

    case FMOD_ERR_OUTPUT_NODRIVERS:
        return
            u8"The output device has no drivers installed. If pre-init, FMOD_OUTPUT_NOSOUND is selected as the output mode. If post-init, the function just fails."sv;

    case FMOD_ERR_PLUGIN:
        return u8"An unspecified error has been returned from a plugin."sv;

    case FMOD_ERR_PLUGIN_MISSING:
        return u8"A requested output, dsp unit type or codec was not available."sv;

    case FMOD_ERR_PLUGIN_RESOURCE:
        return u8"A resource that the plugin requires cannot be found. (ie the DLS file for MIDI playback)"sv;

    case FMOD_ERR_PLUGIN_VERSION:
        return u8"A plugin was built with an unsupported SDK version."sv;

    case FMOD_ERR_RECORD:
        return u8"An error occurred trying to initialize the recording device."sv;

    case FMOD_ERR_REVERB_CHANNELGROUP:
        return
            u8"Reverb properties cannot be set on this channel because a parent channelgroup owns the reverb connection."sv;

    case FMOD_ERR_REVERB_INSTANCE:
        return
            u8"Specified instance in FMOD_REVERB_PROPERTIES couldn't be set. Most likely because it is an invalid instance number or the reverb doesn't exist."sv;

    case FMOD_ERR_SUBSOUNDS:
        return
            u8"The error occurred because the sound referenced contains subsounds when it shouldn't have, or it doesn't contain subsounds when it should have. The operation may also not be able to be performed on a parent sound."sv;

    case FMOD_ERR_SUBSOUND_ALLOCATED:
        return
            u8"This subsound is already being used by another sound, you cannot have more than one parent to a sound. Null out the other parent's entry first."sv;

    case FMOD_ERR_SUBSOUND_CANTMOVE:
        return
            u8"Shared subsounds cannot be replaced or moved from their parent stream, such as when the parent stream is an FSB file."sv;

    case FMOD_ERR_TAGNOTFOUND:
        return u8"The specified tag could not be found or there are no tags."sv;

    case FMOD_ERR_TOOMANYCHANNELS:
        return
            u8"The sound created exceeds the allowable input channel count. This can be increased using the 'maxinputchannels' parameter in System::setSoftwareFormat."sv;

    case FMOD_ERR_TRUNCATED:
        return u8"The retrieved string is too long to fit in the supplied buffer and has been truncated."sv;

    case FMOD_ERR_UNIMPLEMENTED:
        return u8"Something in FMOD hasn't been implemented when it should be! contact support!"sv;

    case FMOD_ERR_UNINITIALIZED:
        return u8"This command failed because System::init or System::setDriver was not called."sv;

    case FMOD_ERR_UNSUPPORTED:
        return
            u8"A command issued was not supported by this object. Possibly a plugin without certain callbacks specified."sv;

    case FMOD_ERR_VERSION:
        return u8"The version number of this file format is not supported."sv;

    case FMOD_ERR_EVENT_ALREADY_LOADED:
        return u8"The specified bank has already been loaded."sv;

    case FMOD_ERR_EVENT_LIVEUPDATE_BUSY:
        return u8"The live update connection failed due to the game already being connected."sv;

    case FMOD_ERR_EVENT_LIVEUPDATE_MISMATCH:
        return u8"The live update connection failed due to the game data being out of sync with the tool."sv;

    case FMOD_ERR_EVENT_LIVEUPDATE_TIMEOUT:
        return u8"The live update connection timed out."sv;

    case FMOD_ERR_EVENT_NOTFOUND:
        return u8"The requested event, parameter, bus or vca could not be found."sv;

    case FMOD_ERR_STUDIO_UNINITIALIZED:
        return u8"The Studio::System object is not yet initialized."sv;

    case FMOD_ERR_STUDIO_NOT_LOADED:
        return u8"The specified resource is not loaded, so it can't be unloaded."sv;

    case FMOD_ERR_INVALID_STRING:
        return u8"An invalid string was passed to this function."sv;

    case FMOD_ERR_ALREADY_LOCKED:
        return u8"The specified resource is already locked."sv;

    case FMOD_ERR_NOT_LOCKED:
        return u8"The specified resource is not locked, so it can't be unlocked."sv;

    case FMOD_ERR_RECORD_DISCONNECTED:
        return u8"The specified recording driver has been disconnected."sv;

    case FMOD_ERR_TOOMANYSAMPLES:
        return u8"The length provided exceeds the allowable limit."sv;

    default:
        return u8"Unknown error."sv;
    }
}
}
}
