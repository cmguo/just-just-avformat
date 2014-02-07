// Mp4BoxArchive.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_ARCHIVE_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_ARCHIVE_H_

#include <util/archive/BigEndianBinaryIArchive.h>
#include <util/archive/BigEndianBinaryOArchive.h>

namespace ppbox
{
    namespace avformat
    {

        typedef util::archive::BigEndianBinaryOArchive<boost::uint8_t> Mp4BoxOArchive;

        typedef util::archive::BigEndianBinaryIArchive<boost::uint8_t> Mp4BoxIArchive;

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_ARCHIVE_H_
