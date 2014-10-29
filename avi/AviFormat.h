// AviFormat.h

#ifndef _PPBOX_AVFORMAT_AVI_AVI_FORMAT_H_
#define _PPBOX_AVFORMAT_AVI_AVI_FORMAT_H_

#include "ppbox/avformat/Format.h"

namespace ppbox
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
                ppbox::avbase::StreamInfo & info, 
                boost::system::error_code & ec);

            virtual bool finish_from_codec(
                ppbox::avbase::StreamInfo & info, 
                boost::system::error_code & ec);

        private:
            static CodecInfo const codecs_[];
        };

        PPBOX_REGISTER_FORMAT("avi", AviFormat);

    } // namespace avformat
} // namespace ppbox


#endif // _PPBOX_AVFORMAT_AVI_AVI_FORMAT_H_
