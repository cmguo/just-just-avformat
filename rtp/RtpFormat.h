// RtpFormat.h

#ifndef _JUST_AVFORMAT_RTP_RTP_FORMAT_H_
#define _JUST_AVFORMAT_RTP_RTP_FORMAT_H_

#include <just/avformat/Format.h>

namespace just
{
    namespace avformat
    {

        class RtpFormat
            : public Format
        {
        public:
            RtpFormat();

        public:
            virtual  CodecInfo const * codec_from_stream(
                boost::uint32_t category, 
                boost::uint32_t stream_type, 
                void const * context, 
                boost::system::error_code & ec);

        private:
            static CodecInfo const codecs_[];
        };

        JUST_REGISTER_FORMAT("rtp", RtpFormat);

    } // namespace avformat
} // namespace just


#endif // _JUST_AVFORMAT_RTP_RTP_FORMAT_H_
