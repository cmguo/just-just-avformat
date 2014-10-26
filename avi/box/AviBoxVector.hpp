// AviBoxVector.hpp

#ifndef _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_VECTOR_HPP_
#define _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_VECTOR_HPP_

#include "ppbox/avformat/avi/box/AviBoxVector.h"

namespace ppbox
{
    namespace avformat
    {


        template <typename T>
        T * AviBoxVector::find_item_as(
            boost::uint32_t type)
        {
            AviBox * b = find_item(type);
            if (b) {
                return b->is<T>() ? &b->as<T>() : NULL;
            } else {
                return NULL;
            }
        }

        template <typename T>
        T const * AviBoxVector::find_item_as(
            boost::uint32_t type) const
        {
            AviBox const * b = find_item(type);
            if (b) {
                return b->is<T>() ? &b->as<T>() : NULL;
            } else {
                return NULL;
            }
        }

        template <typename T>
        T * AviBoxVector::create_item_as(
            boost::uint32_t type)
        {
            AviBox * b = create_item(type);
            if (b) {
                return b->is<T>() ? &b->as<T>() : NULL;
            } else {
                return NULL;
            }
        }

        template <typename T>
        T * AviBoxVector::find_item_as(
            std::string const & path)
        {
            AviBox * b = find_item(path);
            if (b) {
                return b->is<T>() ? &b->as<T>() : NULL;
            } else {
                return NULL;
            }
        }

        template <typename T>
        T const * AviBoxVector::find_item_as(
            std::string const & path) const
        {
            AviBox const * b = find_item(path);
            if (b) {
                return b->is<T>() ? &b->as<T>() : NULL;
            } else {
                return NULL;
            }
        }

        template <typename T>
        T * AviBoxVector::create_item_as(
            std::string const & path)
        {
            AviBox * b = create_item(path);
            if (b) {
                return b->is<T>() ? &b->as<T>() : NULL;
            } else {
                return NULL;
            }
        }

        template <
            typename T, 
            typename AviBoxHeader::id_type type
        >
        AviBoxVectorRegister<T, type>::AviBoxVectorRegister()
        {
            AviBoxVector::reg_box(type, get_vec);
        }

        template <
            typename T, 
            typename AviBoxHeader::id_type type
        >
        AviBoxVector * AviBoxVectorRegister<T, type>::get_vec(
            AviBox & box)
        {
            return static_cast<AviBoxVector *>(&box.as<T>());
        }

        template <
            typename T, 
            typename AviBoxHeader::id_type type
        >
        AviBoxVectorRegister<T, type> const AviBoxContainer<T, type>::reg;

        template <
            typename T, 
            typename AviBoxHeader::id_type type
        >
        AviBoxContainer<T, type>::AviBoxContainer()
        {
            reg_ = &reg;
        }

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_VECTOR_HPP_
