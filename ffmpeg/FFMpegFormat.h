// FFMpegFormat.h

#ifndef _PPBOX_AVFORMAT_FFMPEG_FFMPEG_FORMAT_H_
#define _PPBOX_AVFORMAT_FFMPEG_FFMPEG_FORMAT_H_

#include "ppbox/avformat/Format.h"

namespace ppbox
{
    namespace avformat
    {

        class FFMpegFormat
            : public Format
        {
        public:
            FFMpegFormat();

        private:
            static CodecInfo const codecs_[];
        };

        PPBOX_REGISTER_FORMAT("ffmpeg", FFMpegFormat);

    } // namespace avformat
} // namespace ppbox


#endif // _PPBOX_AVFORMAT_FFMPEG_FFMPEG_FORMAT_H_
