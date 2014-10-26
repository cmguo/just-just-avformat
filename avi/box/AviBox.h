// AviBox.h

#ifndef _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_H_
#define _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_H_

#include "ppbox/avformat/avi/box/AviBoxHeader.h"
#include "ppbox/avformat/avi/box/AviBoxTraits.h"

#include <ppbox/avbase/object/Object.h>

namespace ppbox
{
    namespace avformat
    {

        typedef ppbox::avbase::Object<AviBoxTraits> AviBox;

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_H_
