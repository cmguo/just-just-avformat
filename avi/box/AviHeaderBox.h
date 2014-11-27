// AviHeaderBox.h

#ifndef _JUST_AVFORMAT_AVI_BOX_AVI_HEADER_BOX_H_
#define _JUST_AVFORMAT_AVI_BOX_AVI_HEADER_BOX_H_

#include "just/avformat/avi/box/AviBoxData.h"
#include "just/avformat/avi/box/AviBoxEnum.h"
#include "just/avformat/avi/box/AviBoxVector.h"

namespace just
{
    namespace avformat
    {

        struct AviHeaderListBox
            : AviBoxData<AviHeaderListBox, AviBoxType::hdrl>
            , AviBoxContainer<AviHeaderListBox, AviBoxType::hdrl>
        {
        };

        struct AviHeaderBox
            : AviBoxData<AviHeaderBox, AviBoxType::avih>
        {
            boost::uint32_t dwMicroSecPerFrame;
            boost::uint32_t dwMaxBytesPerSec;
            boost::uint32_t dwPaddingGranularity;
            boost::uint32_t dwFlags;
            boost::uint32_t dwTotalFrames;
            boost::uint32_t dwInitialFrames;
            boost::uint32_t dwStreams;
            boost::uint32_t dwSuggestedBufferSize;
            boost::uint32_t dwWidth;
            boost::uint32_t dwHeight;
            boost::uint32_t dwReserved[4];

            AviHeaderBox()
                : dwMicroSecPerFrame(0)
                , dwMaxBytesPerSec(0)
                , dwPaddingGranularity(0)
                , dwFlags(0)
                , dwTotalFrames(0)
                , dwInitialFrames(0)
                , dwStreams(0)
                , dwSuggestedBufferSize(0)
                , dwWidth(0)
                , dwHeight(0)
            {
                dwReserved[0] = dwReserved[1] = dwReserved[2] = dwReserved[3] = 0;
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & dwMicroSecPerFrame;
                ar & dwMaxBytesPerSec;
                ar & dwPaddingGranularity;
                ar & dwFlags;
                ar & dwTotalFrames;
                ar & dwInitialFrames;
                ar & dwStreams;
                ar & dwSuggestedBufferSize;
                ar & dwWidth;
                ar & dwHeight;
                ar & framework::container::make_array(dwReserved);
            }
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_AVI_BOX_AVI_HEADER_BOX_H_
