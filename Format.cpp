// Format.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/Format.h"

#include <ppbox/common/ClassRegister.h>
#include "ppbox/avformat/flv/FlvFormat.h"
#include "ppbox/avformat/asf/AsfFormat.h"
#include "ppbox/avformat/ts/TsFormat.h"
#include "ppbox/avformat/mkv/MkvFormat.h"

#include <ppbox/avcodec/Codec.h>

namespace ppbox
{
    namespace avformat
    {

        Format::Format()
            : codecs_(NULL)
            , ncodec_(0)
        {
        }

        Format::Format(
            CodecInfo const * codecs, 
            size_t ncodec)
            : codecs_(codecs)
            , ncodec_(ncodec)
        {
        }

        Format::~Format()
        {
        }

        struct codec_info_equal_format
        {
            codec_info_equal_format(
                boost::uint32_t category, 
                intptr_t format)
                : category_(category)
                , format_(format)
            {
            }

            bool operator()(
                CodecInfo const & l)
            {
                return l.category == category_ && l.format == format_;
            }

        private:
            boost::uint32_t category_;
            intptr_t format_;
        };

        CodecInfo const * Format::codec_from_format(
            boost::uint32_t category, 
            intptr_t format)
        {
            CodecInfo const * codec = std::find_if(codecs_, codecs_ + ncodec_, codec_info_equal_format(category, format));
            if (codec == codecs_ + ncodec_)
                codec = NULL;
            return codec;
        }

        struct codec_info_equal_codec
        {
            codec_info_equal_codec(
                boost::uint32_t category, 
                boost::uint32_t codec)
                : category_(category)
                , codec_(codec)
            {
            }

            bool operator()(
                CodecInfo const & l)
            {
                return l.category == category_ && l.codec == codec_;
            }

        private:
            boost::uint32_t category_;
            boost::uint32_t codec_;
        };

        CodecInfo const * Format::codec_from_codec(
            boost::uint32_t category, 
            boost::uint32_t c)
        {
            CodecInfo const * codec = std::find_if(codecs_, codecs_ + ncodec_, codec_info_equal_codec(category, c));
            if (codec == codecs_ + ncodec_)
                codec = NULL;
            return codec;
        }

        bool Format::finish_stream_info(
            ppbox::avbase::StreamInfo & info, 
            intptr_t format)
        {
            CodecInfo const * codec = codec_from_format(info.type, format);
            if (codec) {
                info.type = codec->category;
                info.sub_type = codec->codec;
                info.format_type = codec->codec_format;
                return ppbox::avcodec::Codec::static_finish_stream_info(info);
            } else {
                return false;
            }
        }

        bool Format::finish_stream_info(
            ppbox::avbase::StreamInfo & info, 
            boost::uint32_t type, 
            intptr_t f)
        {
            Format * format = factory_type::create(type);
            if (format) {
                bool b = format->finish_stream_info(info, f);
                delete format;
                return b;
            } else {
                return false;
            }
        }

    } // namespace avformat
} // namespace ppbox
