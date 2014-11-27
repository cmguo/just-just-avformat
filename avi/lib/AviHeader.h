// AviHeader.h

#ifndef _JUST_AVFORMAT_AVI_LIB_AVI_HEADER_H_
#define _JUST_AVFORMAT_AVI_LIB_AVI_HEADER_H_

#include "just/avformat/avi/box/AviHeaderBox.h"
#include "just/avformat/avi/lib/AviBoxWrapper.h"

namespace just
{
    namespace avformat
    {

        class AviHeader
            : public AviBoxWrapper<AviHeaderBox>
        {
        public:
            AviHeader(
                AviBox & box);

        public:
            boost::uint32_t width() const
            {
                return data_->dwWidth;
            }

            boost::uint32_t height() const
            {
                return data_->dwHeight;
            }

            void width(
                boost::uint32_t n);

            void height(
                boost::uint32_t n);
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_AVI_LIB_AVI_HEADER_H_
