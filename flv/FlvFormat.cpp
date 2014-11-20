// FlvFormat.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/flv/FlvFormat.h"
#include "ppbox/avformat/flv/FlvEnum.h"

#include <ppbox/avcodec/CodecType.h>
using namespace ppbox::avcodec;

namespace ppbox
{
    namespace avformat
    {

        CodecInfo const FlvFormat::codecs_[] = {
            {StreamType::VIDE,  FlvVideoCodec::H263, VideoType::H263, AvcFormatType::packet,     1000}, 
            {StreamType::VIDE,  FlvVideoCodec::VP6,  VideoType::VP6,  AvcFormatType::packet,     1000}, 
            {StreamType::VIDE,  FlvVideoCodec::H264, VideoType::AVC,  AvcFormatType::packet,     1000}, 
            {StreamType::VIDE,  FlvVideoCodec::H265, VideoType::HEVC, AvcFormatType::packet,     1000}, 
            {StreamType::AUDI,  FlvSoundCodec::AAC,  AudioType::AAC,  AacFormatType::raw,        1000}, 
            {StreamType::AUDI,  FlvSoundCodec::MP3,  AudioType::MP3,  StreamFormatType::none,    1000}, 
            {StreamType::AUDI,  FlvSoundCodec::MP3,  AudioType::MP1,  StreamFormatType::none,    1000}, 
            {StreamType::AUDI,  FlvSoundCodec::MP3,  AudioType::MP2,  StreamFormatType::none,    1000}, 
            {StreamType::AUDI,  FlvSoundCodec::MP3,  AudioType::MP1A, StreamFormatType::none,    1000}, 
            {StreamType::AUDI,  FlvSoundCodec::MP3,  AudioType::MP2A, StreamFormatType::none,    1000}, 
        };

        FlvFormat::FlvFormat()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

    } // namespace avformat
} // namespace ppbox
