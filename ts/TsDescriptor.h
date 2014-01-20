// TsDescriptor.h

#ifndef _PPBOX_AVFORMAT_TS_TS_DESCRIPTOR_H_
#define _PPBOX_AVFORMAT_TS_TS_DESCRIPTOR_H_

namespace ppbox
{
    namespace avformat
    {

        struct TsDescriptor
        {
            boost::uint8_t descriptor_tag;
            boost::uint8_t descriptor_length;
            std::vector<boost::uint8_t> descriptor;

            TsDescriptor()
                : descriptor_tag(0)
                , descriptor_length(0)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & descriptor_tag;
                ar & descriptor_length;
                descriptor.resize(descriptor_length);
                ar & framework::container::make_array(&descriptor.front(), descriptor.size());
            }
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_TS_TS_DESCRIPTOR_H_
