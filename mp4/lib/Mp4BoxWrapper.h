// Mp4BoxWrapper.h

#ifndef _PPBOX_AVFORMAT_MP4_LIB_MP4_BOX_WRAPPER_H_
#define _PPBOX_AVFORMAT_MP4_LIB_MP4_BOX_WRAPPER_H_

#include "ppbox/avformat/mp4/box/Mp4Box.h"
#include "ppbox/avformat/mp4/box/Mp4BoxVector.h"

namespace ppbox
{
    namespace avformat
    {

        struct create_new_t {};

        template <typename T>
        class Mp4BoxWrapper
        {
        public:
            Mp4BoxWrapper(
                Mp4Box & box)
                : box_(&box)
                , data_(box.is<T>() ? &box.as<T>() : NULL)
            {
            }

            Mp4BoxWrapper(
                Mp4Box * box)
                : box_(box)
                , data_((box && box->is<T>()) ? &box->as<T>() : NULL)
            {
            }

        public:
            bool null() const
            {
                return box_ == NULL;
            }

            void assign(
                Mp4Box * box)
            {
                box_ = box;
                data_ = (box && box->is<T>()) ? &box->as<T>() : NULL;
            }

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

        class Mp4BoxVectorWrapper
        {
        public:
            Mp4BoxVectorWrapper(
                Mp4BoxVector & vec)
                : vec_(&vec)
            {
            }

            Mp4BoxVectorWrapper(
                Mp4BoxVector * vec)
                : vec_(vec)
            {
            }

        public:
            Mp4BoxVector & vector()
            {
                return *vec_;
            }

            Mp4BoxVector const & vector() const
            {
                return *vec_;
            }

        public:
            Mp4Box * find_item(
                boost::uint32_t type)
            {
                return vec_ ? vec_->find_item(type) : NULL;
            }

            Mp4Box const * find_item(
                boost::uint32_t type) const
            {
                return vec_ ? vec_->find_item(type) : NULL;
            }

            Mp4Box * create_item(
                boost::uint32_t type)
            {
                return vec_ ? vec_->create_item(type) : NULL;
            }

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

            Mp4Box * create_item(
                std::string const & path)
            {
                return vec_ ? vec_->create_item(path) : NULL;
            }

            template <typename U>
            U * find_item_as(
                boost::uint32_t type)
            {
                return vec_ ? vec_->find_item_as<U>(type) : NULL;
            }

            template <typename U>
            U const * find_item_as(
                boost::uint32_t type) const
            {
                return vec_ ? vec_->find_item_as<U>(type) : NULL;
            }

            template <typename U>
            U * create_item_as(
                boost::uint32_t type)
            {
                return vec_ ? vec_->create_item_as<U>(type) : NULL;
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

            template <typename U>
            U * create_item_as(
                std::string const & path)
            {
                return vec_ ? vec_->create_item_as<U>(path) : NULL;
            }

        protected:
            Mp4BoxVector * vec_;
        };

        template <typename T>
        class Mp4BoxWrapper2
            : public Mp4BoxWrapper<T>
            , public Mp4BoxVectorWrapper
        {
        public:
            Mp4BoxWrapper2(
                Mp4Box & box)
                : Mp4BoxWrapper<T>(box)
                , Mp4BoxVectorWrapper(this->data_)
            {
            }

            Mp4BoxWrapper2(
                Mp4Box * box)
                : Mp4BoxWrapper<T>(box)
                , Mp4BoxVectorWrapper(this->data_)
            {
            }
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_LIB_MP4_BOX_WRAPPER_H_
