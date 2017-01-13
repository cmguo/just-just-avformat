// Mp4BoxEnum.h

#ifndef _JUST_AVFORMAT_MP4_BOX_MP4_BOX_ENUM_H_
#define _JUST_AVFORMAT_MP4_BOX_MP4_BOX_ENUM_H_

#include "just/avbase/FourCC.h"

namespace just
{
    namespace avformat
    {

        struct Mp4BoxType
        {
            enum Enum {
                ftyp = MAKE_FOURC_TYPE('f', 't', 'y', 'p'), 
                moov = MAKE_FOURC_TYPE('m', 'o', 'o', 'v'), 
                mdat = MAKE_FOURC_TYPE('m', 'd', 'a', 't'), 
                mvhd = MAKE_FOURC_TYPE('m', 'v', 'h', 'd'), 
                trak = MAKE_FOURC_TYPE('t', 'r', 'a', 'k'), 
                tkhd = MAKE_FOURC_TYPE('t', 'k', 'h', 'd'), 
                mdia = MAKE_FOURC_TYPE('m', 'd', 'i', 'a'), 
                mdhd = MAKE_FOURC_TYPE('m', 'd', 'h', 'd'), 
                hdlr = MAKE_FOURC_TYPE('h', 'd', 'l', 'r'), 
                minf = MAKE_FOURC_TYPE('m', 'i', 'n', 'f'), 
                vmhd = MAKE_FOURC_TYPE('v', 'm', 'h', 'd'), 
                smhd = MAKE_FOURC_TYPE('s', 'm', 'h', 'd'), 
                stbl = MAKE_FOURC_TYPE('s', 't', 'b', 'l'), 
                esds = MAKE_FOURC_TYPE('e', 's', 'd', 's'), 
                pasp = MAKE_FOURC_TYPE('p', 'a', 's', 'p'), 
                clap = MAKE_FOURC_TYPE('c', 'l', 'a', 'p'), 
                stsd = MAKE_FOURC_TYPE('s', 't', 's', 'd'), 
                stts = MAKE_FOURC_TYPE('s', 't', 't', 's'), 
                stss = MAKE_FOURC_TYPE('s', 't', 's', 's'), 
                ctts = MAKE_FOURC_TYPE('c', 't', 't', 's'), 
                stsc = MAKE_FOURC_TYPE('s', 't', 's', 'c'), 
                stsz = MAKE_FOURC_TYPE('s', 't', 's', 'z'), 
                stco = MAKE_FOURC_TYPE('s', 't', 'c', 'o'), 
                co64 = MAKE_FOURC_TYPE('c', 'o', '6', '4'), 
                free = MAKE_FOURC_TYPE('f', 'r', 'e', 'e'), 
                udat = MAKE_FOURC_TYPE('u', 'd', 'a', 't'), 
            };
        };

        struct Mp4HandlerType
        {
            enum Enum {
                soun = MAKE_FOURC_TYPE('s', 'o', 'u', 'n'), 
                vide = MAKE_FOURC_TYPE('v', 'i', 'd', 'e'), 
                hint = MAKE_FOURC_TYPE('h', 'i', 'n', 't'), 
                meta = MAKE_FOURC_TYPE('m', 'e', 't', 'a'), 
            };
        };

        struct Mp4CodecType
        {
            enum Enum {
                hvc1 = MAKE_FOURC_TYPE('h', 'v', 'c', '1'), 
                hev1 = MAKE_FOURC_TYPE('h', 'e', 'v', '1'), 
                avc1 = MAKE_FOURC_TYPE('a', 'v', 'c', '1'), 
                avc2 = MAKE_FOURC_TYPE('a', 'v', 'c', '2'), 
                avc3 = MAKE_FOURC_TYPE('a', 'v', 'c', '3'), 
                avc4 = MAKE_FOURC_TYPE('a', 'v', 'c', '4'), 
                mp4v = MAKE_FOURC_TYPE('m', 'p', '4', 'v'), 
                mp4a = MAKE_FOURC_TYPE('m', 'p', '4', 'a'), 
                ac_3 = MAKE_FOURC_TYPE('a', 'c', '-', '3'), 
                ec_3 = MAKE_FOURC_TYPE('e', 'c', '-', '3'),
                encv = MAKE_FOURC_TYPE('e', 'n', 'c', 'v'), //pptv drm
                enca = MAKE_FOURC_TYPE('e', 'n', 'c', 'a'),
            };
        };

        struct Mp4ObjectType
        {
            enum {
                FORBIDDEN            = 0x00, 
                MPEG4_SYSTEM         = 0x01, 
                MPEG4_SYSTEM_COR     = 0x02, 
                MPEG4_TEXT           = 0x08, 
                MPEG4_VISUAL         = 0x20, 
                MPEG4_AUDIO          = 0x40, 
                MPEG2_VISUAL_SIMPLE  = 0x60, 
                MPEG2_VISUAL_MAIN    = 0x61, 
                MPEG2_VISUAL_SNR     = 0x62, 
                MPEG2_VISUAL_SPATIAL = 0x63, 
                MPEG2_VISUAL_HIGH    = 0x64, 
                MPEG2_VISUAL_422     = 0x65, 
                MPEG2_AAC_AUDIO_MAIN = 0x66, 
                MPEG2_AAC_AUDIO_LC   = 0x67, 
                MPEG2_AAC_AUDIO_SSRP = 0x68, 
                MPEG2_PART3_AUDIO    = 0x69, 
                MPEG1_VISUAL         = 0x6A, 
                MPEG1_AUDIO          = 0x6B, 
                JPEG                 = 0x6C, 
            };
        };

        struct Mp4StreamType
        {
            enum {
                FORBIDDEN           = 0x00, 
                OBJECT_DESCRIPTOR   = 0x01, 
                CLOCK_REFERENCE     = 0x02, 
                SCENE_DESCRIPTION   = 0x03, 
                VISUAL              = 0x04, 
                AUDIO               = 0x05, 
                MPEG7               = 0x06, 
                IPMP                = 0x07, 
                OBJECT_CONTENT_INFO = 0x08, 
                MPEGJ               = 0x09, 
            };
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MP4_BOX_MP4_BOX_ENUM_H_
