// RtpFormat.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/rtp/RtpFormat.h"
#include "ppbox/avformat/Error.h"

#include <ppbox/avcodec/avc/AvcFormatType.h>
#include <ppbox/avcodec/aac/AacFormatType.h>

using namespace ppbox::avcodec;

namespace ppbox
{
    namespace avformat
    {

        // http://www.iana.org/assignments/rtp-parameters/rtp-parameters.xhtml

        ppbox::avformat::CodecInfo const RtpFormat::codecs_[] = {
            {StreamType::VIDE,  0,  VideoSubType::AVC,  StreamFormatType::none, 90000,  "H264"},
            {StreamType::VIDE,  0,  VideoSubType::HEVC, StreamFormatType::none, 90000,  "H265"},
            {StreamType::AUDI,  0,  AudioSubType::AAC,  AacFormatType::raw,     1,      "mpeg4-generic"}, 
            {StreamType::AUDI,  0,  AudioSubType::MP1A, StreamFormatType::none, 90000,  "MPA"},
            {StreamType::AUDI,  0,  AudioSubType::MP2A, StreamFormatType::none, 90000,  "MPA"},
            {StreamType::AUDI,  0,  AudioSubType::MP1,  StreamFormatType::none, 90000,  "MPA"},
            {StreamType::AUDI,  0,  AudioSubType::MP2,  StreamFormatType::none, 90000,  "MPA"},
            {StreamType::AUDI,  0,  AudioSubType::MP3,  StreamFormatType::none, 90000,  "MPA"},
            {StreamType::AUDI,  0,  AudioSubType::AC3,  StreamFormatType::none, 1,      "ac3"},
            {StreamType::AUDI,  0,  AudioSubType::EAC3, StreamFormatType::none, 1,      "eac3"},           
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
} // namespace ppbox
