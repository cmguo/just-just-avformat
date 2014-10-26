// AviBoxData.h

#ifndef _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_DATA_H_
#define _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_DATA_H_

#include "ppbox/avformat/avi/box/AviBoxTraits.h"
#include "ppbox/avformat/avi/box/AviBoxHeader.h"

#include <ppbox/avbase/object/ObjectData.h>

namespace ppbox
{
    namespace avformat
    {

        typedef ppbox::avbase::ObjectDefine AviBoxDefine;

        template <
            typename T, 
            AviBoxHeader::id_type id
        >
        struct AviBoxData
            : ppbox::avbase::ObjectData<AviBoxTraits, T, id>
        {
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_DATA_H_
