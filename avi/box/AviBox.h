// AviBox.h

#ifndef _JUST_AVFORMAT_AVI_BOX_AVI_BOX_H_
#define _JUST_AVFORMAT_AVI_BOX_AVI_BOX_H_

#include "just/avformat/avi/box/AviBoxHeader.h"
#include "just/avformat/avi/box/AviBoxTraits.h"

#include <just/avbase/object/Object.h>

namespace just
{
    namespace avformat
    {

        typedef just::avbase::Object<AviBoxTraits> AviBox;

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_AVI_BOX_AVI_BOX_H_
