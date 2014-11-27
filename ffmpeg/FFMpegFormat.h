// FFMpegFormat.h

#ifndef _JUST_AVFORMAT_FFMPEG_FFMPEG_FORMAT_H_
#define _JUST_AVFORMAT_FFMPEG_FFMPEG_FORMAT_H_

#include "just/avformat/Format.h"

namespace just
{
    namespace avformat
    {

        class FFMpegFormat
            : public Format
        {
        public:
            FFMpegFormat();
        };

        JUST_REGISTER_FORMAT("ffmpeg", FFMpegFormat);

    } // namespace avformat
} // namespace just


#endif // _JUST_AVFORMAT_FFMPEG_FFMPEG_FORMAT_H_
