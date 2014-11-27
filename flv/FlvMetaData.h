// FlvMetadata.h

#ifndef _JUST_AVFORMAT_FLV_FLV_META_DATA_H_
#define _JUST_AVFORMAT_FLV_FLV_META_DATA_H_

#include "just/avformat/flv/FlvDataType.h"
#include "just/avformat/SeekPoint.h"

namespace just
{
    namespace avformat
    {

        struct FlvMetaData
        {
            bool hasaudio;
            bool hasvideo;
            std::string author;
            std::string copyright;
            std::string description;
            boost::uint32_t filesize;
            boost::uint32_t datarate;
            boost::uint32_t datasize;
            boost::uint64_t duration;
            boost::uint64_t lasttimestamp;
            // video
            boost::uint64_t videosize;
            boost::uint32_t videodatarate;
            boost::uint32_t width;
            boost::uint32_t height;
            boost::uint32_t framerate;
            boost::uint32_t videocodecid;
            bool canSeekToEnd;
            boost::uint64_t lastkeyframetimestamp;
            // audio
            boost::uint64_t audiosize;
            boost::uint64_t audiodatarate;
            boost::uint32_t audiosamplerate;
            boost::uint32_t audiosamplesize;
            bool stereo;
            boost::uint32_t audiocodecid;
            // seek
            std::vector<SeekPoint> keyframes;
            std::vector<boost::uint8_t> BinaryData;

            FlvMetaData();

            void from_data(
                FlvDataValue const & data);

            void to_data(
                FlvDataValue & data);
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_FLV_FLV_META_DATA_H_
