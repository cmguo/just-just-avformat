// FFMpegFormat.cpp

#include "just/avformat/Common.h"
#include "just/avformat/ffmpeg/FFMpegFormat.h"

#include <just/avcodec/ffmpeg/FFMpegCodecMap.h>
using namespace just::avcodec;

extern "C" {
#ifndef UINT64_C
#  define UINT64_C(c)   c ## ULL
#endif
#include <libavformat/avformat.h>
}

namespace just
{
    namespace avformat
    {

        static CodecInfo const * make_ffmpeg_codecs()
        {
            FFMpegCodec const * table = FFMpegCodecMap::table();
            CodecInfo * codecs = new CodecInfo[FFMpegCodecMap::count()];
            for (size_t i = 0; i < FFMpegCodecMap::count(); ++i) {
                codecs[i].category = table[i].category;
                codecs[i].stream_type = table[i].ffmpeg_type;
                codecs[i].codec_type = table[i].type;
                codecs[i].codec_format = 0;
                codecs[i].time_scale = 0;
            }
            return codecs;
        }

        static CodecInfo const * ffmpeg_codecs()
        {
            static std::auto_ptr<CodecInfo const> codecs(make_ffmpeg_codecs()); // better use auto_array_ptr
            return codecs.get();
        }

        FFMpegFormat::FFMpegFormat()
            : Format(ffmpeg_codecs(), FFMpegCodecMap::count())
        {

        }

    } // namespace avformat
} // namespace just
