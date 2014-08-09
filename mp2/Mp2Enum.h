// TsEnum.h

#ifndef _PPBOX_AVFORMAT_MP2_MP2_ENUM_H_
#define _PPBOX_AVFORMAT_MP2_MP2_ENUM_H_

namespace ppbox
{
    namespace avformat
    {

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

        struct Mp2StreamId
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
                emm_stream          = 0xf1, 
                dsmcc_stream        = 0xf2, 
                iec_13552_stream    = 0xf3, 
                itu_t_rec_h_222_1_A = 0xf4, 
                itu_t_rec_h_222_1_B = 0xf5, 
                itu_t_rec_h_222_1_C = 0xf6, 
                itu_t_rec_h_222_1_D = 0xf7, 
                itu_t_rec_h_222_1_E = 0xf8, 
                ancillary_stream    = 0xf9, 
                iso_14496_1_sl_packetized_stream = 0xfa, 
                iso_14496_1_flex_mux_stream = 0xfb, 
                reserved_data_stream_base = 0xfc, 
                program_stream_directory = 0xff, 
            };
        };

        struct Mp2StreamType
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

        struct Mp2
        {
            static boost::uint32_t const TIME_SCALE = 90000;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP2_MP2_ENUM_H_
