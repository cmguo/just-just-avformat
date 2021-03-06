// Format.h

#ifndef _JUST_AVFORMAT_FORMAT_H_
#define _JUST_AVFORMAT_FORMAT_H_

#include <just/avbase/StreamInfo.h>

#include <util/tools/ClassFactory.h>

namespace just
{
    namespace avformat
    {

        struct CodecInfo
        {
            boost::uint32_t category;
            boost::uint32_t stream_type; // type of stream according to file specific, eg: WAVE_FORMAT_WMA2 = 0x0055
            boost::uint32_t codec_type;
            boost::uint32_t codec_format;
            boost::uint32_t time_scale;
            void const * context;
        };

        class Format
        {
        public:
            Format();

            Format(
                CodecInfo const * codecs, 
                size_t ncodec);

            virtual ~Format();

        public:
            virtual CodecInfo const * codec_from_stream(
                boost::uint32_t category, 
                boost::uint32_t stream_type, 
                void const * context, 
                boost::system::error_code & ec);

            virtual CodecInfo const * codec_from_codec(
                boost::uint32_t category, 
                boost::uint32_t codec_type, 
                void const * context, 
                boost::system::error_code & ec);

            virtual CodecInfo const * codec_from_stream(
                boost::uint32_t category, 
                boost::uint32_t stream_type, 
                boost::system::error_code & ec);

            virtual CodecInfo const * codec_from_codec(
                boost::uint32_t category, 
                boost::uint32_t codec_type, 
                boost::system::error_code & ec);

        public:
            // call by demuxer, with type, sub_type of info valid, 
            // sub_type is the stream type according to file specific
            // fill type, sub_type and time_scale
            virtual bool finish_from_stream(
                just::avbase::StreamInfo & info, 
                boost::system::error_code & ec);

            // call by muxer, with type, sub_type of info valid, 
            // fill format_type according to file specific
            // fill time_scale according to file specific
            virtual bool finish_from_codec(
                just::avbase::StreamInfo & info, 
                boost::system::error_code & ec);

        public:
            static bool finish_from_stream(
                just::avbase::StreamInfo & info, 
                std::string const & format_str, 
                boost::uint32_t stream_type, 
                boost::system::error_code & ec);

        protected:
            CodecInfo const * codecs_;
            size_t ncodec_;
        };

        struct FormatTraits
            : util::tools::ClassFactoryTraits
        {
            typedef std::string key_type;
            typedef Format * (create_proto)();

            static boost::system::error_code error_not_found();
        };

        typedef util::tools::ClassFactory<FormatTraits> FormatFactory;

    } // namespace avformat
} // namespace just

#define JUST_REGISTER_FORMAT(key, cls) UTIL_REGISTER_CLASS(just::avformat::FormatFactory, key, cls)

#endif // _JUST_AVFORMAT_FORMAT_H_
