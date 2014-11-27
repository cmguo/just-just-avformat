// AviBoxData.h

#ifndef _JUST_AVFORMAT_AVI_BOX_AVI_BOX_DATA_H_
#define _JUST_AVFORMAT_AVI_BOX_AVI_BOX_DATA_H_

#include "just/avformat/avi/box/AviBoxTraits.h"
#include "just/avformat/avi/box/AviBoxHeader.h"

#include <just/avbase/object/ObjectData.h>

namespace just
{
    namespace avformat
    {

        typedef just::avbase::ObjectDefine AviBoxDefine;

        template <
            typename T, 
            AviBoxHeader::id_type id
        >
        struct AviBoxData
            : just::avbase::ObjectData<AviBoxTraits, T, id>
        {
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_AVI_BOX_AVI_BOX_DATA_H_
