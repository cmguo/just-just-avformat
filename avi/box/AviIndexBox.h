// AviIndexBox.h

#ifndef _JUST_AVFORMAT_AVI_BOX_AVI_INDEX_BOX_H_
#define _JUST_AVFORMAT_AVI_BOX_AVI_INDEX_BOX_H_

#include "just/avformat/avi/box/AviBoxData.h"
#include "just/avformat/avi/box/AviBoxEnum.h"
#include "just/avformat/avi/box/AviVector.h"

namespace just
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
} // namespace just

#endif // _JUST_AVFORMAT_AVI_BOX_AVI_INDEX_BOX_H_
