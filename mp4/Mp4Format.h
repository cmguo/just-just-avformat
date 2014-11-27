// Mp4Format.h

#ifndef _JUST_AVFORMAT_MP4_MP4_FORMAT_H_
#define _JUST_AVFORMAT_MP4_MP4_FORMAT_H_

#include "just/avformat/Format.h"

namespace just
{
    namespace avformat
    {

        class Mp4Format
            : public Format
        {
        public:
            Mp4Format();

        public:
            virtual CodecInfo const * codec_from_stream(
                boost::uint32_t category, 
                boost::uint32_t stream_type, 
                void const * context, 
                boost::system::error_code & ec);

        private:
            static CodecInfo const codecs_[];
        };

        JUST_REGISTER_FORMAT("mp4", Mp4Format);

    } // namespace avformat
} // namespace just


#endif // _JUST_AVFORMAT_MP4_MP4_FORMAT_H_
