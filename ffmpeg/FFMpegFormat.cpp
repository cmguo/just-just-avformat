// FFMpegFormat.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/ffmpeg/FFMpegFormat.h"

#include <ppbox/avcodec/avc/AvcFormatType.h>
#include <ppbox/avcodec/aac/AacFormatType.h>
using namespace ppbox::avcodec;

extern "C" {
#define UINT64_C(c)   c ## ULL
#include <libavformat/avformat.h>
}

namespace ppbox
{
    namespace avformat
    {

        CodecInfo const FFMpegFormat::codecs_[] = {
            {StreamType::VIDE,  AV_CODEC_ID_H264, VideoSubType::AVC1,   StreamFormatType::none,    0}, 
            {StreamType::AUDI,  AV_CODEC_ID_AAC,  AudioSubType::MP4A,   StreamFormatType::none,    0}, 
            {StreamType::AUDI,  AV_CODEC_ID_MP3,  AudioSubType::MP1A,   StreamFormatType::none,    0}, 
        };

        FFMpegFormat::FFMpegFormat()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

    } // namespace avformat
} // namespace ppbox
