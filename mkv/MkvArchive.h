// MkvArchive.h

#ifndef _JUST_AVFORMAT_MKV_MKV_ARCHIVE_H_
#define _JUST_AVFORMAT_MKV_MKV_ARCHIVE_H_

#include "just/avformat/mkv/ebml/EBML_Type.h"
#include "just/avformat/FormatBuffer.h"

namespace just
{
    namespace avformat
    {

        typedef EBML_OArchive MkvOArchive;

        typedef EBML_IArchive MkvIArchive;

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MKV_MKV_ARCHIVE_H_
