// AvcDecode.h

#ifndef _PPBOX_AVFORMAT_AVC_CODEC_
#define _PPBOX_AVFORMAT_AVC_CODEC_

#include "ppbox/avformat/codec/Codec.h"
#include "ppbox/avformat/codec/AvcConfig.h"

namespace ppbox
{
    namespace avformat
    {
        class AvcCodec
            : public Codec
        {
        public:
            AvcCodec()
            {
            }

            ~AvcCodec()
            {
            }

            virtual void config(
                std::vector<boost::uint8_t> & data,
                void *& config)
            {
                avc_config_.set_buffer(&data.at(0), data.size());
                avc_config_.creat();
                config = (void*)&avc_config_;
            }

        private:
            AvcConfig avc_config_;
        };
    }
}

#endif // _PPBOX_AVFORMAT_AVC_CODEC_
