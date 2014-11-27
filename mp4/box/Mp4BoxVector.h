// Mp4BoxVector.h

#ifndef _JUST_AVFORMAT_MP4_BOX_MP4_BOX_VECTOR_H_
#define _JUST_AVFORMAT_MP4_BOX_MP4_BOX_VECTOR_H_

#include "just/avformat/mp4/box/Mp4BoxContext.h"

#include <util/serialization/Serialization.h>
#include <util/serialization/SplitMember.h>

#include <just/avbase/FourCC.h>

#include <framework/container/Array.h>

namespace just
{
    namespace avformat
    {

        using framework::container::make_array;


        struct Mp4BoxVector
            : std::vector<Mp4Box *>
        {
            typedef Mp4BoxVector * (* get_vec_t)(Mp4Box &);

            ~Mp4BoxVector()
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
                Mp4BoxContext * ctx = (Mp4BoxContext *)ar.context();
                this->clear();
                std::streampos data_end = ctx->data_ends.back();
                while (ar.tellg() < data_end) {
                    Mp4Box * t = new Mp4Box;
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
                    Mp4Box const * b)
                {
                    return b->type == type_;
                }
                boost::uint32_t type_;
            };

            Mp4Box * find_item(
                boost::uint32_t type)
            {
                iterator iter = std::find_if(
                    begin(), end(), equal_box_type(type));
                if (iter == end())
                    return NULL;
                return *iter;
            }

            Mp4Box const * find_item(
                boost::uint32_t type) const
            {
                return const_cast<Mp4BoxVector *>(this)->find_item(type);
            }

            Mp4Box * create_item(
                boost::uint32_t type)
            {
                push_back(new Mp4Box(type));
                return back();
            }

            Mp4Box * find_item(
                std::string const & path)
            {
                boost::uint32_t type = 
                    just::avbase::FourCC::from_string(path.substr(1, 4));
                Mp4Box * box = find_item(type);
                if (box == NULL)
                    return NULL;
                if (path.size() == 5)
                    return box;
                get_vec_t get_vec = get_vec_map()[type];
                assert(get_vec);
                if (get_vec == NULL)
                    return NULL;
                Mp4BoxVector * vec = get_vec(*box);
                return vec->find_item(path.substr(5));
            }

            Mp4Box const * find_item(
                std::string const & path) const
            {
                return const_cast<Mp4BoxVector *>(this)->find_item(path);
            }

            Mp4Box * create_item(
                std::string const & path)
            {
                boost::uint32_t type = 
                    just::avbase::FourCC::from_string(path.substr(1, 4));
                if (path.size() == 5) { // always create one
                    return create_item(type);
                }
                Mp4Box * box = find_item(type);
                if (box == NULL) {
                    box = create_item(type);
                }
                get_vec_t get_vec = get_vec_map()[type];
                assert(get_vec);
                if (get_vec == NULL)
                    return NULL;
                Mp4BoxVector * vec = get_vec(*box);
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
            static std::map<Mp4BoxHeader::id_type, get_vec_t> & get_vec_map()
            {
                static std::map<Mp4BoxHeader::id_type, get_vec_t> m;
                return m;
            }
        };

        template <
            typename T, 
            typename Mp4BoxHeader::id_type type
        >
        struct Mp4BoxVectorRegister
        {
            Mp4BoxVectorRegister();

        private:
            static Mp4BoxVector * get_vec(
                Mp4Box & box);
        };

        template <
            typename T, 
            typename Mp4BoxHeader::id_type type
        >
        struct Mp4BoxContainer
            : Mp4BoxVector
        {
        public:
            Mp4BoxContainer();

        private:
            Mp4BoxVectorRegister<T, type> const * reg_;

        private:
            static Mp4BoxVectorRegister<T, type> const reg;
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MP4_BOX_MP4_BOX_VECTOR_H_
