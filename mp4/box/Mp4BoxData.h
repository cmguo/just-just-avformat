// Mp4BoxData.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_DATA_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_DATA_H_

#include "ppbox/avformat/mp4/box/Mp4BoxTraits.h"
#include "ppbox/avformat/mp4/box/Mp4BoxHeader.h"

#include <ppbox/avbase/object/ObjectData.h>

namespace ppbox
{
    namespace avformat
    {

        typedef ppbox::avbase::ObjectDefine Mp4BoxDefine;

        template <
            typename T, 
            Mp4BoxHeader::id_type id
        >
        struct Mp4BoxData
            : ppbox::avbase::ObjectData<Mp4BoxTraits, T, id>
        {
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_DATA_H_
