// Mp4Format.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/Mp4Format.h"
#include "ppbox/avformat/mp4/box/Mp4BoxEnum.h"

#include <ppbox/avcodec/avc/AvcFormatType.h>
#include <ppbox/avcodec/aac/AacFormatType.h>

using namespace ppbox::avcodec;

namespace ppbox
{
    namespace avformat
    {

        CodecInfo const Mp4Format::codecs_[] = {
            {StreamType::VIDE,  MpegObjectType::MPEG4_VISUAL,           VideoSubType::MP4V, StreamFormatType::none,     0}, 
            {StreamType::VIDE,  MAKE_FOURC_TYPE('a', 'v', 'c', '1'),    VideoSubType::AVC1, AvcFormatType::packet,      0}, 
            {StreamType::AUDI,  MpegObjectType::MPEG4_AUDIO,            AudioSubType::MP4A, AacFormatType::raw,         0}, 
            {StreamType::AUDI,  MpegObjectType::MPEG2_AAC_AUDIO_MAIN,   AudioSubType::MP4A, AacFormatType::raw,         0}, 
            {StreamType::AUDI,  MpegObjectType::MPEG2_AAC_AUDIO_LC,     AudioSubType::MP4A, AacFormatType::raw,         0}, 
            {StreamType::AUDI,  MpegObjectType::MPEG2_AAC_AUDIO_SSRP,   AudioSubType::MP4A, AacFormatType::raw,         0}, 
            {StreamType::AUDI,  MpegObjectType::MPEG2_PART3_AUDIO,      AudioSubType::MP2A, StreamFormatType::none,     0}, 
            {StreamType::AUDI,  MpegObjectType::MPEG1_AUDIO,            AudioSubType::MP1A, StreamFormatType::none,     0}, 
            {StreamType::AUDI,  MAKE_FOURC_TYPE('a', 'c', '-', '3'),    AudioSubType::AC3,  StreamFormatType::none,     0}, 
            {StreamType::AUDI,  MAKE_FOURC_TYPE('e', 'c', '-', '3'),    AudioSubType::EAC3, StreamFormatType::none,     0}, 
        };

        Mp4Format::Mp4Format()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

    } // namespace avformat
} // namespace ppbox
