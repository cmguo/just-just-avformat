// AviFormat.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/avi/AviFormat.h"
#include "ppbox/avformat/avi/box/AviBoxEnum.h"
#include "ppbox/avformat/avi/box/AviBoxArchive.h"
#include "ppbox/avformat/Error.h"

#include <ppbox/avcodec/avc/AvcFormatType.h>
#include <ppbox/avcodec/aac/AacFormatType.h>

#include <ppbox/avbase/TypeMap.h>
using namespace ppbox::avcodec;

#include <util/archive/ArchiveBuffer.h>

#include <boost/asio/buffer.hpp>

namespace ppbox
{
    namespace avformat
    {

        CodecInfo const AviFormat::codecs_[] = {
            {StreamType::VIDE,  AviVideoCodec::DIVX,    AviVideoCodec::DIVX,StreamFormatType::none,      1}, 
            {StreamType::VIDE,  AviVideoCodec::H264,    VideoSubType::AVC,  AvcFormatType::byte_stream,  1}, 
            {StreamType::VIDE,  AviVideoCodec::h264,    VideoSubType::AVC,  AvcFormatType::byte_stream,  1}, 
            {StreamType::VIDE,  AviVideoCodec::WMV3,    VideoSubType::WMV3, StreamFormatType::none,      1}, 
            {StreamType::AUDI,  AviAudioCodec::MP3,     AudioSubType::MP3,  StreamFormatType::none,      1}, 
            {StreamType::AUDI,  AviAudioCodec::MP3,     AudioSubType::MP1,  StreamFormatType::none,      1}, 
            {StreamType::AUDI,  AviAudioCodec::MP3,     AudioSubType::MP2,  StreamFormatType::none,      1}, 
            {StreamType::AUDI,  AviAudioCodec::MP3,     AudioSubType::MP1A, StreamFormatType::none,      1}, 
            {StreamType::AUDI,  AviAudioCodec::MP3,     AudioSubType::MP2A, StreamFormatType::none,      1}, 
            {StreamType::AUDI,  AviAudioCodec::AAC,     AudioSubType::AAC,  AacFormatType::raw,          1}, 
            {StreamType::AUDI,  AviAudioCodec::WMA2,    AudioSubType::WMA2, StreamFormatType::none,      1}, 
        };

        AviFormat::AviFormat()
            : Format(codecs_, sizeof(codecs_) / sizeof(CodecInfo))
        {
        }

        struct MPEGLAYER3WAVEFORMAT
        {
            boost::uint16_t wID;
            boost::uint32_t fdwFlags;
            boost::uint16_t nBlockSize;
            boost::uint16_t nFramesPerBlock;
            boost::uint16_t nCodecDelay;
        };


        bool AviFormat::finish_from_stream(
            ppbox::avbase::StreamInfo & info, 
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
                        ia >> f.wID
                            >> f.fdwFlags
                            >> f.nBlockSize
                            >> f.nFramesPerBlock
                            >> f.nCodecDelay;
                        info.audio_format.sample_per_frame = f.nBlockSize / f.nFramesPerBlock;
                        info.format_data.clear();
                    }
                    break;
                }
            }
            return Format::finish_from_stream(info, ec);
        }

    } // namespace avformat
} // namespace ppbox
