// AsfFormat.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/asf/AsfFormat.h"
#include "ppbox/avformat/asf/AsfEnum.h"

#include <ppbox/avcodec/avc/AvcFormatType.h>
#include <ppbox/avcodec/aac/AacFormatType.h>

using namespace ppbox::avcodec;

namespace ppbox
{
    namespace avformat
    {

        CodecInfo const AsfFormat::codecs_[] = {
            {StreamType::VIDE,  AsfVideoCodec::H264,    VideoSubType::AVC1, AvcFormatType::byte_stream}, 
            {StreamType::VIDE,  AsfVideoCodec::h264,    VideoSubType::AVC1, AvcFormatType::byte_stream}, 
            {StreamType::VIDE,  AsfVideoCodec::WMV3,    VideoSubType::WMV3, StreamFormatType::none}, 
            {StreamType::AUDI,  AsfAudioCodec::AAC,     AudioSubType::MP4A, AacFormatType::raw}, 
            {StreamType::AUDI,  AsfAudioCodec::WMA2,    AudioSubType::WMA2, StreamFormatType::none}, 
        };

        AsfFormat::AsfFormat()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

    } // namespace avformat
} // namespace ppbox