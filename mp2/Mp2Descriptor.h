// Mp2Descriptor.h

#ifndef _JUST_AVFORMAT_MP2_MP2_DESCRIPTOR_H_
#define _JUST_AVFORMAT_MP2_MP2_DESCRIPTOR_H_

namespace just
{
    namespace avformat
    {

        struct Mp2Descriptor
        {
            boost::uint8_t descriptor_tag;
            boost::uint8_t descriptor_length;
            std::vector<boost::uint8_t> descriptor;

            Mp2Descriptor()
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
} // namespace just

#endif // _JUST_AVFORMAT_MP2_MP2_DESCRIPTOR_H_
