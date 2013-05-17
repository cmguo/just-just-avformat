// AsfFormat.h

#ifndef _PPBOX_AVFORMAT_ASF_ASF_FORMAT_H_
#define _PPBOX_AVFORMAT_ASF_ASF_FORMAT_H_

#include "ppbox/avformat/Format.h"

namespace ppbox
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

        PPBOX_REGISTER_FORMAT(FormatType::ASF, AsfFormat);

    } // namespace avformat
} // namespace ppbox


#endif // _PPBOX_AVFORMAT_ASF_ASF_FORMAT_H_
