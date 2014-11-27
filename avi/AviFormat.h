// AviFormat.h

#ifndef _JUST_AVFORMAT_AVI_AVI_FORMAT_H_
#define _JUST_AVFORMAT_AVI_AVI_FORMAT_H_

#include "just/avformat/Format.h"

namespace just
{
    namespace avformat
    {

        class AviFormat
            : public Format
        {
        public:
            AviFormat();

        public:
            virtual bool finish_from_stream(
                just::avbase::StreamInfo & info, 
                boost::system::error_code & ec);

            virtual bool finish_from_codec(
                just::avbase::StreamInfo & info, 
                boost::system::error_code & ec);

        private:
            static CodecInfo const codecs_[];
        };

        JUST_REGISTER_FORMAT("avi", AviFormat);

    } // namespace avformat
} // namespace just


#endif // _JUST_AVFORMAT_AVI_AVI_FORMAT_H_
