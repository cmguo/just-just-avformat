// AsfFormat.h

#ifndef _JUST_AVFORMAT_ASF_ASF_FORMAT_H_
#define _JUST_AVFORMAT_ASF_ASF_FORMAT_H_

#include "just/avformat/Format.h"

namespace just
{
    namespace avformat
    {

        class AsfFormat
            : public Format
        {
        public:
            AsfFormat();

        private:
            static CodecInfo const codecs_[];
        };

        JUST_REGISTER_FORMAT("asf", AsfFormat);

    } // namespace avformat
} // namespace just


#endif // _JUST_AVFORMAT_ASF_ASF_FORMAT_H_
