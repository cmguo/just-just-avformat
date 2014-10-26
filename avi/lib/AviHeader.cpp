// AviHeader.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/avi/lib/AviHeader.h"
#include "ppbox/avformat/avi/box/AviBox.hpp"
#include "ppbox/avformat/avi/box/AviBoxVector.hpp"

namespace ppbox
{
    namespace avformat
    {

        AviHeader::AviHeader(
            AviBox & box)
            : AviBoxWrapper<AviHeaderBox>(box)
        {
        }

        void AviHeader::width(
            boost::uint32_t n)
        {
            data_->dwWidth = n;
        }

        void AviHeader::height(
            boost::uint32_t n)
        {
            data_->dwHeight = n;
        }

    } // namespace avformat
} // namespace ppbox
