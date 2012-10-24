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
            std::vector<boost::uint8_t> & config)
            : config_(config)
        {
        }

    } // namespace avformat
} // namespace ppbox
