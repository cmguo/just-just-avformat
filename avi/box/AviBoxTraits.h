// AviBoxTraits.h

#ifndef _JUST_AVFORMAT_AVI_BOX_AVI_BOX_TRAITS_H_
#define _JUST_AVFORMAT_AVI_BOX_AVI_BOX_TRAITS_H_

#include "just/avformat/avi/box/AviBoxArchive.h"

#include <just/avbase/object/ObjectHelper.h>

namespace just
{
    namespace avformat
    {

        struct AviBoxHeader;
        class AviBoxDataHelper;

        class AviBoxTraits
        {
        public:
            typedef AviBoxHeader header_type;

            typedef AviBoxIArchive i_archive_t;

            typedef AviBoxOArchive o_archive_t;

            static size_t const max_size = 128;

            typedef just::avbase::ObjectHelper<AviBoxTraits> helper_t;

            typedef AviBoxDataHelper data_helper_t;
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_AVI_BOX_AVI_BOX_TRAITS_H_
