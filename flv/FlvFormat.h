// FlvFormat.h

#ifndef _PPBOX_AVFORMAT_FLV_FLV_FORMAT_H_
#define _PPBOX_AVFORMAT_FLV_FLV_FORMAT_H_

#include "ppbox/avformat/Format.h"

namespace ppbox
{
    namespace avformat
    {

        class FlvFormat
            : public Format
        {
        public:
            FlvFormat();

        private:
            static CodecInfo const codecs_[];
        };

        PPBOX_REGISTER_FORMAT("flv", FlvFormat);

    } // namespace avformat
} // namespace ppbox


#endif // _PPBOX_AVFORMAT_FLV_FLV_FORMAT_H_
