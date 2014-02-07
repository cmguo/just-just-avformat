// Mp4FileBox.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_FILE_BOX_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_FILE_BOX_H_

#include "ppbox/avformat/mp4/box/Mp4BoxData.h"
#include "ppbox/avformat/mp4/box/Mp4BoxEnum.h"
#include "ppbox/avformat/mp4/box/Mp4Vector.h"

namespace ppbox
{
    namespace avformat
    {

        struct Mp4FileTypeBox
            : Mp4BoxData<Mp4FileTypeBox, Mp4BoxType::ftyp>
        {
            boost::uint32_t major_brand;
            boost::uint32_t minor_version;
            Mp4Vector<boost::uint32_t> compatible_brands;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & major_brand
                    & minor_version
                    & compatible_brands;
            }
        };

        struct Mp4MediaDataBox
            : Mp4BoxData<Mp4MediaDataBox, Mp4BoxType::mdat>
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

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_FILE_BOX_H_
