// MkvFormat.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mkv/MkvFormat.h"
#include "ppbox/avformat/Error.h"

#include <ppbox/avcodec/avc/AvcFormatType.h>
#include <ppbox/avcodec/aac/AacFormatType.h>

#include <ppbox/avcodec/Codec.h>

using namespace ppbox::avcodec;

namespace ppbox
{
    namespace avformat
    {

        CodecInfo const MkvFormat::codecs_[] = {
            {StreamType::VIDE,  0,  VideoSubType::AVC1, AvcFormatType::packet,    1000, "V_MPEG4/ISO/AVC"}, 
            {StreamType::AUDI,  1,  AudioSubType::MP4A, AacFormatType::raw,       1000, "A_AAC"}, 
            {StreamType::AUDI,  2,  AudioSubType::MP1,  StreamFormatType::none,   1000, "A_MPEG/L1"}, 
            {StreamType::AUDI,  3,  AudioSubType::MP2,  StreamFormatType::none,   1000, "A_MPEG/L2"}, 
            {StreamType::AUDI,  4,  AudioSubType::MP3,  StreamFormatType::none,   1000, "A_MPEG/L3"}, 
            {StreamType::AUDI,  4,  AudioSubType::MP1A, StreamFormatType::none,   1000, "A_MPEG/L3"}, 
            {StreamType::AUDI,  4,  AudioSubType::MP2A, StreamFormatType::none,   1000, "A_MPEG/L3"}, 
            {StreamType::AUDI,  5,  AudioSubType::AC3,  StreamFormatType::none,   1000, "A_AC3"}, 
            {StreamType::AUDI,  5,  AudioSubType::EAC3, StreamFormatType::none,   1000, "A_EAC3"}, 
        };

        MkvFormat::MkvFormat()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

        struct mkv_equal_type_str
        {
            mkv_equal_type_str(
                char const * str)
                : str_(str)
            {
            }

            bool operator()(
                CodecInfo const & l)
            {
                return str_ == (char const *)l.context;
            }

        private:
            std::string str_;
        };

        CodecInfo const * MkvFormat::codec_from_stream(
            boost::uint32_t category, 
            boost::uint32_t stream_type, 
            void const * context, 
            boost::system::error_code & ec)
        {
            CodecInfo const * codec = std::find_if(codecs_, 
                codecs_ + sizeof(codecs_) / sizeof(codecs_[0]), mkv_equal_type_str((char const *)context));
            if (codec == codecs_ + sizeof(codecs_) / sizeof(codecs_[0])) {
                ec = error::codec_not_support;
            } else {
                ec.clear();
            }
            return codec;
        }

    } // namespace avformat
} // namespace ppbox
