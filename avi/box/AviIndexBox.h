// AviIndexBox.h

#ifndef _PPBOX_AVFORMAT_AVI_BOX_AVI_INDEX_BOX_H_
#define _PPBOX_AVFORMAT_AVI_BOX_AVI_INDEX_BOX_H_

#include "ppbox/avformat/avi/box/AviBoxData.h"
#include "ppbox/avformat/avi/box/AviBoxEnum.h"
#include "ppbox/avformat/avi/box/AviVector.h"

namespace ppbox
{
    namespace avformat
    {

        struct AviIndexBox
            : AviBoxData<AviIndexBox, AviBoxType::idx1>
        {
            struct Entry {
                union {
                    boost::uint32_t dwChunkId;
                    char fccChunkId[4];
                };
                boost::uint32_t dwFlags;
                boost::uint32_t dwOffset;
                boost::uint32_t dwSize;

                template <typename Archive>
                void serialize(
                    Archive & ar)
                {
                    ar & framework::container::make_array(fccChunkId);
                    ar & dwFlags;
                    ar & dwOffset;
                    ar & dwSize;
                }
            };

            AviVector<Entry> table;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & table;
            }
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_BOX_AVI_INDEX_BOX_H_
