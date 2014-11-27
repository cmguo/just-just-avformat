// Mp4Box.h

#ifndef _JUST_AVFORMAT_MP4_BOX_MP4_BOX_H_
#define _JUST_AVFORMAT_MP4_BOX_MP4_BOX_H_

#include "just/avformat/mp4/box/Mp4BoxHeader.h"
#include "just/avformat/mp4/box/Mp4BoxTraits.h"

#include <just/avbase/object/Object.h>

namespace just
{
    namespace avformat
    {

        typedef just::avbase::Object<Mp4BoxTraits> Mp4Box;

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MP4_BOX_MP4_BOX_H_
