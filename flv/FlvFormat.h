// FlvFormat.h

#ifndef _PPBOX_AVFORMAT_FLV_FLV_FORMAT_H_
#define _PPBOX_AVFORMAT_FLV_FLV_FORMAT_H_

#include <util/archive/BigEndianBinaryIArchive.h>
#include <util/archive/BigEndianBinaryOArchive.h>

namespace ppbox
{
    namespace avformat
    {
        struct FlagType
        {
            enum Enum {
                FLV_HEADER_FLAG_HASVIDEO = 1,
                FLV_HEADER_FLAG_HASAUDIO = 4,
            };
        };

        struct TagType
        {
            enum Enum {
                FLV_TAG_TYPE_AUDIO = 0x08,
                FLV_TAG_TYPE_VIDEO = 0x09,
                FLV_TAG_TYPE_META  = 0x12,
            };
        };

        struct SoundType
        {
            enum Enum {
                FLV_MONO   = 0,
                FLV_STEREO = 1,
            };
        };

        struct SoundSizeType
        {
            enum Enum {
                FLV_SAMPLESSIZE_8BIT  = 0,
                FLV_SAMPLESSIZE_16BIT = 1,
            };
        };

        struct SoundSampleRateType
        {
            enum Enum {
                FLV_SAMPLERATE_SPECIAL = 0, 
                FLV_SAMPLERATE_11025HZ = 1,
                FLV_SAMPLERATE_22050HZ = 2,
                FLV_SAMPLERATE_44100HZ = 3,
            };
        };

        struct SoundCodec
        {
            enum Enum {
                FLV_CODECID_PCM                  = 0,
                FLV_CODECID_ADPCM                = 1,
                FLV_CODECID_MP3                  = 2,
                FLV_CODECID_PCM_LE               = 3,
                FLV_CODECID_NELLYMOSER_16KHZ_MONO = 4,
                FLV_CODECID_NELLYMOSER_8KHZ_MONO = 5,
                FLV_CODECID_NELLYMOSER           = 6,
                FLV_CODECID_AAC                  = 10,
                FLV_CODECID_SPEEX                = 11,
            };
        };

        struct VideoCodec
        {
            enum Enum {
                FLV_CODECID_H263    = 2,
                FLV_CODECID_SCREEN  = 3,
                FLV_CODECID_VP6     = 4,
                FLV_CODECID_VP6A    = 5,
                FLV_CODECID_SCREEN2 = 6,
                FLV_CODECID_H264    = 7,
            };
        };

        struct FrameType
        {
            enum Enum {
                FLV_FRAME_KEY        = 1,
                FLV_FRAME_INTER      = 2,
                FLV_FRAME_DISP_INTER = 3,
            };
        };

        struct AMFDataType
        {
            enum Enum {
                AMF_DATA_TYPE_NUMBER      = 0x00,
                AMF_DATA_TYPE_BOOL        = 0x01,
                AMF_DATA_TYPE_STRING      = 0x02,
                AMF_DATA_TYPE_OBJECT      = 0x03,
                AMF_DATA_TYPE_MOVIECLIP   = 0x04,
                AMF_DATA_TYPE_NULL        = 0x05,
                AMF_DATA_TYPE_UNDEFINED   = 0x06,
                AMF_DATA_TYPE_REFERENCE   = 0x07,
                AMF_DATA_TYPE_MIXEDARRAY  = 0x08,
                AMF_DATA_TYPE_OBJECT_END  = 0x09,
                AMF_DATA_TYPE_ARRAY       = 0x0a,
                AMF_DATA_TYPE_DATE        = 0x0b,
                AMF_DATA_TYPE_LONG_STRING = 0x0c,
                AMF_DATA_TYPE_UNSUPPORTED = 0x0d,
            };
        };

        typedef util::archive::BigEndianBinaryIArchive<boost::uint8_t> FLVArchive;
        typedef util::archive::BigEndianBinaryOArchive<char> FLVOArchive;

        struct FlvMetadata
        {
            FlvMetadata()
                : hasaudio(true)
                , hasvideo(true)
                , duration(0)
                , datarate(0)
                , livetime(0)
                , timeshift(0)
                , width(0)
                , height(0)
                , videodatarate(0)
                , framerate(0)
                , videocodecid(0)
                , audiosamplerate(0)
                , audiosamplesize(0)
                , stereo(false)
                , audiocodecid(0)
                , filesize(0)
            {
            }
            bool hasaudio;
            bool hasvideo;
            std::string author;
            std::string copyright;
            std::string description;
            boost::uint32_t duration;
            boost::uint32_t datarate;
            boost::uint64_t livetime;
            boost::uint64_t timeshift;
            boost::uint32_t width;
            boost::uint32_t height;
            boost::uint32_t videodatarate;
            boost::uint32_t framerate;
            boost::uint32_t videocodecid;
            boost::uint32_t audiosamplerate;
            boost::uint32_t audiosamplesize;
            bool stereo;
            boost::uint32_t audiocodecid;
            boost::uint32_t filesize;
            std::vector<boost::uint8_t> BinaryData;
        };
    }
}


#endif // End _PPBOX_AVFORMAT_FLV_FLV_FORMAT_H_