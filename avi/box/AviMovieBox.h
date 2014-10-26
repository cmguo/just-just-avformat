// AviMovieBox.h

#ifndef _PPBOX_AVFORMAT_AVI_BOX_AVI_MOVIE_BOX_H_
#define _PPBOX_AVFORMAT_AVI_BOX_AVI_MOVIE_BOX_H_

#include "ppbox/avformat/avi/box/AviBoxData.h"
#include "ppbox/avformat/avi/box/AviBoxEnum.h"
#include "ppbox/avformat/avi/box/AviBoxVector.h"

namespace ppbox
{
    namespace avformat
    {

        struct AviMovieBox
            : AviBoxData<AviMovieBox, AviBoxType::movi>
        {
            static ppbox::avbase::ObjectDefine::ClassEnum const static_cls = ppbox::avbase::ObjectDefine::cls_data;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
            }
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_BOX_AVI_MOVIE_BOX_H_
