// AviBoxEnum.h

#ifndef _JUST_AVFORMAT_AVI_BOX_AVI_BOX_ENUM_H_
#define _JUST_AVFORMAT_AVI_BOX_AVI_BOX_ENUM_H_

#include "just/avbase/FourCC.h"

namespace just
{
    namespace avformat
    {

        struct AviBoxType
        {
            enum Enum {
                AVI  = MAKE_FOURC_TYPE('A', 'V', 'I', ' '),

                hdrl = MAKE_FOURC_TYPE('h', 'd', 'r', 'l'), 
                avih = MAKE_FOURC_TYPE('a', 'v', 'i', 'h'), 
                strl = MAKE_FOURC_TYPE('s', 't', 'r', 'l'), 
                strh = MAKE_FOURC_TYPE('s', 't', 'r', 'h'), 
                strf = MAKE_FOURC_TYPE('s', 't', 'r', 'f'), 
                strn = MAKE_FOURC_TYPE('s', 't', 'r', 'n'), 

                movi = MAKE_FOURC_TYPE('m', 'o', 'v', 'i'), 
                idx1 = MAKE_FOURC_TYPE('i', 'd', 'x', '1'), 

                INFO = MAKE_FOURC_TYPE('I', 'N', 'F', 'O'), 
                JUNK = MAKE_FOURC_TYPE('J', 'U', 'N', 'K'), 
            };
        };

        struct AviHeaderFlags
        {
            enum Enum
            {
                HasIndex        = 0x00000010, 
                MustUseIndex    = 0x00000020,
                IsInterleaved   = 0x00000100, 
                WasCaptureFile  = 0x00010000,
                CopyRighted     = 0x00020000, 
            };
        };

        struct AviStreamType
        {
            enum Enum {
                vids = MAKE_FOURC_TYPE('v', 'i', 'd', 's'), 
                auds = MAKE_FOURC_TYPE('a', 'u', 'd', 's'), 
                mids = MAKE_FOURC_TYPE('m', 'i', 'd', 's'), 
                txts = MAKE_FOURC_TYPE('t', 'x', 't', 's'), 
            };
        };

        struct AviHandlerType
        {
            enum Enum {
                DIVX = MAKE_FOURC_TYPE('D', 'I', 'V', 'X'), 
            };
        };

        struct AviAudioCodec
        {
            enum Enum {
                MPEG    = 0x0050, 
                MP3     = 0x0055, 
                AAC     = 0x00ff, 
                WMA2    = 0x0161, 
                AC3     = 0x2000, 
                DTS     = 0x2001, 
            };
        };

        struct AviVideoCodec
        {
            enum Enum {
                WMV3    = MAKE_FOURC_TYPE('W', 'M', 'V', '3'),
                H264    = MAKE_FOURC_TYPE('H', '2', '6', '4'),
                h264    = MAKE_FOURC_TYPE('h', '2', '6', '4'),
                DIVX    = MAKE_FOURC_TYPE('D', 'I', 'V', 'X'),
            };
        };

        struct AviIndexFlags
        {
            enum Enum
            {
                List        = 0x00000001, 
                KeyFrame    = 0x00000010, 
                NoTime      = 0x00000100, 
            };
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_AVI_BOX_AVI_BOX_ENUM_H_
