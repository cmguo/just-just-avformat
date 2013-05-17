// FormatType.h

#ifndef _PPBOX_AVFORMAT_FORMAT_TYPE_H_
#define _PPBOX_AVFORMAT_FORMAT_TYPE_H_

#include <ppbox/avbase/StreamType.h>
#include <ppbox/avcodec/CodecType.h>

namespace ppbox
{
    namespace avformat
    {

        using ppbox::avbase::StreamType;
        using ppbox::avcodec::VideoSubType;
        using ppbox::avcodec::AudioSubType;

        struct FormatType
        {
            enum Enum
            {
                MP4 = MAKE_FOURC_TYPE('m', 'p', '4', '\0'), 
                FLV = MAKE_FOURC_TYPE('f', 'l', 'v', '\0'), 
                TS  = MAKE_FOURC_TYPE('t', 's', '\0', '\0'), 
                ASF = MAKE_FOURC_TYPE('a', 's', 'f', '\0'), 
                MKV = MAKE_FOURC_TYPE('m', 'k', 'v', '\0'), 
            };
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_FORMAT_TYPE_H_
