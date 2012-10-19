// Codec.h

#ifndef _PPBOX_AVFORMAT_CODEC_H_
#define _PPBOX_AVFORMAT_CODEC_H_

#include <boost/asio/buffer.hpp>

namespace ppbox
{
    namespace avformat
    {

        class Codec
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

#endif // _PPBOX_AVFORMAT_CODEC_H_
