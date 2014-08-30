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
            {StreamType::VIDE,  AsfVideoCodec::H264,    VideoSubType::AVC,  AvcFormatType::byte_stream, 1000}, 
            {StreamType::VIDE,  AsfVideoCodec::h264,    VideoSubType::AVC,  AvcFormatType::byte_stream, 1000}, 
            {StreamType::VIDE,  AsfVideoCodec::WMV3,    VideoSubType::WMV3, StreamFormatType::none,     1000}, 
            {StreamType::AUDI,  AsfAudioCodec::MP3,     AudioSubType::MP3,  StreamFormatType::none,     1000}, 
            {StreamType::AUDI,  AsfAudioCodec::MP3,     AudioSubType::MP1,  StreamFormatType::none,     1000}, 
            {StreamType::AUDI,  AsfAudioCodec::MP3,     AudioSubType::MP2,  StreamFormatType::none,     1000}, 
            {StreamType::AUDI,  AsfAudioCodec::MP3,     AudioSubType::MP1A, StreamFormatType::none,     1000}, 
            {StreamType::AUDI,  AsfAudioCodec::MP3,     AudioSubType::MP2A, StreamFormatType::none,     1000}, 
            {StreamType::AUDI,  AsfAudioCodec::AAC,     AudioSubType::AAC,  AacFormatType::raw,         1000}, 
            {StreamType::AUDI,  AsfAudioCodec::WMA2,    AudioSubType::WMA2, StreamFormatType::none,     1000}, 
        };

        AsfFormat::AsfFormat()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

    } // namespace avformat
} // namespace ppbox
