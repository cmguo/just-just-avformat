// MkvObjectType.h

#ifndef _JUST_AVFORMAT_MKV_MKV_OBJECT_TYPE_H_
#define _JUST_AVFORMAT_MKV_MKV_OBJECT_TYPE_H_

#include "just/avformat/mkv/MkvFormat.h"
#include "just/avformat/mkv/ebml/EBML_File.h"
#include "just/avformat/mkv/MkvSegment.h"

namespace just
{
    namespace avformat
    {

        typedef EBML_OArchive MkvOArchive;

        typedef EBML_IArchive MkvIArchive;

        struct MkvFile
            : EBML_File
        {
            MkvFile()
            {
                DocType = "matroska";
                DocTypeVersion = 2;
                DocTypeReadVersion = 2;
            }
        };

    } // namespace avformat
} // namespace just


#endif // _JUST_AVFORMAT_MKV_MKV_OBJECT_TYPE_H_
