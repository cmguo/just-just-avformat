// MkvFormat.cpp

#include "just/avformat/Common.h"
#include "just/avformat/mkv/MkvFormat.h"
#include "just/avformat/Error.h"

#include <just/avcodec/CodecType.h>
using namespace just::avcodec;

namespace just
{
    namespace avformat
    {

        CodecInfo const MkvFormat::codecs_[] = {
            {StreamType::VIDE,  0,  VideoType::HEVC, HevcFormatType::packet,   1000, "V_MPEGH/ISO/HEVC"}, 
            {StreamType::VIDE,  0,  VideoType::AVC,  AvcFormatType::packet,    1000, "V_MPEG4/ISO/AVC"}, 
            {StreamType::AUDI,  0,  AudioType::AAC,  AacFormatType::raw,       1000, "A_AAC"}, 
            {StreamType::AUDI,  0,  AudioType::MP1,  StreamFormatType::none,   1000, "A_MPEG/L1"}, 
            {StreamType::AUDI,  0,  AudioType::MP2,  StreamFormatType::none,   1000, "A_MPEG/L2"}, 
            {StreamType::AUDI,  0,  AudioType::MP3,  StreamFormatType::none,   1000, "A_MPEG/L3"}, 
            {StreamType::AUDI,  0,  AudioType::MP1A, StreamFormatType::none,   1000, "A_MPEG/L3"}, 
            {StreamType::AUDI,  0,  AudioType::MP2A, StreamFormatType::none,   1000, "A_MPEG/L3"}, 
            {StreamType::AUDI,  0,  AudioType::AC3,  StreamFormatType::none,   1000, "A_AC3"}, 
            {StreamType::AUDI,  0,  AudioType::EAC3, StreamFormatType::none,   1000, "A_EAC3"}, 
            {StreamType::SUBS,  0,  SubtlType::UTF8, StreamFormatType::none,   1000, "S_TEXT/UTF8"}, 
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
                codec = NULL;
            } else {
                ec.clear();
            }
            return codec;
        }

    } // namespace avformat
} // namespace just
