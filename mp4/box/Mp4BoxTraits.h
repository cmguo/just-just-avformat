// Mp4BoxTraits.h

#ifndef _JUST_AVFORMAT_MP4_BOX_MP4_BOX_TRAITS_H_
#define _JUST_AVFORMAT_MP4_BOX_MP4_BOX_TRAITS_H_

#include "just/avformat/mp4/box/Mp4BoxArchive.h"

#include <just/avbase/object/ObjectHelper.h>

namespace just
{
    namespace avformat
    {

        struct Mp4BoxHeader;
        class Mp4BoxDataHelper;

        class Mp4BoxTraits
        {
        public:
            typedef Mp4BoxHeader header_type;

            typedef Mp4BoxIArchive i_archive_t;

            typedef Mp4BoxOArchive o_archive_t;

            static size_t const max_size = 128;

            typedef just::avbase::ObjectHelper<Mp4BoxTraits> helper_t;

            typedef Mp4BoxDataHelper data_helper_t;
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MP4_BOX_MP4_BOX_TRAITS_H_
