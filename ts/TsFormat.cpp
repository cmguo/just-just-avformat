// TsFormat.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/ts/TsFormat.h"
#include "ppbox/avformat/ts/TsEnum.h"
#include "ppbox/avformat/ts/TsPacket.h"
#include "ppbox/avformat/Error.h"

#include <ppbox/avcodec/avc/AvcFormatType.h>
#include <ppbox/avcodec/aac/AacFormatType.h>
using namespace ppbox::avcodec;

#include <ppbox/avbase/TypeMap.h>

namespace ppbox
{
    namespace avformat
    {

        CodecInfo const TsFormat::codecs_[] = {
            {StreamType::VIDE,  TsStreamType::iso_13818_video,      VideoSubType::AVC1, AvcFormatType::byte_stream, TsPacket::TIME_SCALE}, 
            {StreamType::VIDE,  TsStreamType::iso_13818_2_video,    VideoSubType::MP4V, StreamFormatType::none,     TsPacket::TIME_SCALE}, 
            {StreamType::AUDI,  TsStreamType::iso_13818_7_audio,    AudioSubType::MP4A, AacFormatType::adts,        TsPacket::TIME_SCALE}, 
            {StreamType::AUDI,  TsStreamType::iso_11172_audio,      AudioSubType::MP1A, StreamFormatType::none,     TsPacket::TIME_SCALE}, 
            {StreamType::AUDI,  TsStreamType::iso_13818_3_audio,    AudioSubType::MP2A, StreamFormatType::none,     TsPacket::TIME_SCALE}, 
        };

        TsFormat::TsFormat()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

        CodecInfo const * TsFormat::codec_from_stream(
            boost::uint32_t category, 
            boost::uint32_t stream_type, 
            boost::system::error_code & ec)
        {
            CodecInfo const * codec = ppbox::avbase::type_map_find(
                codecs_, ncodec_, 
                &CodecInfo::stream_type, stream_type);
            if (codec == NULL) {
                ec = error::codec_not_support;
            } else {
                ec.clear();
            }
            return codec;
        }

    } // namespace avformat
} // namespace ppbox
