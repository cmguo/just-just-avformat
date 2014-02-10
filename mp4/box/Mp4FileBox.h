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

        struct Mp4FourCCVector
            : std::vector<boost::uint32_t>
        {
            SERIALIZATION_SPLIT_MEMBER();

            template <typename Archive>
            void load(
                Archive & ar)
            {
                Mp4BoxContext * ctx = (Mp4BoxContext *)ar.context();
                std::streampos data_end = ctx->data_ends.back();
                size_t n = data_end - ar.tellg();
                resize(n / sizeof(boost::uint32_t));
                if (!empty())
                    ar >> make_array((char *)&at(0), size() * 4);
            }

            template <typename Archive>
            void save(
                Archive & ar) const
            {
                if (!empty())
                    ar << make_array((char const *)&at(0), size() * 4);
            }
        };

        struct Mp4FileTypeBox
            : Mp4BoxData<Mp4FileTypeBox, Mp4BoxType::ftyp>
        {
            union {
                boost::uint32_t major_brand;
                char major_brand_str[4];
            };
            boost::uint32_t minor_version;
            Mp4FourCCVector compatible_brands;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & make_array(major_brand_str)
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
