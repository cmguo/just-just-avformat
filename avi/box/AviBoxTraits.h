// AviBoxTraits.h

#ifndef _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_TRAITS_H_
#define _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_TRAITS_H_

#include "ppbox/avformat/avi/box/AviBoxArchive.h"

#include <ppbox/avbase/object/ObjectHelper.h>

namespace ppbox
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

            typedef ppbox::avbase::ObjectHelper<AviBoxTraits> helper_t;

            typedef AviBoxDataHelper data_helper_t;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_TRAITS_H_
