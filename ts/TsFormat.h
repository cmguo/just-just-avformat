// TsFormat.h

#ifndef _PPBOX_AVFORMAT_TS_TS_FORMAT_H_
#define _PPBOX_AVFORMAT_TS_TS_FORMAT_H_

#include "ppbox/avformat/Format.h"

namespace ppbox
{
    namespace avformat
    {

        class TsFormat
            : public Format
        {
        public:
            TsFormat();

        public:
            virtual  CodecInfo const * codec_from_format(
                boost::uint32_t category, 
                intptr_t format);

        private:
            static CodecInfo const codecs_[];
        };

        PPBOX_REGISTER_FORMAT(FormatType::TS, TsFormat);

    } // namespace avformat
} // namespace ppbox


#endif // _PPBOX_AVFORMAT_TS_TS_FORMAT_H_
