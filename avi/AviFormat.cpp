// AviFormat.cpp

#include "just/avformat/Common.h"
#include "just/avformat/avi/AviFormat.h"
#include "just/avformat/avi/box/AviBoxEnum.h"
#include "just/avformat/avi/box/AviBoxArchive.h"
#include "just/avformat/Error.h"

#include <just/avcodec/CodecType.h>
using namespace just::avcodec;

#include <just/avbase/TypeMap.h>

#include <util/archive/ArchiveBuffer.h>

#include <boost/asio/buffer.hpp>

namespace just
{
    namespace avformat
    {

        CodecInfo const AviFormat::codecs_[] = {
            {StreamType::VIDE,  AviVideoCodec::DIVX,    AviVideoCodec::DIVX,StreamFormatType::none,      1}, 
            {StreamType::VIDE,  AviVideoCodec::H264,    VideoType::AVC,  AvcFormatType::byte_stream,  1}, 
            {StreamType::VIDE,  AviVideoCodec::h264,    VideoType::AVC,  AvcFormatType::byte_stream,  1}, 
            {StreamType::VIDE,  AviVideoCodec::WMV3,    VideoType::WMV3, StreamFormatType::none,      1}, 
            {StreamType::AUDI,  AviAudioCodec::MP3,     AudioType::MP3,  StreamFormatType::none,      1}, 
            {StreamType::AUDI,  AviAudioCodec::MPEG,    AudioType::MP1,  StreamFormatType::none,      1}, 
            {StreamType::AUDI,  AviAudioCodec::MPEG,    AudioType::MP2,  StreamFormatType::none,      1}, 
            {StreamType::AUDI,  AviAudioCodec::MPEG,    AudioType::MP1A, StreamFormatType::none,      1}, 
            {StreamType::AUDI,  AviAudioCodec::MPEG,    AudioType::MP2A, StreamFormatType::none,      1}, 
            {StreamType::AUDI,  AviAudioCodec::AAC,     AudioType::AAC,  AacFormatType::adts,         1}, 
            {StreamType::AUDI,  AviAudioCodec::WMA2,    AudioType::WMA2, StreamFormatType::none,      1}, 
            {StreamType::AUDI,  AviAudioCodec::AC3,     AudioType::EAC3, StreamFormatType::none,      1}, 
            {StreamType::AUDI,  AviAudioCodec::DTS,     AudioType::DTS,  StreamFormatType::none,      1}, 
        };

        AviFormat::AviFormat()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

        struct MPEG1WAVEFORMAT
        {
            boost::uint16_t fwHeadLayer;
            boost::uint32_t dwHeadBitrate;
            boost::uint16_t fwHeadMode;
            boost::uint16_t fwHeadModeExt;
            boost::uint16_t wHeadEmphasis;
            boost::uint16_t fwHeadFlags;
            boost::uint32_t dwPTSLow;
            boost::uint32_t dwPTSHigh;
            
            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & fwHeadLayer
                    & dwHeadBitrate
                    & fwHeadMode
                    & fwHeadModeExt
                    & wHeadEmphasis
                    & fwHeadFlags
                    & dwPTSLow
                    & dwPTSHigh;
            }
        };

        struct MPEGLAYER3WAVEFORMAT
        {
            boost::uint16_t wID;
            boost::uint32_t fdwFlags;
            boost::uint16_t nBlockSize;
            boost::uint16_t nFramesPerBlock;
            boost::uint16_t nCodecDelay;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & wID
                    & fdwFlags
                    & nBlockSize
                    & nFramesPerBlock
                    & nCodecDelay;
            }
        };

        bool AviFormat::finish_from_stream(
            just::avbase::StreamInfo & info, 
            boost::system::error_code & ec)
        {
            if (info.type == StreamType::AUDI) {
                switch (info.sub_type) {
                    case AviAudioCodec::MP3:
                    {
                        MPEGLAYER3WAVEFORMAT f;
                        util::archive::ArchiveBuffer<boost::uint8_t> abuf(
                            boost::asio::buffer(info.format_data));
                        abuf.commit(info.format_data.size());
                        AviBoxIArchive ia(abuf);
                        ia >> f;
                        info.audio_format.sample_per_frame = f.nBlockSize / f.nFramesPerBlock;
                        info.format_data.clear();
                    }
                    break;
                }
            }
            return Format::finish_from_stream(info, ec);
        }

        bool AviFormat::finish_from_codec(
            just::avbase::StreamInfo & info, 
            boost::system::error_code & ec)
        {
            if (!Format::finish_from_codec(info, ec))
                return false;
            if (info.type == StreamType::AUDI) {
                switch (info.sub_type) {
                    case AviAudioCodec::MP3:
                    {
                        MPEGLAYER3WAVEFORMAT f = {
                            1, // MPEGLAYER3_ID_MPEG.
                            0, 
                            (boost::uint16_t)info.audio_format.sample_per_frame, 
                            1,
                            0};
                        info.format_data.resize(12);
                        util::archive::ArchiveBuffer<boost::uint8_t> abuf(
                            boost::asio::buffer(info.format_data));
                        AviBoxOArchive oa(abuf);
                        oa << f;
                    }
                    break;
                    case AviAudioCodec::MPEG:
                    {
                        MPEG1WAVEFORMAT f = {
                            2, // ACM_MPEG_LAYER2.
                            0, 
                            1, // ACM_MPEG_STEREO
                            0, // fwHeadModeExt
                            0, // wHeadEmphasis
                            0, // fwHeadFlags
                            0, 
                            0}; 
                        info.format_data.resize(22);
                        util::archive::ArchiveBuffer<boost::uint8_t> abuf(
                            boost::asio::buffer(info.format_data));
                        AviBoxOArchive oa(abuf);
                        oa << f;
                    }
                    break;
                    case AviAudioCodec::AAC:
                    break;
                }
            }
            if (info.format_data.size() & 1)
                info.format_data.push_back(0);
            return true;
        }

    } // namespace avformat
} // namespace just
