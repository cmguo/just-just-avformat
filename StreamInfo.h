// Format.h

#ifndef _PPBOX_AVFORMAT_STREAM_INFO_H_
#define _PPBOX_AVFORMAT_STREAM_INFO_H_

#include "ppbox/avformat/Format.h"
#include "ppbox/avformat/codec/Codec.h"

#include <boost/intrusive_ptr.hpp>

namespace ppbox
{
    namespace avformat
    {

        struct VideoInfo
        {
            boost::uint32_t width;
            boost::uint32_t height;
            boost::uint32_t frame_rate;
        };

        struct AudioInfo
        {
            boost::uint32_t channel_count;
            boost::uint32_t sample_size;
            boost::uint32_t sample_rate;
        };

        struct StreamInfoBase
        {
            StreamInfoBase()
                : type(MEDIA_TYPE_NONE)
                , sub_type(VIDEO_TYPE_NONE)
                , time_scale(0)
                , duration(0)
                , codec(NULL)
            {
            }

            union {
                boost::uint32_t type;
                char type_char[4];
            };
            union {
                boost::uint32_t sub_type;
                char sub_type_char[4];
            };
            boost::uint32_t index;
            boost::uint32_t time_scale;
            boost::uint64_t start_time;
            boost::uint64_t duration;
            boost::intrusive_ptr<Codec> codec;
        };

        struct StreamInfo
            : StreamInfoBase
        {
            enum FormatTypeEnum
            {
                none, 
                video_avc_packet = 1, 
                video_avc_byte_stream = 2, 
                audio_microsoft_wave = 3, 
                audio_iso_mp4 = 4, 
                audio_aac_adts = 5, 
            };

            StreamInfo()
                : format_type(none)
            {
            }

            boost::uint32_t format_type;             // 格式说明的类型
            union {
                VideoInfo video_format;
                AudioInfo audio_format;
            };
            std::vector<boost::uint8_t> format_data; // 格式说明的内容
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_STREAM_INFO_H_
