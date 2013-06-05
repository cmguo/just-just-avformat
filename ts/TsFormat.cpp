// TsFormat.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/ts/TsFormat.h"
#include "ppbox/avformat/ts/TsEnum.h"
#include "ppbox/avformat/ts/TsPacket.h"

#include <ppbox/avcodec/avc/AvcFormatType.h>
#include <ppbox/avcodec/aac/AacFormatType.h>

using namespace ppbox::avcodec;

namespace ppbox
{
    namespace avformat
    {

        CodecInfo const TsFormat::codecs_[] = {
            {StreamType::VIDE,  TsStreamType::iso_13818_video,      VideoSubType::AVC1, AvcFormatType::byte_stream, TsPacket::TIME_SCALE}, 
            {StreamType::VIDE,  TsStreamType::iso_13818_2_video,    VideoSubType::MP4V, StreamFormatType::none,     TsPacket::TIME_SCALE}, 
            {StreamType::AUDI,  TsStreamType::iso_13818_7_audio,    AudioSubType::MP4A, AacFormatType::adts,        TsPacket::TIME_SCALE}, 
            {StreamType::AUDI,  TsStreamType::iso_11172_audio,      AudioSubType::MP1A, StreamFormatType::none,     TsPacket::TIME_SCALE}, 
        };

        TsFormat::TsFormat()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

        struct ts_codec_info_equal_stream_type
        {
            ts_codec_info_equal_stream_type(
                intptr_t format)
                : format_(format)
            {
            }

            bool operator()(
                CodecInfo const & l)
            {
                return l.stream_type == format_;
            }

        private:
            intptr_t format_;
        };

        CodecInfo const * TsFormat::codec_from_stream(
            boost::uint32_t category, 
            boost::uint32_t stream_type)
        {
            CodecInfo const * codec = std::find_if(codecs_, codecs_ + ncodec_, ts_codec_info_equal_stream_type(stream_type));
            if (codec == codecs_ + ncodec_)
                codec = NULL;
            return codec;
        }

    } // namespace avformat
} // namespace ppbox
