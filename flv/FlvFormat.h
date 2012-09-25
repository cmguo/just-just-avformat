// FlvFormat.h

#ifndef _PPBOX_AVFORMAT_FLV_FLV_FORMAT_H_
#define _PPBOX_AVFORMAT_FLV_FLV_FORMAT_H_

#include <util/archive/BigEndianBinaryIArchive.h>
#include <util/archive/BigEndianBinaryOArchive.h>

namespace ppbox
{
    namespace avformat
    {

        struct FlvFlagType
        {
            enum Enum {
                HASVIDEO = 1,
                HASAUDIO = 4,
            };
        };

        struct FlvTagType
        {
            enum Enum {
                AUDIO = 0x08,
                VIDEO = 0x09,
                META  = 0x12,
            };
        };

        struct FlvSoundType
        {
            enum Enum {
                FLV_MONO   = 0,
                FLV_STEREO = 1,
            };
        };

        struct SoundSizeType
        {
            enum Enum {
                _8BIT  = 0,
                _16BIT = 1,
            };
        };

        struct FlvSoundSampleRateType
        {
            enum Enum {
                _SPECIAL = 0, 
                _11025HZ = 1,
                _22050HZ = 2,
                _44100HZ = 3,
            };
        };

        struct FlvSoundCodec
        {
            enum Enum {
                PCM                  = 0,
                ADPCM                = 1,
                MP3                  = 2,
                PCM_LE               = 3,
                NELLYMOSER_16KHZ_MONO = 4,
                NELLYMOSER_8KHZ_MONO = 5,
                NELLYMOSER           = 6,
                AAC                  = 10,
                SPEEX                = 11,
            };
        };

        struct FlvVideoCodec
        {
            enum Enum {
                H263    = 2,
                SCREEN  = 3,
                VP6     = 4,
                VP6A    = 5,
                SCREEN2 = 6,
                H264    = 7,
            };
        };

        struct FlvFrameType
        {
            enum Enum {
                KEY        = 1,
                INTER      = 2,
                DISP_INTER = 3,
            };
        };

        struct AMFDataType
        {
            enum Enum {
                NUMBER      = 0x00,
                BOOL        = 0x01,
                STRING      = 0x02,
                OBJECT      = 0x03,
                MOVIECLIP   = 0x04,
                _NULL       = 0x05,
                UNDEFINED   = 0x06,
                REFERENCE   = 0x07,
                MIXEDARRAY  = 0x08,
                OBJECT_END  = 0x09,
                ARRAY       = 0x0a,
                DATE        = 0x0b,
                LONG_STRING = 0x0c,
                UNSUPPORTED = 0x0d,
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
