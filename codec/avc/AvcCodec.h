// AvcCodec.h

#ifndef _PPBOX_AVFORMAT_AVC_AVC_CODEC_H_
#define _PPBOX_AVFORMAT_AVC_AVC_CODEC_H_

#include "ppbox/avformat/codec/Codec.h"
#include "ppbox/avformat/codec/avc/AvcConfig.h"

namespace ppbox
{
    namespace avformat
    {
        class AvcCodec
            : public Codec
        {
        public:
            AvcCodec();

            AvcCodec(
                std::vector<boost::uint8_t> & config);

        public:
            AvcConfig const & config() const
            {
                return config_;
            }

        private:
            AvcConfig config_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVC_AVC_CODEC_H_
