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

        static const boost::uint32_t MEDIA_TYPE_VIDE = MAKE_FOURC_TYPE('V', 'I', 'D', 'E');
        static const boost::uint32_t MEDIA_TYPE_AUDI = MAKE_FOURC_TYPE('A', 'U', 'D', 'I');
        static const boost::uint32_t MEDIA_TYPE_NONE = 0;

        static const boost::uint32_t VIDEO_TYPE_AVC1 = MAKE_FOURC_TYPE('A', 'V', 'C', '1');
        static const boost::uint32_t VIDEO_TYPE_WMV3 = MAKE_FOURC_TYPE('W', 'M', 'V', '3');
        static const boost::uint32_t VIDEO_TYPE_MP4V = MAKE_FOURC_TYPE('M', 'P', '4', 'V');
        static const boost::uint32_t VIDEO_TYPE_NONE = 0;

        static const boost::uint32_t AUDIO_TYPE_MP4A = MAKE_FOURC_TYPE('M', 'P', '4', 'A');
        static const boost::uint32_t AUDIO_TYPE_MP1A = MAKE_FOURC_TYPE('M', 'P', '1', 'A');
        static const boost::uint32_t AUDIO_TYPE_WMA2 = MAKE_FOURC_TYPE('W', 'M', 'A', '2');
        static const boost::uint32_t AUDIO_TYPE_NONE = 0;

        struct FormatType
        {
            enum Enum
            {
                none, 
                video_avc_packet = 1, 
                video_avc_byte_stream = 2, 
                audio_iso_mp4 = 1, 
                audio_aac_adts = 2, 
            };
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_FORMAT_H_
