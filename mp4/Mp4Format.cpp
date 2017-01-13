// Mp4Format.cpp

#include "just/avformat/Common.h"
#include "just/avformat/mp4/Mp4Format.h"
#include "just/avformat/mp4/box/Mp4BoxEnum.h"
#include "just/avformat/Error.h"

#include <just/avcodec/CodecType.h>
using namespace just::avcodec;

#include <just/avbase/TypeMap.h>

namespace just
{
    namespace avformat
    {

        CodecInfo const Mp4Format::codecs_[] = {
            {StreamType::VIDE,  Mp4CodecType::hvc1, VideoType::HEVC, AvcFormatType::packet,  0,  (void*)MAKE_FOURC_TYPE('h', 'v', 'c', 'C')}, 
            {StreamType::VIDE,  Mp4CodecType::hev1, VideoType::HEVC, AvcFormatType::packet,  0,  (void*)MAKE_FOURC_TYPE('h', 'v', 'c', 'C')}, 
            {StreamType::VIDE,  Mp4CodecType::avc1, VideoType::AVC,  AvcFormatType::packet,  0,  (void*)MAKE_FOURC_TYPE('a', 'v', 'c', 'C')}, 
            {StreamType::VIDE,  Mp4CodecType::avc2, VideoType::AVC,  AvcFormatType::packet,  0,  (void*)MAKE_FOURC_TYPE('a', 'v', 'c', 'C')}, 
            {StreamType::VIDE,  Mp4CodecType::avc3, VideoType::AVC,  AvcFormatType::packet,  0,  (void*)MAKE_FOURC_TYPE('a', 'v', 'c', 'C')}, 
            {StreamType::VIDE,  Mp4CodecType::avc4, VideoType::AVC,  AvcFormatType::packet,  0,  (void*)MAKE_FOURC_TYPE('a', 'v', 'c', 'C')}, 
            {StreamType::VIDE,  Mp4CodecType::mp4v, VideoType::MP4V, StreamFormatType::none, 0,  (void*)Mp4ObjectType::MPEG4_VISUAL}, 
            {StreamType::VIDE,  Mp4CodecType::encv, VideoType::AVC,  AvcFormatType::packet,  0,  (void*)MAKE_FOURC_TYPE('a', 'v', 'c', 'C')}, 
            {StreamType::AUDI,  Mp4CodecType::mp4a, AudioType::AAC,  AacFormatType::raw,     1,  (void*)Mp4ObjectType::MPEG4_AUDIO}, 
            {StreamType::AUDI,	Mp4CodecType::enca, AudioType::AAC,  AacFormatType::raw,	 1,  (void*)Mp4ObjectType::MPEG4_AUDIO}, 
            {StreamType::AUDI,  Mp4CodecType::mp4a, AudioType::AAC,  AacFormatType::raw,     1,  (void*)Mp4ObjectType::MPEG2_AAC_AUDIO_MAIN}, 
            {StreamType::AUDI,  Mp4CodecType::mp4a, AudioType::AAC,  AacFormatType::raw,     1,  (void*)Mp4ObjectType::MPEG2_AAC_AUDIO_LC}, 
            {StreamType::AUDI,  Mp4CodecType::mp4a, AudioType::AAC,  AacFormatType::raw,     1,  (void*)Mp4ObjectType::MPEG2_AAC_AUDIO_SSRP}, 
            {StreamType::AUDI,  Mp4CodecType::mp4a, AudioType::MP2A, StreamFormatType::none, 1,  (void*)Mp4ObjectType::MPEG2_PART3_AUDIO}, 
            {StreamType::AUDI,  Mp4CodecType::mp4a, AudioType::MP1A, StreamFormatType::none, 1,  (void*)Mp4ObjectType::MPEG1_AUDIO}, 
            {StreamType::AUDI,  Mp4CodecType::ac_3, AudioType::AC3,  StreamFormatType::none, 1,  0}, 
            {StreamType::AUDI,  Mp4CodecType::ec_3, AudioType::EAC3, StreamFormatType::none, 1,  (void*)MAKE_FOURC_TYPE('d',  'e', 'c', '3')},
        };

        Mp4Format::Mp4Format()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

        CodecInfo const * Mp4Format::codec_from_stream(
            boost::uint32_t category, 
            boost::uint32_t stream_type, 
            void const * context, 
            boost::system::error_code & ec)
        {
            if (context && (intptr_t)context < 256) {
                CodecInfo const * codec = just::avbase::type_map_find(
                    codecs_, ncodec_, 
                    &CodecInfo::category, category, 
                    &CodecInfo::stream_type, stream_type, 
                    &CodecInfo::context, context);
                if (codec == NULL) {
                    ec = error::codec_not_support;
                } else {
                    ec.clear();
                }
                return codec;
            }
            return Format::codec_from_stream(category, stream_type, context, ec);
        }

    } // namespace avformat
} // namespace just
