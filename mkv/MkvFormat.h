// MkvFormat.h

#ifndef _JUST_AVFORMAT_MKV_MKV_FORMAT_H_
#define _JUST_AVFORMAT_MKV_MKV_FORMAT_H_

#include "just/avformat/Format.h"

namespace just
{
    namespace avformat
    {

        class MkvFormat
            : public Format
        {
        public:
            MkvFormat();

        public:
            virtual  CodecInfo const * codec_from_stream(
                boost::uint32_t category, 
                boost::uint32_t stream_type, 
                void const * context, 
                boost::system::error_code & ec);

        private:
            static CodecInfo const codecs_[];
            static char const * const type_strs[];
        };

        JUST_REGISTER_FORMAT("mkv", MkvFormat);

    } // namespace avformat
} // namespace just


#endif // _JUST_AVFORMAT_MKV_MKV_FORMAT_H_
