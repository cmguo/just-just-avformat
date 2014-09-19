// RtpFormat.h

#ifndef _PPBOX_AVFORMAT_RTP_RTP_FORMAT_H_
#define _PPBOX_AVFORMAT_RTP_RTP_FORMAT_H_

#include <ppbox/avformat/Format.h>

namespace ppbox
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

        PPBOX_REGISTER_FORMAT("rtp", RtpFormat);

    } // namespace avformat
} // namespace ppbox


#endif // _PPBOX_AVFORMAT_RTP_RTP_FORMAT_H_
