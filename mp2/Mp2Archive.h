// Mp2Archive.h

#ifndef _JUST_AVFORMAT_MP2_MP2_ARCHIVE_H_
#define _JUST_AVFORMAT_MP2_MP2_ARCHIVE_H_

#include "just/avformat/FormatBuffer.h"

#include <util/archive/BigEndianBinaryIArchive.h>
#include <util/archive/BigEndianBinaryOArchive.h>

namespace just
{
    namespace avformat
    {

        typedef util::archive::BigEndianBinaryIArchive<boost::uint8_t> Mp2IArchive;
        typedef util::archive::BigEndianBinaryOArchive<boost::uint8_t> Mp2OArchive;

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MP2_MP2_ARCHIVE_H_
