// Mp4BoxWrapper.h

#ifndef _PPBOX_AVFORMAT_MP4_LIB_MP4_BOX_WRAPPER_H_
#define _PPBOX_AVFORMAT_MP4_LIB_MP4_BOX_WRAPPER_H_

#include "ppbox/avformat/mp4/box/Mp4Box.h"

namespace ppbox
{
    namespace avformat
    {

        template <typename T>
        class Mp4BoxWrapper
        {
        public:
            Mp4BoxWrapper(
                Mp4Box & box)
                : box_(&box)
                , data_(&box.as<T>())
            {
            }

            Mp4BoxWrapper(
                Mp4Box * box)
                : box_(box)
                , data_(box ? &box->as<T>() : NULL)
            {
            }

        public:
            Mp4Box & box()
            {
                return *box_;
            }

            T & data()
            {
                return *data_;
            }

            T * operator->()
            {
                return data_;
            }

            Mp4Box const & box() const
            {
                return *box_;
            }

            T const & data() const
            {
                return *data_;
            }

            T const * operator->() const
            {
                return data_;
            }

        protected:
            Mp4Box * box_;
            T * data_;
        };

        template <typename T>
        class Mp4BoxVectorWrapper
        {
        public:
            Mp4BoxVectorWrapper(
                T & vec)
                : vec_(&vec)
            {
            }

            Mp4BoxVectorWrapper(
                T * vec)
                : vec_(vec)
            {
            }

        public:
            Mp4Box * find_item(
                std::string const & path)
            {
                return vec_ ? vec_->find_item(path) : NULL;
            }

            Mp4Box const * find_item(
                std::string const & path) const
            {
                return vec_ ? vec_->find_item(path) : NULL;
            }

            template <typename U>
            U * find_item_as(
                std::string const & path)
            {
                return vec_ ? vec_->find_item_as<U>(path) : NULL;
            }

            template <typename U>
            U const * find_item_as(
                std::string const & path) const
            {
                return vec_ ? vec_->find_item_as<U>(path) : NULL;
            }

        protected:
            T * vec_;
        };

        template <typename T>
        class Mp4BoxWrapper2
            : public Mp4BoxWrapper<T>
            , public Mp4BoxVectorWrapper<T>
        {
        public:
            Mp4BoxWrapper2(
                Mp4Box & box)
                : Mp4BoxWrapper<T>(box)
                , Mp4BoxVectorWrapper<T>(this->data_)
            {
            }

            Mp4BoxWrapper2(
                Mp4Box * box)
                : Mp4BoxWrapper<T>(box)
                , Mp4BoxVectorWrapper<T>(this->data_)
            {
            }
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_LIB_MP4_BOX_WRAPPER_H_
