// AacCodec.h

#ifndef _PPBOX_AVFORMAT_AAC_AAC_CODEC_H_
#define _PPBOX_AVFORMAT_AAC_AAC_CODEC_H_

#include "ppbox/avformat/codec/Codec.h"
#include "ppbox/avformat/codec/aac/AacConfigHelper.h"
#include "ppbox/avformat/Format.h"

namespace ppbox
{
    namespace avformat
    {

        class AacCodec
            : public Codec
        {
        public:
            AacCodec();

            AacCodec(
                boost::uint32_t format, 
                std::vector<boost::uint8_t> const & config);

        public:
            AacConfigHelper const & config_helper() const
            {
                return config_helper_;
            }

            AacConfig const & config() const
            {
                return config_helper_.data();
            }

        private:
            AacConfigHelper config_helper_;
        };

        PPBOX_REGISTER_CODEC(AudioSubType::MP4A, AacCodec);

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AAC_AAC_CODEC_H_
