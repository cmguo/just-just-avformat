// Format.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/Format.h"
#include "ppbox/avformat/Error.h"

#include <util/tools/ClassRegister.h>
#include "ppbox/avformat/mp4/Mp4Format.h"
#include "ppbox/avformat/flv/FlvFormat.h"
#include "ppbox/avformat/asf/AsfFormat.h"
#include "ppbox/avformat/ts/TsFormat.h"
#include "ppbox/avformat/mkv/MkvFormat.h"
#include "ppbox/avformat/ffmpeg/FFMpegFormat.h"

#include <ppbox/avcodec/Codec.h>

#include <ppbox/avbase/TypeMap.h>

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
            boost::uint32_t stream_type, 
			void const * context, 
            boost::system::error_code & ec)
        {
            CodecInfo const * codec = ppbox::avbase::type_map_find(
                codecs_, ncodec_, 
                &CodecInfo::category, category, 
                &CodecInfo::stream_type, stream_type);
            if (codec == NULL) {
                ec = error::codec_not_support;
            } else {
                ec.clear();
            }
            return codec;
        }

        CodecInfo const * Format::codec_from_codec(
            boost::uint32_t category, 
            boost::uint32_t codec_type, 
			void const * context, 
            boost::system::error_code & ec)
        {
            CodecInfo const * codec = ppbox::avbase::type_map_find(
                codecs_, ncodec_, 
                &CodecInfo::category, category, 
                &CodecInfo::codec_type, codec_type);
            if (codec == NULL) {
                ec = error::codec_not_support;
            } else {
                ec.clear();
            }
            return codec;
        };

        CodecInfo const * Format::codec_from_stream(
            boost::uint32_t category, 
            boost::uint32_t stream_type, 
            boost::system::error_code & ec)
        {
			return codec_from_stream(category, stream_type, NULL, ec);
        }

        CodecInfo const * Format::codec_from_codec(
            boost::uint32_t category, 
            boost::uint32_t codec_type, 
            boost::system::error_code & ec)
        {
			return codec_from_codec(category, codec_type, NULL, ec);
        }

        bool Format::finish_from_stream(
            ppbox::avbase::StreamInfo & info, 
            boost::system::error_code & ec)
        {
            CodecInfo const * codec = codec_from_stream(info.type, info.sub_type, info.context, ec);
            if (codec) {
                info.type = codec->category;
                info.sub_type = codec->codec_type;
                info.format_type = codec->codec_format;
				info.context = codec->context;
                if (info.time_scale == 0 && codec->time_scale != 0)
                    info.time_scale = codec->time_scale;
                return ppbox::avcodec::Codec::static_finish_stream_info(info, ec);
            }
            return false;
        }

        bool Format::finish_from_codec(
            ppbox::avbase::StreamInfo & info, 
            boost::system::error_code & ec)
        {
            CodecInfo const * codec = codec_from_codec(info.type, info.sub_type, info.context, ec);
            if (codec) {
                info.type = codec->category;
                info.sub_type = codec->codec_type;
                info.format_type = codec->codec_format;
				info.context = codec->context;
                if (info.time_scale == 0 && codec->time_scale != 0)
                    info.time_scale = codec->time_scale;
                return true;
            } else {
                return false;
            }
        }

        bool Format::finish_from_stream(
            ppbox::avbase::StreamInfo & info, 
            std::string const & format_str, 
            boost::uint32_t stream_type, 
            boost::system::error_code & ec)
        {
            Format * format = FormatFactory::create(format_str, ec);
            if (format) {
                info.sub_type = stream_type;
                bool b = format->finish_from_stream(info, ec);
                delete format;
                return b;
            } else {
                return false;
            }
        }

        boost::system::error_code FormatTraits::error_not_found()
        {
            return error::format_not_support;
        }

    } // namespace avformat
} // namespace ppbox
