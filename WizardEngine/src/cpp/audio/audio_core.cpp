//
// Created by mecha on 22.04.2022.
//

#include <audio/audio_core.h>
#include <io/Logger.h>

#include <AL/al.h>

namespace engine::audio {

    bool checkError() {
        ALenum error = alGetError();

        if (error != AL_NO_ERROR) {
            switch(error) {
                case AL_INVALID_NAME:
                    ENGINE_ERR("AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function");
                    break;
                case AL_INVALID_ENUM:
                    ENGINE_ERR("AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function");
                    break;
                case AL_INVALID_VALUE:
                    ENGINE_ERR("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function");
                    break;
                case AL_INVALID_OPERATION:
                    ENGINE_ERR("AL_INVALID_OPERATION: the requested operation is not valid");
                    break;
                case AL_OUT_OF_MEMORY:
                    ENGINE_ERR("AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory");
                    break;
                default: ENGINE_ERR("Unknown AL error : {0}", error);
            }

            return false;
        }

        return true;
    }

    bool checkError(ALCdevice* device) {
        ALCenum error = alcGetError(device);

        if (error != ALC_NO_ERROR) {
            switch(error) {
                case ALC_INVALID_VALUE:
                    ENGINE_ERR("ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function");
                    break;
                case ALC_INVALID_DEVICE:
                    ENGINE_ERR("ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function");
                    break;
                case ALC_INVALID_CONTEXT:
                    ENGINE_ERR("ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function");
                    break;
                case ALC_INVALID_ENUM:
                    ENGINE_ERR("ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function");
                    break;
                case ALC_OUT_OF_MEMORY:
                    ENGINE_ERR("ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function");
                    break;
                default: ENGINE_ERR("Unknown ALC error : {0}", error);
            }

            return false;
        }

        return true;
    }

}