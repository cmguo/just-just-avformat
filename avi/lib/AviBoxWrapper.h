// AviBoxWrapper.h

#ifndef _PPBOX_AVFORMAT_AVI_LIB_AVI_BOX_WRAPPER_H_
#define _PPBOX_AVFORMAT_AVI_LIB_AVI_BOX_WRAPPER_H_

#include "ppbox/avformat/avi/box/AviBox.h"
#include "ppbox/avformat/avi/box/AviBoxVector.h"

namespace ppbox
{
    namespace avformat
    {

        class AviBoxWrapperBase
        {
        public:
            struct create_new_t {};
        };

        template <typename T>
        class AviBoxWrapper
            : public AviBoxWrapperBase
        {
        public:
            AviBoxWrapper(
                AviBox & box)
                : box_(&box)
                , data_(box.is<T>() ? &box.as<T>() : NULL)
            {
            }

            AviBoxWrapper(
                AviBox * box = NULL)
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
                AviBox * box)
            {
                box_ = box;
                data_ = (box && box->is<T>()) ? &box->as<T>() : NULL;
            }

            AviBox & box()
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

            AviBox const & box() const
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
            AviBox * box_;
            T * data_;
        };

        class AviBoxVectorWrapper
        {
        public:
            AviBoxVectorWrapper(
                AviBoxVector & vec)
                : vec_(&vec)
            {
            }

            AviBoxVectorWrapper(
                AviBoxVector * vec = NULL)
                : vec_(vec)
            {
            }

        public:
            void assign(
                AviBoxVector * vec)
            {
                vec_ = vec;
            }

        public:
            AviBoxVector & vector()
            {
                return *vec_;
            }

            AviBoxVector const & vector() const
            {
                return *vec_;
            }

        public:
            AviBox * find_item(
                boost::uint32_t type)
            {
                return vec_ ? vec_->find_item(type) : NULL;
            }

            AviBox const * find_item(
                boost::uint32_t type) const
            {
                return vec_ ? vec_->find_item(type) : NULL;
            }

            AviBox * create_item(
                boost::uint32_t type)
            {
                return vec_ ? vec_->create_item(type) : NULL;
            }

            AviBox * find_item(
                std::string const & path)
            {
                return vec_ ? vec_->find_item(path) : NULL;
            }

            AviBox const * find_item(
                std::string const & path) const
            {
                return vec_ ? vec_->find_item(path) : NULL;
            }

            AviBox * create_item(
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
            AviBoxVector * vec_;
        };

        template <typename T>
        class AviBoxWrapper2
            : public AviBoxWrapper<T>
            , public AviBoxVectorWrapper
        {
        public:
            AviBoxWrapper2(
                AviBox & box)
                : AviBoxWrapper<T>(box)
                , AviBoxVectorWrapper(this->data_)
            {
            }

            AviBoxWrapper2(
                AviBox * box)
                : AviBoxWrapper<T>(box)
                , AviBoxVectorWrapper(this->data_)
            {
            }

            void assign(
                AviBox * box)
            {
                AviBoxWrapper<T>::assign(box);
                AviBoxVectorWrapper::assign(this->data_);
            }
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_LIB_AVI_BOX_WRAPPER_H_
