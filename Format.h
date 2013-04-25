// Format.h

#ifndef _PPBOX_AVFORMAT_FORMAT_H_
#define _PPBOX_AVFORMAT_FORMAT_H_

#include <boost/detail/endian.hpp>

namespace ppbox
{
    namespace avformat
    {

#ifdef BOOST_BIG_ENDIAN
#define MAKE_FOURC_TYPE(c1, c2, c3, c4) \
    ((((boost::uint32_t)c1) << 24) | \
    (((boost::uint32_t)c2) << 16) | \
    (((boost::uint32_t)c3) << 8) | \
    (((boost::uint32_t)c4)))
#else
#define MAKE_FOURC_TYPE(c1, c2, c3, c4) \
    ((((boost::uint32_t)c1)) | \
    (((boost::uint32_t)c2) << 8) | \
    (((boost::uint32_t)c3)) << 16 | \
    (((boost::uint32_t)c4) << 24))
#endif

        struct StreamType
        {
            enum Enum
            {
                NONE = 0, 
                VIDE = MAKE_FOURC_TYPE('V', 'I', 'D', 'E'), 
                AUDI = MAKE_FOURC_TYPE('A', 'U', 'D', 'I'), 
            };
        };

        struct VideoSubType
        {
            enum Enum
            {
                NONE = 0, 
                AVC1 = MAKE_FOURC_TYPE('A', 'V', 'C', '1'), 
                WMV3 = MAKE_FOURC_TYPE('W', 'M', 'V', '3'), 
                MP4V = MAKE_FOURC_TYPE('M', 'P', '4', 'V'), 
            };
        };

        struct AudioSubType
        {
            enum Enum
            {
                NONE = 0, 
                MP4A = MAKE_FOURC_TYPE('M', 'P', '4', 'A'), 
                MP1A = MAKE_FOURC_TYPE('M', 'P', '1', 'A'), 
                WMA2 = MAKE_FOURC_TYPE('W', 'M', 'A', '2'), 
            };
        };

        struct FormatType
        {
            enum Enum
            {
                none = 0, 
                video_avc_packet = 1, 
                video_avc_byte_stream = 2, 
                audio_raw = 1, 
                audio_adts = 2, 
            };
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_FORMAT_H_
