// EBML_Element.h

#ifndef _JUST_AVFORMAT_MKV_EBML_EBML_ELEMENT_H_
#define _JUST_AVFORMAT_MKV_EBML_EBML_ELEMENT_H_

#include "just/avformat/mkv/ebml/EBML_Type.h"
#include "just/avformat/mkv/ebml/EBML_Archive.h"

namespace just
{
    namespace avformat
    {

        // EBML_Element
        template <
            boost::uint32_t _Id, 
            typename _Data
        >
        struct EBML_Element
            : EBML_ElementHeader
            , _Data
        {
            EBML_Element()
                : EBML_ElementHeader(_Id)
            {
            }

            static boost::uint32_t const StaticId = _Id;

            SERIALIZATION_SPLIT_MEMBER();

            template <typename Archive>
            void save(
                Archive & ar) const
            {
                if (empty()) {
                    return;
                }

                ar << (EBML_ElementHeader const &)(*this);
                const_cast<EBML_Element &>(*this)._Data::serialize(ar);
            }

            template <typename Archive>
            void load(
                Archive & ar)
            {
                ar >> (EBML_ElementHeader &)(*this);

                if (empty()) {
                    return;
                }

                _Data::serialize(ar);
            }
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MKV_EBML_EBML_ELEMENT_H_
