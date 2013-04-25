// AvcType.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/codec/avc/AvcCodec.h"

namespace ppbox
{
    namespace avformat
    {

        AvcCodec::AvcCodec()
        {
        }

        AvcCodec::AvcCodec(
            boost::uint32_t format, 
            std::vector<boost::uint8_t> const & config)
        {
            if (format == FormatType::video_avc_packet) {
                config_helper_.from_data(config);
            } else {
                config_helper_.from_es_data(config);
            }
        }

    } // namespace avformat
} // namespace ppbox
