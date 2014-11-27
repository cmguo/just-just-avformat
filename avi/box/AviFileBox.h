// AviFileBox.h

#ifndef _JUST_AVFORMAT_AVI_BOX_AVI_FILE_BOX_H_
#define _JUST_AVFORMAT_AVI_BOX_AVI_FILE_BOX_H_

#include "just/avformat/avi/box/AviBoxData.h"
#include "just/avformat/avi/box/AviBoxEnum.h"
#include "just/avformat/avi/box/AviBoxVector.h"

namespace just
{
    namespace avformat
    {

        struct AviFileBox
            : AviBoxData<AviFileBox, AviBoxType::AVI>
            , AviBoxContainer<AviFileBox, AviBoxType::AVI>
        {
        };

        struct AviJunkBox
            : AviBoxData<AviJunkBox, AviBoxType::JUNK>
        {
            template <typename Archive>
            void serialize(
                Archive & ar)
            {
            }
        };

        struct AviInfoListBox
            : AviBoxData<AviInfoListBox, AviBoxType::INFO>
            , AviBoxContainer<AviInfoListBox, AviBoxType::INFO>
        {
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_AVI_BOX_AVI_FILE_BOX_H_
