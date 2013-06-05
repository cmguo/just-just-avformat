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

        struct codec_info_equal_stream_type
        {
            codec_info_equal_stream_type(
                boost::uint32_t category, 
                boost::uint32_t stream_type)
                : category_(category)
                , stream_type_(stream_type)
            {
            }

            bool operator()(
                CodecInfo const & l)
            {
                return l.category == category_ && l.stream_type == stream_type_;
            }

        private:
            boost::uint32_t category_;
            boost::uint32_t stream_type_;
        };

        CodecInfo const * Format::codec_from_stream(
            boost::uint32_t category, 
            boost::uint32_t stream_type)
        {
            CodecInfo const * codec = std::find_if(codecs_, codecs_ + ncodec_, codec_info_equal_stream_type(category, stream_type));
            if (codec == codecs_ + ncodec_)
                codec = NULL;
            return codec;
        }

        struct codec_info_equal_codec_type
        {
            codec_info_equal_codec_type(
                boost::uint32_t category, 
                boost::uint32_t codec_type)
                : category_(category)
                , codec_type_(codec_type)
            {
            }

            bool operator()(
                CodecInfo const & l)
            {
                return l.category == category_ && l.codec_type == codec_type_;
            }

        private:
            boost::uint32_t category_;
            boost::uint32_t codec_type_;
        };

        CodecInfo const * Format::codec_from_codec(
            boost::uint32_t category, 
            boost::uint32_t codec_type)
        {
            CodecInfo const * codec = std::find_if(codecs_, codecs_ + ncodec_, codec_info_equal_codec_type(category, codec_type));
            if (codec == codecs_ + ncodec_)
                codec = NULL;
            return codec;
        }

        bool Format::finish_from_stream(
            ppbox::avbase::StreamInfo & info)
        {
            CodecInfo const * codec = codec_from_stream(info.type, info.sub_type);
            if (codec) {
                info.type = codec->category;
                info.sub_type = codec->codec_type;
                info.format_type = codec->codec_format;
                info.time_scale = codec->time_scale;
                return ppbox::avcodec::Codec::static_finish_stream_info(info);
            }
            return false;
        }

        bool Format::finish_from_codec(
            ppbox::avbase::StreamInfo & info)
        {
            CodecInfo const * codec = codec_from_codec(info.type, info.sub_type);
            if (codec) {
                info.type = codec->category;
                info.sub_type = codec->codec_type;
                info.format_type = codec->codec_format;
                info.time_scale = codec->time_scale;
                return true;
            } else {
                return false;
            }
        }

        bool Format::finish_from_stream(
            ppbox::avbase::StreamInfo & info, 
            std::string const & format_str, 
            boost::uint32_t stream_type)
        {
            Format * format = factory_type::create(format_str);
            if (format) {
                info.sub_type = stream_type;
                bool b = format->finish_from_stream(info);
                delete format;
                return b;
            } else {
                return false;
            }
        }

    } // namespace avformat
} // namespace ppbox
