// Mp4BoxVector.hpp

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_VECTOR_HPP_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_VECTOR_HPP_

#include "ppbox/avformat/mp4/box/Mp4BoxVector.h"

#include <stdio.h>

namespace ppbox
{
    namespace avformat
    {


        template <typename T>
        T * Mp4BoxVector::find_item_as(
            std::string const & path)
        {
            Mp4Box * b = find_item(path);
            if (b) {
                return b->is<T>() ? &b->as<T>() : NULL;
            } else {
                return NULL;
            }
        }

        template <typename T>
        T const * Mp4BoxVector::find_item_as(
            std::string const & path) const
        {
            Mp4Box const * b = find_item(path);
            if (b) {
                return b->is<T>() ? &b->as<T>() : NULL;
            } else {
                return NULL;
            }
        }

        template <typename T>
        T * Mp4BoxVector::find_item_as(
            Mp4Box & box, 
            std::string const & path)
        {
            Mp4Box * b = find_item(box, path);
            if (b) {
                return b->is<T>() ? &b->as<T>() : NULL;
            } else {
                return NULL;
            }
        }

        template <typename T>
        T const * Mp4BoxVector::find_item_as(
            Mp4Box const & box, 
            std::string const & path)
        {
            Mp4Box * b = find_item(box, path);
            if (b) {
                return b->is<T>() ? &b->as<T>() : NULL;
            } else {
                return NULL;
            }
        }


        template <
            typename T, 
            typename Mp4BoxHeader::id_type type
        >
        Mp4BoxVectorRegister<T, type>::Mp4BoxVectorRegister()
        {
            printf("Mp4BoxVectorRegister<T, %u>\n", type);
            Mp4BoxVector::reg_box(type, get_vec);
        }

        template <
            typename T, 
            typename Mp4BoxHeader::id_type type
        >
        Mp4BoxVector * Mp4BoxVectorRegister<T, type>::get_vec(
            Mp4Box & box)
        {
            return static_cast<Mp4BoxVector *>(&box.as<T>());
        }

        template <
            typename T, 
            typename Mp4BoxHeader::id_type type
        >
        Mp4BoxVectorRegister<T, type> const Mp4BoxContainer<T, type>::reg;

        template <
            typename T, 
            typename Mp4BoxHeader::id_type type
        >
        Mp4BoxContainer<T, type>::Mp4BoxContainer()
        {
            reg_ = &reg;
        }

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_VECTOR_HPP_
