// AviBoxArchive.h

#ifndef _JUST_AVFORMAT_AVI_BOX_AVI_BOX_ARCHIVE_H_
#define _JUST_AVFORMAT_AVI_BOX_AVI_BOX_ARCHIVE_H_

#include <util/archive/LittleEndianBinaryIArchive.h>
#include <util/archive/LittleEndianBinaryOArchive.h>

namespace just
{
    namespace avformat
    {

        typedef util::archive::LittleEndianBinaryOArchive<boost::uint8_t> AviBoxOArchive;

        typedef util::archive::LittleEndianBinaryIArchive<boost::uint8_t> AviBoxIArchive;

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_AVI_BOX_AVI_BOX_ARCHIVE_H_
