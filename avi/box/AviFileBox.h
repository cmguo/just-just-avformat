// AviFileBox.h

#ifndef _PPBOX_AVFORMAT_AVI_BOX_AVI_FILE_BOX_H_
#define _PPBOX_AVFORMAT_AVI_BOX_AVI_FILE_BOX_H_

#include "ppbox/avformat/avi/box/AviBoxData.h"
#include "ppbox/avformat/avi/box/AviBoxEnum.h"
#include "ppbox/avformat/avi/box/AviBoxVector.h"

namespace ppbox
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
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_BOX_AVI_FILE_BOX_H_
