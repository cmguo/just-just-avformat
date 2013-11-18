// Mp4Format.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/Mp4Format.h"
#include <bento4/Core/Ap4SampleDescription.h>

#include <ppbox/avcodec/avc/AvcFormatType.h>
#include <ppbox/avcodec/aac/AacFormatType.h>

using namespace ppbox::avcodec;

namespace ppbox
{
    namespace avformat
    {

        const AP4_MpegSampleDescription::OTI AP4_OTI_MPEG4_PART10_VISUAL         = 0x21; //AVC

        CodecInfo const Mp4Format::codecs_[] = {
            {StreamType::VIDE,  AP4_OTI_MPEG4_VISUAL,           VideoSubType::MP4V, StreamFormatType::none,     0}, 
            {StreamType::VIDE,  AP4_OTI_MPEG4_PART10_VISUAL,    VideoSubType::AVC1, AvcFormatType::packet,      0}, 
            {StreamType::AUDI,  AP4_OTI_MPEG4_AUDIO,            AudioSubType::MP4A, AacFormatType::raw,         0}, 
            {StreamType::AUDI,  AP4_OTI_MPEG2_AAC_AUDIO_MAIN,   AudioSubType::MP4A, AacFormatType::raw,         0}, 
            {StreamType::AUDI,  AP4_OTI_MPEG2_AAC_AUDIO_LC,     AudioSubType::MP4A, AacFormatType::raw,         0}, 
            {StreamType::AUDI,  AP4_OTI_MPEG2_AAC_AUDIO_SSRP,   AudioSubType::MP4A, AacFormatType::raw,         0}, 
            {StreamType::AUDI,  AP4_OTI_MPEG2_PART3_AUDIO,      AudioSubType::MP2A, StreamFormatType::none,     0}, 
            {StreamType::AUDI,  AP4_OTI_MPEG1_AUDIO,            AudioSubType::MP1A, StreamFormatType::none,     0}, 
        };

        Mp4Format::Mp4Format()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

    } // namespace avformat
} // namespace ppbox
