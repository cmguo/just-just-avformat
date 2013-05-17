// MkvFormat.h

#ifndef _PPBOX_AVFORMAT_MKV_MKV_FORMAT_H_
#define _PPBOX_AVFORMAT_MKV_MKV_FORMAT_H_

#include "ppbox/avformat/Format.h"

namespace ppbox
{
    namespace avformat
    {

        class MkvFormat
            : public Format
        {
        public:
            MkvFormat();

        public:
            virtual CodecInfo const * codec_from_format(
                boost::uint32_t category, 
                intptr_t format);

        private:
            static CodecInfo const codecs_[];
        };

        PPBOX_REGISTER_FORMAT(FormatType::MKV, MkvFormat);

    } // namespace avformat
} // namespace ppbox


#endif // _PPBOX_AVFORMAT_MKV_MKV_FORMAT_H_
