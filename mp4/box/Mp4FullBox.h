// Mp4FullBox.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_FULL_BOX_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_FULL_BOX_H_

#include "ppbox/avformat/mp4/box/Mp4Vector.h"
#include "ppbox/avformat/mp4/box/Mp4BoxData.h"
#include "ppbox/avformat/mp4/box/Mp4Box.h"

namespace ppbox
{
    namespace avformat
    {

        struct Mp4FullBoxHeader
        {
            union {
                struct {
#ifdef BOOST_BIG_ENDIAN
                    boost::uint32_t version : 8;
                    boost::uint32_t flags : 24;
#else
                    boost::uint32_t flags : 24;
                    boost::uint32_t version : 8;
#endif
                };
                boost::uint32_t dword;
            };

            Mp4FullBoxHeader()
                : dword(0)
            {
            }

            Mp4FullBoxHeader(
                boost::uint8_t v, 
                boost::uint32_t f)
                : dword(v << 24 | f)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & dword;
            }
        };

        template <
            typename T, 
            typename Mp4BoxHeader::id_type id
        >
        struct Mp4FullBoxData
            : Mp4BoxData<T, id>
            , Mp4FullBoxHeader
        {
        };

        struct Mp4BoxVector
            : Mp4Vector<Mp4Box>
        {
            typedef Mp4BoxVector * (* get_vec_t)(Mp4Box &);

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
                    Mp4Box const & b)
                {
                    return b.type == type_;
                }
                boost::uint32_t type_;
            };

            Mp4Box * find_item(
                std::string const & path)
            {
                boost::uint32_t type = 
                    ppbox::avbase::FourCC::from_string(path.substr(1, 4));
                iterator iter = std::find_if(
                    begin(), end(), equal_box_type(type));
                if (iter == end())
                    return NULL;
                return find_item(*iter, path.substr(5));
            }

            Mp4Box const * find_item(
                std::string const & path) const
            {
                return const_cast<Mp4BoxVector *>(this)->find_item(path);
            }

            static Mp4Box * find_item(
                Mp4Box & box, 
                std::string const & path)
            {
                if (path.empty())
                    return &box;
                get_vec_t get_vec = get_vec_map()[box.type];
                assert(get_vec);
                if (get_vec == NULL)
                    return NULL;
                Mp4BoxVector * vec = get_vec(box);
                return vec->find_item(path);
            }

            static Mp4Box const * find_item(
                Mp4Box const & box, 
                std::string const & path)
            {
                return find_item(const_cast<Mp4Box &>(box), path);
            };

            template <typename T>
            T * find_item_as(
                std::string const & path)
            {
                Mp4Box * b = find_item(path);
                if (b) {
                    return &b->as<T>();
                } else {
                    return NULL;
                }
            }

            template <typename T>
            T const * find_item_as(
                std::string const & path) const
            {
                Mp4Box const * b = find_item(path);
                if (b) {
                    return &b->as<T>();
                } else {
                    return NULL;
                }
            }

            template <typename T>
            static T * find_item_as(
                Mp4Box & box, 
                std::string const & path)
            {
                Mp4Box * b = find_item(box, path);
                if (b) {
                    return &b->as<T>();
                } else {
                    return NULL;
                }
            }

            template <typename T>
            static T const * find_item_as(
                Mp4Box const & box, 
                std::string const & path)
            {
                Mp4Box * b = find_item(box, path);
                if (b) {
                    return &b->as<T>();
                } else {
                    return NULL;
                }
            }

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
            Mp4BoxVectorRegister()
            {
                Mp4BoxVector::reg_box(type, get_vec);
            }
        private:
            static Mp4BoxVector * get_vec(
                Mp4Box & box)
            {
                return static_cast<Mp4BoxVector *>(&box.as<T>());
            }
        };

        template <
            typename T, 
            typename Mp4BoxHeader::id_type type
        >
        struct Mp4BoxContainer
            : Mp4BoxVector
        {
        public:
            Mp4BoxContainer()
            {
                (void)&reg;
            }

        private:
            static Mp4BoxVectorRegister<T, type> const reg;
        };

        template <
            typename T, 
            typename Mp4BoxHeader::id_type type
        >
        Mp4BoxVectorRegister<T, type> const Mp4BoxContainer<T, type>::reg;

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_FULL_BOX_H_
