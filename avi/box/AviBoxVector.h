// AviBoxVector.h

#ifndef _JUST_AVFORMAT_AVI_BOX_AVI_BOX_VECTOR_H_
#define _JUST_AVFORMAT_AVI_BOX_AVI_BOX_VECTOR_H_

#include "just/avformat/avi/box/AviBoxContext.h"

#include <util/serialization/Serialization.h>
#include <util/serialization/SplitMember.h>

#include <just/avbase/FourCC.h>

#include <framework/container/Array.h>

namespace just
{
    namespace avformat
    {

        using framework::container::make_array;


        struct AviBoxVector
            : std::vector<AviBox *>
        {
            typedef AviBoxVector * (* get_vec_t)(AviBox &);

            ~AviBoxVector()
            {
                for (size_t i = 0; i < size(); ++i) {
                    delete at(i);
                }
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
                    AviBox * t = new AviBox;
                    ar >> *t;
                    if (ar) {
                        push_back(t);
                    } else {
                        delete t;
                        break;
                    }
                }
                assert(!ar || ar.tellg() == data_end);
            }

            template <typename Archive>
            void save(
                Archive & ar) const
            {
                const_iterator iter = this->begin();
                const_iterator iend = this->end();
                while (iter != iend) {
                    ar << **iter;
                    if (ar) {
                        ++iter;
                    } else {
                        break;
                    }
                }
            }

            static void reg_box(
                boost::uint32_t type, 
                get_vec_t get_vec)
            {
                get_vec_map()[type] = get_vec;
            }

            struct equal_box_type
            {
                equal_box_type(
                    boost::uint32_t type)
                    : type_(type)
                {
                }
                bool operator()(
                    AviBox const * b)
                {
                    return b->type == type_;
                }
                boost::uint32_t type_;
            };

            AviBox * find_item(
                boost::uint32_t type)
            {
                iterator iter = std::find_if(
                    begin(), end(), equal_box_type(type));
                if (iter == end())
                    return NULL;
                return *iter;
            }

            AviBox const * find_item(
                boost::uint32_t type) const
            {
                return const_cast<AviBoxVector *>(this)->find_item(type);
            }

            AviBox * create_item(
                boost::uint32_t type)
            {
                push_back(new AviBox(type));
                if (get_vec_map()[type] != NULL)
                    back()->list_id(type);
                return back();
            }

            AviBox * find_item(
                std::string const & path)
            {
                boost::uint32_t type = 
                    just::avbase::FourCC::from_string(path.substr(1, 4));
                AviBox * box = find_item(type);
                if (box == NULL)
                    return NULL;
                if (path.size() == 5)
                    return box;
                get_vec_t get_vec = get_vec_map()[type];
                assert(get_vec);
                if (get_vec == NULL)
                    return NULL;
                AviBoxVector * vec = get_vec(*box);
                return vec->find_item(path.substr(5));
            }

            AviBox const * find_item(
                std::string const & path) const
            {
                return const_cast<AviBoxVector *>(this)->find_item(path);
            }

            AviBox * create_item(
                std::string const & path)
            {
                boost::uint32_t type = 
                    just::avbase::FourCC::from_string(path.substr(1, 4));
                if (path.size() == 5) { // always create one
                    return create_item(type);
                }
                AviBox * box = find_item(type);
                if (box == NULL) {
                    box = create_item(type);
                }
                get_vec_t get_vec = get_vec_map()[type];
                assert(get_vec);
                if (get_vec == NULL)
                    return NULL;
                AviBoxVector * vec = get_vec(*box);
                return vec->create_item(path.substr(5));
            }

            template <typename T>
            T * find_item_as(
                boost::uint32_t type);

            template <typename T>
            T const * find_item_as(
                boost::uint32_t type) const;

            template <typename T>
            T * create_item_as(
                boost::uint32_t type);

            template <typename T>
            T * find_item_as(
                std::string const & path);

            template <typename T>
            T const * find_item_as(
                std::string const & path) const;

            template <typename T>
            T * create_item_as(
                std::string const & path);

        private:
            static std::map<AviBoxHeader::id_type, get_vec_t> & get_vec_map()
            {
                static std::map<AviBoxHeader::id_type, get_vec_t> m;
                return m;
            }
        };

        template <
            typename T, 
            typename AviBoxHeader::id_type type
        >
        struct AviBoxVectorRegister
        {
            AviBoxVectorRegister();

        private:
            static AviBoxVector * get_vec(
                AviBox & box);
        };

        template <
            typename T, 
            typename AviBoxHeader::id_type type
        >
        struct AviBoxContainer
            : AviBoxVector
        {
        public:
            AviBoxContainer();

        private:
            AviBoxVectorRegister<T, type> const * reg_;

        private:
            static AviBoxVectorRegister<T, type> const reg;
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_AVI_BOX_AVI_BOX_VECTOR_H_
