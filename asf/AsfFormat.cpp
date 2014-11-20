// AsfFormat.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/asf/AsfFormat.h"
#include "ppbox/avformat/asf/AsfEnum.h"

#include <ppbox/avcodec/CodecType.h>
using namespace ppbox::avcodec;

namespace ppbox
{
    namespace avformat
    {

        CodecInfo const AsfFormat::codecs_[] = {
            {StreamType::VIDE,  AsfVideoCodec::H264,    VideoType::AVC,  AvcFormatType::byte_stream, 1000}, 
            {StreamType::VIDE,  AsfVideoCodec::h264,    VideoType::AVC,  AvcFormatType::byte_stream, 1000}, 
            {StreamType::VIDE,  AsfVideoCodec::WMV3,    VideoType::WMV3, StreamFormatType::none,     1000}, 
            {StreamType::AUDI,  AsfAudioCodec::MP3,     AudioType::MP3,  StreamFormatType::none,     1000}, 
            {StreamType::AUDI,  AsfAudioCodec::MP3,     AudioType::MP1,  StreamFormatType::none,     1000}, 
            {StreamType::AUDI,  AsfAudioCodec::MP3,     AudioType::MP2,  StreamFormatType::none,     1000}, 
            {StreamType::AUDI,  AsfAudioCodec::MP3,     AudioType::MP1A, StreamFormatType::none,     1000}, 
            {StreamType::AUDI,  AsfAudioCodec::MP3,     AudioType::MP2A, StreamFormatType::none,     1000}, 
            {StreamType::AUDI,  AsfAudioCodec::AAC,     AudioType::AAC,  AacFormatType::raw,         1000}, 
            {StreamType::AUDI,  AsfAudioCodec::WMA2,    AudioType::WMA2, StreamFormatType::none,     1000}, 
        };

        AsfFormat::AsfFormat()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

    } // namespace avformat
} // namespace ppbox
