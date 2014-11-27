// AviMovieBox.h

#ifndef _JUST_AVFORMAT_AVI_BOX_AVI_MOVIE_BOX_H_
#define _JUST_AVFORMAT_AVI_BOX_AVI_MOVIE_BOX_H_

#include "just/avformat/avi/box/AviBoxData.h"
#include "just/avformat/avi/box/AviBoxEnum.h"
#include "just/avformat/avi/box/AviBoxVector.h"

namespace just
{
    namespace avformat
    {

        struct AviMovieBox
            : AviBoxData<AviMovieBox, AviBoxType::movi>
            , AviBoxContainer<AviMovieBox, AviBoxType::movi>
        {
            static just::avbase::ObjectDefine::ClassEnum const static_cls = just::avbase::ObjectDefine::cls_data;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
            }
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_AVI_BOX_AVI_MOVIE_BOX_H_
