// TsFormat.h

#ifndef _PPBOX_AVFORMAT_TS_TS_FORMAT_H_
#define _PPBOX_AVFORMAT_TS_TS_FORMAT_H_

#include <util/archive/BigEndianBinaryIArchive.h>
#include <util/archive/BigEndianBinaryOArchive.h>

namespace ppbox
{
    namespace avformat
    {

        typedef util::archive::BigEndianBinaryIArchive<boost::uint8_t> TsIArchive;
        typedef util::archive::BigEndianBinaryOArchive<boost::uint8_t> TsOArchive;

        static boost::uint16_t const default_program_number = 1;

        struct TsPid
        {
            enum Enum
            {
                pat         = 0x00, 
                pmt         = 0x64, 
                stream_base = 0x65, 
            };
        };

        struct PsiTableId
        {
            enum Enum
            {
                program_association, 
                conditional_access, 
                program_map, 
                discription, 
                iso_14496_scene_discription, 
                iso_14496_object_discription, 

                forbidden = 0xff
            };
        };

        struct TsStreamId
        {
            enum Enum
            {
                program_stream_map  = 0xbc, 
                private_stream_1    = 0xbd, 
                padding_stream      = 0xbe, 
                private_stream_2    = 0xbf, 
                audio_base          = 0xc0, 
                video_base          = 0xe0, 
                ecm_stream          = 0xf0, 
                emm_stream          = 0xf1
            };
        };

        struct TsStreamType
        {
            enum Enum
            {
                iso_reserved        = 0x00, 
                iso_11172_video     = 0x01, // MPEG-1 video
                iso_13818_2_video   = 0x02, // MPEG-2 video
                iso_11172_audio     = 0x03, // MPEG-1 audio layer 1,2,3
                iso_13818_3_audio   = 0x04, // MPEG-2 audio layer 1,2,3

                iso_13818_7_audio   = 0x0f, // AAC Audio with ADTS
                iso_13818_video   = 0x1b, // AVC
            };
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_TS_TS_FORMAT_H_