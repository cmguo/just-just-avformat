// Mp4Vector.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_VECTOR_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_VECTOR_H_

#include "ppbox/avformat/mp4/box/Mp4BoxContext.h"

#include <util/serialization/Serialization.h>
#include <util/serialization/SplitMember.h>

namespace ppbox
{
    namespace avformat
    {

        template <typename _Ty>
        struct Mp4Vector
            : std::vector<_Ty>
        {
            Mp4Vector()
            {
            }

            Mp4Vector & operator=(
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
                Mp4BoxContext * ctx = (Mp4BoxContext *)ar.context();
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
                typename Mp4Vector::const_iterator iter = this->begin();
                typename Mp4Vector::const_iterator iend = this->end();
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
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_VECTOR_H_
