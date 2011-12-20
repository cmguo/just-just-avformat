// Codec.h

#ifndef   _PPBOX_AVFORMAT_CODEC_
#define   _PPBOX_AVFORMAT_CODEC_

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

            virtual void config(
                std::vector<boost::uint8_t> & data,
                void *& config) = 0;

        };
    }
}

#endif // _PPBOX_AVFORMAT_CODEC_
