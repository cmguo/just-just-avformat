// Format.h

#ifndef _PPBOX_AVFORMAT_FORMAT_H_
#define _PPBOX_AVFORMAT_FORMAT_H_

#include "ppbox/avformat/FormatType.h"

#include <ppbox/avbase/StreamInfo.h>

#include <ppbox/common/ClassFactory.h>

namespace ppbox
{
    namespace avformat
    {
        struct CodecInfo
        {
            boost::uint32_t category;
            intptr_t format;
            boost::uint32_t codec;
            boost::uint32_t codec_format;
        };

        class Format
            : public ppbox::common::ClassFactory<
                Format, 
                boost::uint32_t, 
                Format *()
            >
        {
        public:
            Format();

            Format(
                CodecInfo const * codecs, 
                size_t ncodec);

            virtual ~Format();

        public:
            virtual CodecInfo const * codec_from_format(
                boost::uint32_t category, 
                intptr_t format);

            virtual CodecInfo const * codec_from_codec(
                boost::uint32_t category, 
                boost::uint32_t codec);

        public:
            virtual bool finish_stream_info(
                ppbox::avbase::StreamInfo & info, 
                intptr_t format);

        public:
            static bool finish_stream_info(
                ppbox::avbase::StreamInfo & info, 
                boost::uint32_t type, 
                intptr_t format);

        protected:
            CodecInfo const * codecs_;
            size_t ncodec_;
        };

    } // namespace avformat
} // namespace ppbox

#define PPBOX_REGISTER_FORMAT(key, cls) PPBOX_REGISTER_CLASS(key, cls)

#endif // _PPBOX_AVFORMAT_FORMAT_H_
