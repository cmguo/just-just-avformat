// AviVector.h

#ifndef _JUST_AVFORMAT_AVI_BOX_AVI_VECTOR_H_
#define _JUST_AVFORMAT_AVI_BOX_AVI_VECTOR_H_

#include "just/avformat/avi/box/AviBoxContext.h"

#include <util/serialization/Serialization.h>
#include <util/serialization/SplitMember.h>

namespace just
{
    namespace avformat
    {

        template <typename _Ty>
        struct AviVector
            : std::vector<_Ty>
        {
            AviVector()
            {
            }

            AviVector & operator=(
                std::vector<_Ty> const & v)
            {
                (std::vector<_Ty> &)(*this) = v;
                return *this;
            }

            SERIALIZATION_SPLIT_MEMBER();

            template <typename Archive>
            void load(
                Archive & ar)
            {
                AviBoxContext * ctx = (AviBoxContext *)ar.context();
                this->clear();
                std::streampos data_end = ctx->data_ends.back();
                while (ar.tellg() < data_end) {
                    _Ty t;
                    ar >> t;
                    if (ar) {
                        this->push_back(t);
                    } else {
                        break;
                    }
                }
                assert(!ar || ar.tellg() == data_end);
            }

            template <typename Archive>
            void save(
                Archive & ar) const
            {
                typename AviVector::const_iterator iter = this->begin();
                typename AviVector::const_iterator iend = this->end();
                while (iter != iend) {
                    ar << *iter;
                    if (ar) {
                        ++iter;
                    } else {
                        break;
                    }
                }
            }
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_AVI_BOX_AVI_VECTOR_H_
