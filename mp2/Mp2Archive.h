// Mp2Archive.h

#ifndef _PPBOX_AVFORMAT_MP2_MP2_ARCHIVE_H_
#define _PPBOX_AVFORMAT_MP2_MP2_ARCHIVE_H_

#include "ppbox/avformat/FormatBuffer.h"

#include <util/archive/BigEndianBinaryIArchive.h>
#include <util/archive/BigEndianBinaryOArchive.h>

namespace ppbox
{
    namespace avformat
    {

        typedef util::archive::BigEndianBinaryIArchive<boost::uint8_t> Mp2IArchive;
        typedef util::archive::BigEndianBinaryOArchive<boost::uint8_t> Mp2OArchive;

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP2_MP2_ARCHIVE_H_
