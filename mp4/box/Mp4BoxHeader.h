// Mp4BoxHeader.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_HEADER_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_HEADER_H_

#include <util/serialization/Serialization.h>
#include <util/serialization/SplitMember.h>
#include <util/serialization/Array.h>

namespace ppbox
{
    namespace avformat
    {

        using framework::container::make_array;

        struct Mp4BoxHeader
        {
            typedef boost::uint32_t id_type;

            typedef boost::uint64_t size_type;

            boost::uint32_t size;
            union {
                boost::uint32_t type;
                char type_str[4];
            };
            boost::uint64_t largesize;

            Mp4BoxHeader(
                boost::uint32_t t = 0)
                : size(0)
                , type(t)
                , largesize(0)
            {
            }

            Mp4BoxHeader(
                boost::uint32_t t,
                boost::uint64_t s)
                : size(boost::uint32_t(s))
                , type(t)
                , largesize(size)
            {
                if (size != largesize) {
                    size = 1;
                }
            }

            SERIALIZATION_SPLIT_MEMBER();

            template <
                typename Archive
            >
            void save(
                Archive & ar) const
            {
                ar & size;
                ar & make_array(type_str);
                if (size == 1) {
                    ar & largesize;
                }
            }

            template <
                typename Archive
            >
            void load(
                Archive & ar)
            {
                ar & size;
                ar & make_array(type_str);
                if (size == 1) {
                    ar & largesize;
                } else {
                    largesize = size;
                }
            }

            void clear()
            {
                size = 0;
            }

            bool empty() const
            {
                return size == 0;
            }

            boost::uint32_t id() const
            {
                return type;
            }

            void id(
                boost::uint32_t id)
            {
                type = id;
            }

            boost::uint64_t head_size() const
            {
                return (size == 1) ? 16 : 8;
            }

            boost::uint64_t data_size() const
            {
                return (size  == 1) ? largesize - 16 : largesize - 8;
            }

            void data_size(
                boost::uint64_t sz)
            {
                largesize = sz + 8;
                size = (boost::uint32_t)largesize;
                if (size != largesize) {
                    size = 1;
                    largesize += 8;
                }
            }

            boost::uint64_t byte_size() const
            {
                return largesize;
            }
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_HEADER_H_
