// AsfEnum.h

#ifndef _JUST_AVFORMAT_ASF_ASF_ENUM_H_
#define _JUST_AVFORMAT_ASF_ASF_ENUM_H_

#include <just/avbase/FourCC.h>

namespace just
{
    namespace avformat
    {

        struct AsfAudioCodec
        {
            enum Enum {
                MP3     = 0x0055, 
                WMA2    = 0x0161, 
                AAC     = 0x00ff, 
            };
        };

        struct AsfVideoCodec
        {
            enum Enum {
                WMV3    = MAKE_FOURC_TYPE('W', 'M', 'V', '3'),
                H264    = MAKE_FOURC_TYPE('H', '2', '6', '4'),
                h264    = MAKE_FOURC_TYPE('h', '2', '6', '4'),
            };
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_ASF_ASF_ENUM_H_
