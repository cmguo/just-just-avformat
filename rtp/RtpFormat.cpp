// RtpFormat.cpp

#include "just/avformat/Common.h"
#include "just/avformat/rtp/RtpFormat.h"
#include "just/avformat/Error.h"

#include <just/avcodec/CodecType.h>
using namespace just::avcodec;

namespace just
{
    namespace avformat
    {

        // http://www.iana.org/assignments/rtp-parameters/rtp-parameters.xhtml

        just::avformat::CodecInfo const RtpFormat::codecs_[] = {
            {StreamType::VIDE,  0,  VideoType::AVC,  StreamFormatType::none, 90000,  "H264"},
            {StreamType::VIDE,  0,  VideoType::HEVC, StreamFormatType::none, 90000,  "H265"},
            {StreamType::AUDI,  0,  AudioType::AAC,  AacFormatType::raw,     1,      "mpeg4-generic"}, 
            {StreamType::AUDI,  0,  AudioType::MP1A, StreamFormatType::none, 90000,  "MPA"},
            {StreamType::AUDI,  0,  AudioType::MP2A, StreamFormatType::none, 90000,  "MPA"},
            {StreamType::AUDI,  0,  AudioType::MP1,  StreamFormatType::none, 90000,  "MPA"},
            {StreamType::AUDI,  0,  AudioType::MP2,  StreamFormatType::none, 90000,  "MPA"},
            {StreamType::AUDI,  0,  AudioType::MP3,  StreamFormatType::none, 90000,  "MPA"},
            {StreamType::AUDI,  0,  AudioType::AC3,  StreamFormatType::none, 1,      "ac3"},
            {StreamType::AUDI,  0,  AudioType::EAC3, StreamFormatType::none, 1,      "eac3"},           
        };

        RtpFormat::RtpFormat()
            : Format(codecs_, sizeof(codecs_) / sizeof(codecs_[0]))
        {
        }

        struct rtp_equal_type_str
        {
            rtp_equal_type_str(
                boost::uint32_t cat, 
                char const * str)
                : cat_(cat)
                , str_(str)
            {
            }

            bool operator()(
                CodecInfo const & l)
            {
                return cat_ == l.category && str_ == (char const *)l.context;
            }

        private:
            boost::uint32_t cat_;
            std::string str_;
        };

        CodecInfo const * RtpFormat::codec_from_stream(
            boost::uint32_t category, 
            boost::uint32_t stream_type, 
            void const * context, 
            boost::system::error_code & ec)
        {
            CodecInfo const * codec = std::find_if(codecs_, 
                codecs_ + sizeof(codecs_) / sizeof(codecs_[0]), rtp_equal_type_str(category, (char const *)context));
            if (codec == codecs_ + sizeof(codecs_) / sizeof(codecs_[0])) {
                ec = error::codec_not_support;
                codec = NULL;
            } else {
                ec.clear();
            }
            return codec;
        }

    } // namespace avformat
} // namespace just
