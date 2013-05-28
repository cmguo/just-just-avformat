// FlvFormat.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/flv/FlvFormat.h"
#include "ppbox/avformat/flv/FlvEnum.h"

#include <ppbox/avcodec/avc/AvcFormatType.h>
#include <ppbox/avcodec/aac/AacFormatType.h>

using namespace ppbox::avcodec;

namespace ppbox
{
    namespace avformat
    {

        CodecInfo const FlvFormat::codecs_[] = {
            {StreamType::VIDE,  FlvVideoCodec::H264, VideoSubType::AVC1, AvcFormatType::packet}, 
            {StreamType::AUDI,  FlvSoundCodec::AAC,  AudioSubType::MP4A, AacFormatType::raw}, 
            {StreamType::AUDI,  FlvSoundCodec::MP3,  AudioSubType::MP1A, StreamFormatType::none}, 
        };

        FlvFormat::FlvFormat()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

    } // namespace avformat
} // namespace ppbox