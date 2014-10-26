// AviStreamBox.h

#ifndef _PPBOX_AVFORMAT_AVI_BOX_AVI_STREAM_BOX_H_
#define _PPBOX_AVFORMAT_AVI_BOX_AVI_STREAM_BOX_H_

#include "ppbox/avformat/avi/box/AviBoxData.h"
#include "ppbox/avformat/avi/box/AviBoxEnum.h"
#include "ppbox/avformat/avi/box/AviBoxVector.h"

namespace ppbox
{
    namespace avformat
    {

        struct AviStreamBox
            : AviBoxData<AviStreamBox, AviBoxType::strl>
            , AviBoxContainer<AviStreamBox, AviBoxType::strl>
        {
        };

        struct AviStreamHeaderBox
            : AviBoxData<AviStreamHeaderBox, AviBoxType::strh>
        {
            union {
                boost::uint32_t dwType;
                char fccType[4];
            };
            union {
                boost::uint32_t dwHandler;
                char fccHandler[4];
            };
            boost::uint32_t dwFlags;
            boost::uint16_t wPriority;
            boost::uint16_t wLanguage;
            boost::uint32_t dwInitialFrames;
            boost::uint32_t dwScale;
            boost::uint32_t dwRate;
            boost::uint32_t dwStart;
            boost::uint32_t dwLength;
            boost::uint32_t dwSuggestedBufferSize;
            boost::uint32_t dwQuality;
            boost::uint32_t dwSampleSize;
            struct {
              short int left;
              short int top;
              short int right;
              short int bottom;
            } rcFrame;

            AviStreamHeaderBox()
                : dwType(0)
                , dwHandler(0)
                , dwFlags(0)
                , wPriority(0)
                , wLanguage(0)
                , dwInitialFrames(0)
                , dwScale(0)
                , dwRate(0)
                , dwStart(0)
                , dwLength(0)
                , dwSuggestedBufferSize(0)
                , dwQuality(0)
                , dwSampleSize(0)
            {
                rcFrame.left = rcFrame.top = rcFrame.right = rcFrame.bottom = 0;
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & framework::container::make_array(fccType);
                ar & framework::container::make_array(fccHandler);
                ar & dwFlags;
                ar & wPriority;
                ar & wLanguage;
                ar & dwInitialFrames;
                ar & dwScale;
                ar & dwRate;
                ar & dwStart;
                ar & dwLength;
                ar & dwSuggestedBufferSize;
                ar & dwQuality;
                ar & dwSampleSize;
                ar & rcFrame.left;
                ar & rcFrame.top;
                ar & rcFrame.right;
                ar & rcFrame.bottom;
            }
        };

        struct AviStreamFormatBox
            : AviBoxData<AviStreamFormatBox, AviBoxType::strf>
        {
            
            struct VideoFormat
            {
                boost::uint32_t biSize;
                boost::uint32_t biWidth;
                boost::uint32_t biHeight;
                boost::uint16_t biPlanes;
                boost::uint16_t biBitCount;
                boost::uint32_t biCompression;
                boost::uint32_t biSizeImage;
                boost::uint32_t biXPelsPerMeter;
                boost::uint32_t biYPelsPerMeter;
                boost::uint32_t biClrUsed;
                boost::uint32_t biClrImportant;

                VideoFormat()
                    : biSize(0)
                    , biWidth(0)
                    , biHeight(0)
                    , biPlanes(0)
                    , biBitCount(0)
                    , biCompression(0)
                    , biSizeImage(0)
                    , biXPelsPerMeter(0)
                    , biYPelsPerMeter(0)
                    , biClrUsed(0)
                    , biClrImportant(0)
                {
                }

                template <typename Archive>
                void serialize(
                    Archive & ar)
                {
                    ar & biSize;
                    ar & biWidth;
                    ar & biHeight;
                    ar & biPlanes;
                    ar & biBitCount;
                    ar & biCompression;
                    ar & biSizeImage;
                    ar & biXPelsPerMeter;
                    ar & biYPelsPerMeter;
                    ar & biClrUsed;
                    ar & biClrImportant;
                }
            };

            struct AudioFormat
            {
                boost::uint16_t wFormatTag;
                boost::uint16_t nChannels;
                boost::uint32_t nSamplesPerSec;
                boost::uint32_t nAvgBytesPerSec;
                boost::uint16_t nBlockAlign;
                boost::uint16_t wBitsPerSample;
                boost::uint16_t cbSize;
                std::vector<boost::uint8_t> cbData;

                AudioFormat()
                    : wFormatTag(0)
                    , nChannels(0)
                    , nSamplesPerSec(0)
                    , nAvgBytesPerSec(0)
                    , nBlockAlign(0)
                    , wBitsPerSample(0)
                    , cbSize(0)
                {
                }

                template <typename Archive>
                void serialize(
                    Archive & ar)
                {
                    ar & wFormatTag;
                    ar & nChannels;
                    ar & nSamplesPerSec;
                    ar & nAvgBytesPerSec;
                    ar & nBlockAlign;
                    ar & wBitsPerSample;
                    ar & cbSize;
                    util::serialization::serialize_collection(ar, cbData, cbSize);
                }
            };

            VideoFormat video;
            AudioFormat audio;
            
            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                AviBoxContext * ctx = (AviBoxContext *)ar.context();
                AviStreamBox * strl = &(*(ctx->stack.end() - 2))->as<AviStreamBox>();
                AviStreamHeaderBox * strh = strl->find_item_as<AviStreamHeaderBox>(AviBoxType::strh);
                if (strh->dwType == AviStreamType::vids)
                    ar & video;
                else if (strh->dwType == AviStreamType::auds)
                    ar & audio;
            }
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_BOX_AVI_STREAM_BOX_H_
