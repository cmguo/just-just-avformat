// MkvFormat.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mkv/MkvFormat.h"

#include <ppbox/avcodec/avc/AvcFormatType.h>
#include <ppbox/avcodec/aac/AacFormatType.h>

using namespace ppbox::avcodec;

namespace ppbox
{
    namespace avformat
    {

        CodecInfo const MkvFormat::codecs_[] = {
            {StreamType::VIDE,  (intptr_t)"V_MPEG4/ISO/AVC",  VideoSubType::AVC1, AvcFormatType::packet}, 
            {StreamType::AUDI,  (intptr_t)"A_AAC",            AudioSubType::MP4A, AacFormatType::raw}, 
        };

        MkvFormat::MkvFormat()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

        struct mkv_codec_info_equal_format
        {
            mkv_codec_info_equal_format(
                boost::uint32_t category, 
                intptr_t format)
                : category_(category)
                , format_(format)
            {
            }

            bool operator()(
                CodecInfo const & l)
            {
                return l.category == category_ && strcmp((char const *)l.format, (char const *)format_) == 0;
            }

        private:
            boost::uint32_t category_;
            intptr_t format_;
        };

        CodecInfo const * MkvFormat::codec_from_format(
            boost::uint32_t category, 
            intptr_t format)
        {
            CodecInfo const * codec = std::find_if(codecs_, codecs_ + ncodec_, mkv_codec_info_equal_format(category, format));
            if (codec == codecs_ + ncodec_)
                codec = NULL;
            return codec;
        }

    } // namespace avformat
} // namespace ppbox
