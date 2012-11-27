// Codec.h

#ifndef _PPBOX_AVFORMAT_CODEC_CODEC_H_
#define _PPBOX_AVFORMAT_CODEC_CODEC_H_

#include "ppbox/avformat/Format.h"

#include <ppbox/common/ClassFactory.h>

namespace ppbox
{
    namespace avformat
    {

        class Codec
            : public ppbox::common::ClassFactory<
                Codec, 
                boost::uint32_t, 
                Codec *(std::vector<boost::uint8_t> const &)
            >
        {
        public:
            Codec()
            {
            }

            virtual ~Codec()
            {
            }
        };

    } // namespace avformat
} // namespace ppbox

#define PPBOX_REGISTER_CODEC(key, cls) PPBOX_REGISTER_CLASS(key, cls)

#endif // _PPBOX_AVFORMAT_CODEC_CODEC_H_
