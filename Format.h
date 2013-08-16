// Format.h

#ifndef _PPBOX_AVFORMAT_FORMAT_H_
#define _PPBOX_AVFORMAT_FORMAT_H_

#include <ppbox/avbase/StreamInfo.h>

#include <ppbox/common/ClassFactory.h>

namespace ppbox
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
        };

        class Format
            : public ppbox::common::ClassFactory<
                Format, 
                std::string, 
                Format *()
            >
        {
        public:
            static boost::system::error_code error_not_found();

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
                ppbox::avbase::StreamInfo & info, 
                boost::system::error_code & ec);

            // call by muxer, with type, sub_type of info valid, 
            // fill format_type according to file specific
            // fill time_scale according to file specific
            virtual bool finish_from_codec(
                ppbox::avbase::StreamInfo & info, 
                boost::system::error_code & ec);

        public:
            static bool finish_from_stream(
                ppbox::avbase::StreamInfo & info, 
                std::string const & format_str, 
                boost::uint32_t stream_type, 
                boost::system::error_code & ec);

        protected:
            CodecInfo const * codecs_;
            size_t ncodec_;
        };

    } // namespace avformat
} // namespace ppbox

#define PPBOX_REGISTER_FORMAT(key, cls) PPBOX_REGISTER_CLASS(key, cls)

#endif // _PPBOX_AVFORMAT_FORMAT_H_
