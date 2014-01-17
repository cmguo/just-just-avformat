// MkvFormat.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mkv/MkvFormat.h"

#include <ppbox/avcodec/avc/AvcFormatType.h>
#include <ppbox/avcodec/aac/AacFormatType.h>

#include <ppbox/avcodec/Codec.h>

using namespace ppbox::avcodec;

namespace ppbox
{
    namespace avformat
    {

        CodecInfo const MkvFormat::codecs_[] = {
            {StreamType::VIDE,  0,  VideoSubType::AVC1, AvcFormatType::packet,    1000}, 
            {StreamType::AUDI,  1,  AudioSubType::MP4A, AacFormatType::raw,       1000}, 
            {StreamType::AUDI,  2,  AudioSubType::MP1,  StreamFormatType::none,   1000}, 
            {StreamType::AUDI,  3,  AudioSubType::MP2,  StreamFormatType::none,   1000}, 
            {StreamType::AUDI,  4,  AudioSubType::MP3,  StreamFormatType::none,   1000}, 
            {StreamType::AUDI,  4,  AudioSubType::MP1A, StreamFormatType::none,   1000}, 
            {StreamType::AUDI,  4,  AudioSubType::MP2A, StreamFormatType::none,   1000}, 
        };

        char const * const MkvFormat::type_strs[] = {
            "V_MPEG4/ISO/AVC", 
            "A_AAC", 
            "A_MPEG/L1", 
            "A_MPEG/L2", 
            "A_MPEG/L3", 
        };

        MkvFormat::MkvFormat()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

        struct mkv_equal_type_str
        {
            mkv_equal_type_str(
                std::string const & str)
                : str_(str)
            {
            }

            bool operator()(
                char const * l)
            {
                return str_ == l;
            }

        private:
            std::string const & str_;
        };

        boost::uint32_t MkvFormat::stream_type(
            std::string const & type_str)
        {
            char const * const * str = std::find_if(type_strs, 
                type_strs + sizeof(type_strs) / sizeof(type_strs[0]), mkv_equal_type_str(type_str));
            return (boost::uint32_t)(str - type_strs);
        }


        char const * MkvFormat::stream_type_str(
            boost::uint32_t type)
        {
            assert(type < sizeof(type_strs) / sizeof(type_strs[0]));
            return type_strs[type];
        }

    } // namespace avformat
} // namespace ppbox
