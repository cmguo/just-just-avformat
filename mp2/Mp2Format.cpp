// Mp2Format.cpp

#include "just/avformat/Common.h"
#include "just/avformat/mp2/Mp2Format.h"
#include "just/avformat/mp2/Mp2Enum.h"
#include "just/avformat/Error.h"

#include <just/avcodec/CodecType.h>
using namespace just::avcodec;

#include <just/avbase/TypeMap.h>

namespace just
{
    namespace avformat
    {

        static Mp2Context const ts_contexts[] = {
            // {0x80,  0,      0},  // PCM_BLURAY
            {0x81,  0x81,   MAKE_FOURC_TYPE('A','C','-','3')},  // AC3
            {0x82,  0x8a,   MAKE_FOURC_TYPE('D','T','S','1')},  // DTS
            // {0x83,  0,      0},  // TRUEHD
            {0x84,  0,      0},  // EAC3
            {0x85,  0,      MAKE_FOURC_TYPE('D','T','S','2')},  // DTS HD
            {0x86,  0,      MAKE_FOURC_TYPE('D','T','S','3')},  // DTS HD MASTER
            {0xa1,  0,      0},  // E-AC3 Secondary Audio
            {0xa2,  0,      0},  // E-AC3 Secondary Audio
        };

        CodecInfo const Mp2Format::codecs_[] = {
            {StreamType::VIDE,  Mp2StreamType::iso_xxxxx_video,    VideoType::HEVC, AvcFormatType::byte_stream, Mp2::TIME_SCALE}, 
            {StreamType::VIDE,  Mp2StreamType::iso_xxxxx_video_2,  VideoType::HEVC, AvcFormatType::byte_stream, Mp2::TIME_SCALE}, 
            {StreamType::VIDE,  Mp2StreamType::iso_14496_10_video, VideoType::AVC,  AvcFormatType::byte_stream, Mp2::TIME_SCALE}, 
            {StreamType::VIDE,  Mp2StreamType::iso_14496_2_video,  VideoType::MP4V, StreamFormatType::none,     Mp2::TIME_SCALE}, 
            {StreamType::VIDE,  Mp2StreamType::iso_13818_2_video,  VideoType::MP2V, StreamFormatType::none,     Mp2::TIME_SCALE}, 
            {StreamType::AUDI,  Mp2StreamType::iso_13818_7_audio,  AudioType::AAC,  AacFormatType::adts,        Mp2::TIME_SCALE}, 
            {StreamType::AUDI,  Mp2StreamType::iso_11172_audio,    AudioType::MP1A, StreamFormatType::none,     Mp2::TIME_SCALE}, 
            {StreamType::AUDI,  Mp2StreamType::iso_11172_audio,    AudioType::MP1,  StreamFormatType::none,     Mp2::TIME_SCALE}, 
            {StreamType::AUDI,  Mp2StreamType::iso_11172_audio,    AudioType::MP2,  StreamFormatType::none,     Mp2::TIME_SCALE}, 
            {StreamType::AUDI,  Mp2StreamType::iso_11172_audio,    AudioType::MP3,  StreamFormatType::none,     Mp2::TIME_SCALE}, 
            {StreamType::AUDI,  Mp2StreamType::iso_13818_3_audio,  AudioType::MP2A, StreamFormatType::none,     Mp2::TIME_SCALE}, 
            {StreamType::AUDI,  0,  AudioType::AC3,  StreamFormatType::none, Mp2::TIME_SCALE,   ts_contexts}, 
            {StreamType::AUDI,  0,  AudioType::DTS,  StreamFormatType::none, Mp2::TIME_SCALE,   ts_contexts + 1}, 
            {StreamType::AUDI,  0,  AudioType::EAC3, StreamFormatType::none, Mp2::TIME_SCALE,   ts_contexts + 2}, 
            {StreamType::AUDI,  0,  AudioType::DTS,  StreamFormatType::none, Mp2::TIME_SCALE,   ts_contexts + 3}, 
            {StreamType::AUDI,  0,  AudioType::DTS,  StreamFormatType::none, Mp2::TIME_SCALE,   ts_contexts + 4}, 
            {StreamType::AUDI,  0,  AudioType::EAC3, StreamFormatType::none, Mp2::TIME_SCALE,   ts_contexts + 5}, 
            {StreamType::AUDI,  0,  AudioType::EAC3, StreamFormatType::none, Mp2::TIME_SCALE,   ts_contexts + 6}, 
        };

        Mp2Format::Mp2Format()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

        CodecInfo const * Mp2Format::codec_from_stream(
            boost::uint32_t category, 
            boost::uint32_t stream_type, 
            void const * context, 
            boost::system::error_code & ec)
        {
            CodecInfo const * codec = just::avbase::type_map_find(codecs_, 
                &CodecInfo::stream_type, stream_type);
            if (codec) {
                ec.clear();
                return codec;
            }
            if (context == NULL) {
                ec = error::codec_not_support;
                return NULL;
            }
            Mp2Context const * tsc = (Mp2Context const *)context;
            Mp2Context const * tsc2 = NULL;
            if (tsc->regd_type) {
                tsc2 = just::avbase::type_map_find(
                    ts_contexts, 
                    &Mp2Context::regd_type, tsc->regd_type);
            }
            if (tsc2 == NULL && tsc->hdmv_type) {
                tsc2 = just::avbase::type_map_find(
                    ts_contexts, 
                    &Mp2Context::hdmv_type, tsc->hdmv_type);
            }
            if (tsc2 == NULL) {
                tsc2 = just::avbase::type_map_find(
                    ts_contexts, 
                    &Mp2Context::misc_type, (boost::uint8_t)stream_type);
            }
            if (tsc2 == NULL) {
                ec = error::codec_not_support;
            } else {
                codec = just::avbase::type_map_find(codecs_, 
                    &CodecInfo::context, (void const *)tsc2);
                assert(codec);
                ec.clear();
            }
            return codec;
        }

    } // namespace avformat
} // namespace just
