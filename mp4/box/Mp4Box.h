// Mp4Box.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_H_

#include "ppbox/avformat/mp4/box/Mp4BoxHeader.h"
#include "ppbox/avformat/mp4/box/Mp4BoxTraits.h"

#include <ppbox/avbase/object/Object.h>

namespace ppbox
{
    namespace avformat
    {

        typedef ppbox::avbase::Object<Mp4BoxTraits> Mp4Box;

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_H_
