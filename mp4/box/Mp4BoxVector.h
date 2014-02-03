// Mp4FullBox.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_VECTOR_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_VECTOR_H_

#include "ppbox/avformat/mp4/box/Mp4Vector.h"

#include <framework/container/Array.h>

namespace ppbox
{
    namespace avformat
    {

        using framework::container::make_array;

        template <
            typename T, 
            typename Mp4Box::type_t id
        >
        struct Mp4BoxContainer
            : Mp4BoxVector
        {
        private:
            static Mp4BoxVector * get_vec(
                Mp4Box & box)
            {
                return static_cast<Mp4BoxVector *>(&box.as<T>());
            }

        private:
            static Mp4BoxVector::Register reg;
        };

        template <
            typename T, 
            typename Mp4Box::type_t id
        >
        Mp4BoxVector::Register Mp4BoxContainer<T, id>::reg(type, Mp4BoxContainer<T, id>::get_vec);

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_VECTOR_H_
