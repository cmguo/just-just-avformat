// Mp4Format.h

#ifndef _PPBOX_AVFORMAT_MP4_MP4_FORMAT_H_
#define _PPBOX_AVFORMAT_MP4_MP4_FORMAT_H_

#include "ppbox/avformat/Format.h"

namespace ppbox
{
    namespace avformat
    {

        class Mp4Format
            : public Format
        {
        public:
            Mp4Format();

        private:
            static CodecInfo const codecs_[];
        };

        PPBOX_REGISTER_FORMAT("mp4", Mp4Format);

    } // namespace avformat
} // namespace ppbox


#endif // _PPBOX_AVFORMAT_MP4_MP4_FORMAT_H_
