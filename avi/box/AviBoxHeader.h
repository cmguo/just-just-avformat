// AviBoxHeader.h

#ifndef _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_HEADER_H_
#define _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_HEADER_H_

#include <util/serialization/Serialization.h>
#include <util/serialization/SplitMember.h>
#include <util/serialization/Array.h>

#include "ppbox/avbase/FourCC.h"

namespace ppbox
{
    namespace avformat
    {

        using framework::container::make_array;

        struct AviBoxHeader
        {
            typedef boost::uint32_t id_type;

            typedef boost::uint32_t size_type;

            static boost::uint32_t const RIFF = MAKE_FOURC_TYPE('R', 'I', 'F', 'F');
            static boost::uint32_t const LIST = MAKE_FOURC_TYPE('L', 'I', 'S', 'T');

            union {
                boost::uint32_t type;
                char type_str[4];
            };
            boost::uint32_t size;
            union {
                boost::uint32_t type2;
                char type_str2[4];
            };

            AviBoxHeader(
                boost::uint32_t t = 0)
                : type(t)
                , size(0)
                , type2(t)
            {
            }

            AviBoxHeader(
                boost::uint32_t t,
                boost::uint32_t s)
                : type(t)
                , size(s)
                , type2(t)
            {
            }

            template <
                typename Archive
            >
            void serialize(
                Archive & ar)
            {
                ar & make_array(type_str);
                ar & size;
                if (type == RIFF || type == LIST)
                    ar & make_array(type_str2);
                else
                    type2 = type;
            }

            boost::uint32_t id() const
            {
                return type2;
            }

            void id(
                boost::uint32_t id)
            {
                type = type2 = id;
            }

            void riff_id(
                boost::uint32_t id)
            {
                type = RIFF;
                type2 = id;
            }

            void list_id(
                boost::uint32_t id)
            {
                type = LIST;
                type2 = id;
            }

            boost::uint32_t head_size() const
            {
                return type == type2 ? 8 : 12;
            }

            boost::uint32_t data_size() const
            {
                return type == type2 ? size : size - 4;
            }

            void data_size(
                boost::uint32_t sz)
            {
                if (type == type2)
                    size = sz;
                else
                    size = sz + 4;
            }

            boost::uint64_t byte_size() const
            {
                return 8 + size;
            }

            void byte_size(
                boost::uint64_t sz)
            {
                size = sz - 8;
            }
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_HEADER_H_
