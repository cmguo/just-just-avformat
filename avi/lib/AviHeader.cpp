// AviHeader.cpp

#include "just/avformat/Common.h"
#include "just/avformat/avi/lib/AviHeader.h"
#include "just/avformat/avi/box/AviBox.hpp"
#include "just/avformat/avi/box/AviBoxVector.hpp"

namespace just
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
} // namespace just
