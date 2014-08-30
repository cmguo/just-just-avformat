// Mp4Format.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/Mp4Format.h"
#include "ppbox/avformat/mp4/box/Mp4BoxEnum.h"
#include "ppbox/avformat/Error.h"

#include <ppbox/avcodec/avc/AvcFormatType.h>
#include <ppbox/avcodec/aac/AacFormatType.h>

#include <ppbox/avbase/TypeMap.h>
using namespace ppbox::avcodec;

namespace ppbox
{
    namespace avformat
    {

        CodecInfo const Mp4Format::codecs_[] = {
            {StreamType::VIDE,  Mp4CodecType::avc1, VideoSubType::AVC,  AvcFormatType::packet,  0,  (void*)MAKE_FOURC_TYPE('a', 'v', 'c', 'C')}, 
            {StreamType::VIDE,  Mp4CodecType::avc2, VideoSubType::AVC,  AvcFormatType::packet,  0,  (void*)MAKE_FOURC_TYPE('a', 'v', 'c', 'C')}, 
            {StreamType::VIDE,  Mp4CodecType::avc3, VideoSubType::AVC,  AvcFormatType::packet,  0,  (void*)MAKE_FOURC_TYPE('a', 'v', 'c', 'C')}, 
            {StreamType::VIDE,  Mp4CodecType::avc4, VideoSubType::AVC,  AvcFormatType::packet,  0,  (void*)MAKE_FOURC_TYPE('a', 'v', 'c', 'C')}, 
            {StreamType::VIDE,  Mp4CodecType::mp4v, VideoSubType::MP4V, StreamFormatType::none, 0,  (void*)Mp4ObjectType::MPEG4_VISUAL}, 
            {StreamType::AUDI,  Mp4CodecType::mp4a, AudioSubType::AAC,  AacFormatType::raw,     1,  (void*)Mp4ObjectType::MPEG4_AUDIO}, 
            {StreamType::AUDI,  Mp4CodecType::mp4a, AudioSubType::AAC,  AacFormatType::raw,     1,  (void*)Mp4ObjectType::MPEG2_AAC_AUDIO_MAIN}, 
            {StreamType::AUDI,  Mp4CodecType::mp4a, AudioSubType::AAC,  AacFormatType::raw,     1,  (void*)Mp4ObjectType::MPEG2_AAC_AUDIO_LC}, 
            {StreamType::AUDI,  Mp4CodecType::mp4a, AudioSubType::AAC,  AacFormatType::raw,     1,  (void*)Mp4ObjectType::MPEG2_AAC_AUDIO_SSRP}, 
            {StreamType::AUDI,  Mp4CodecType::mp4a, AudioSubType::MP2A, StreamFormatType::none, 1,  (void*)Mp4ObjectType::MPEG2_PART3_AUDIO}, 
            {StreamType::AUDI,  Mp4CodecType::mp4a, AudioSubType::MP1A, StreamFormatType::none, 1,  (void*)Mp4ObjectType::MPEG1_AUDIO}, 
            {StreamType::AUDI,  Mp4CodecType::ac_3, AudioSubType::AC3,  StreamFormatType::none, 1,  0}, 
            {StreamType::AUDI,  Mp4CodecType::ec_3, AudioSubType::EAC3, StreamFormatType::none, 1,  0}, 
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
                CodecInfo const * codec = ppbox::avbase::type_map_find(
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
} // namespace ppbox
