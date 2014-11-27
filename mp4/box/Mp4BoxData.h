// Mp4BoxData.h

#ifndef _JUST_AVFORMAT_MP4_BOX_MP4_BOX_DATA_H_
#define _JUST_AVFORMAT_MP4_BOX_MP4_BOX_DATA_H_

#include "just/avformat/mp4/box/Mp4BoxTraits.h"
#include "just/avformat/mp4/box/Mp4BoxHeader.h"

#include <just/avbase/object/ObjectData.h>

namespace just
{
    namespace avformat
    {

        typedef just::avbase::ObjectDefine Mp4BoxDefine;

        template <
            typename T, 
            Mp4BoxHeader::id_type id
        >
        struct Mp4BoxData
            : just::avbase::ObjectData<Mp4BoxTraits, T, id>
        {
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MP4_BOX_MP4_BOX_DATA_H_
