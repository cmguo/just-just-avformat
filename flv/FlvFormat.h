// FlvFormat.h

#ifndef _JUST_AVFORMAT_FLV_FLV_FORMAT_H_
#define _JUST_AVFORMAT_FLV_FLV_FORMAT_H_

#include "just/avformat/Format.h"

namespace just
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

        JUST_REGISTER_FORMAT("flv", FlvFormat);

    } // namespace avformat
} // namespace just


#endif // _JUST_AVFORMAT_FLV_FLV_FORMAT_H_
