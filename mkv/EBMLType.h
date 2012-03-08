//EBMLType.h

#ifndef _PPBOX_AVFORMAT_MKV_EBML_TYPE_H_
#define _PPBOX_AVFORMAT_MKV_EBML_TYPE_H_

#include <util/serialization/VariableNumber.h>
#include <util/serialization/Array.h>
#include <util/archive/BigEndianBinaryIArchive.h>
#include <util/archive/BigEndianBinaryOArchive.h>
#include <util/serialization/stl/vector.h>
#include <util/serialization/SplitMember.h>

#include <framework/system/VariableNumber.h>

namespace ppbox
{
    namespace avformat
    {

        typedef util::archive::BigEndianBinaryOArchive<char> EBMLOArchive;

        typedef util::archive::BigEndianBinaryIArchive<boost::uint8_t> EBMLIArchive;

        //EBML
        struct EBML_Element_Header
        {
            framework::system::VariableNumber<boost::uint32_t> id;
            framework::system::VariableNumber<boost::uint32_t> size;

            EBML_Element_Header(
                boost::uint32_t id)
                : id(id)
            {
            }

            EBML_Element_Header(
                boost::uint32_t id,
                boost::uint32_t size)
                : id(id)
                , size(size)
            {
            }

            template <typename Archive>
            boost::uint8_t serialize(Archive & ar)
            {
                if (size == framework::system::VariableNumber<boost::uint32_t>())
                    return 1;
                ar & id
                    & size;
                return 0;
            }

            size_t byte_size() const
            {
                return size == framework::system::VariableNumber<boost::uint32_t>()
                    ? 0 : id.size() + size.size() + size;
            }

            size_t data_size() const
            {
                return size;
            }
        };

        template <
            boost::uint32_t ID
        >
        struct Element_Header
            : EBML_Element_Header
        {
            Element_Header()
                : EBML_Element_Header(ID)
            {
            }

            template <typename Archive>
            boost::uint8_t serialize(Archive ar)
            {
                return EBML_Element_Header::serialize(ar);
            }
        };

        struct EBML_UInt
            : EBML_Element_Header
        {
            boost::uint64_t v;

            EBML_UInt(
                boost::uint32_t id)
                : EBML_Element_Header(id)
            {
            }

            EBML_UInt(
                boost::uint32_t id,
                boost::uint64_t v)
                : EBML_Element_Header(id, calc_size(v))
                , v(v)
            {
            }

            EBML_UInt & operator=(
                boost::uint64_t v)
            {
                this->v = v;
                this->size = calc_size(v);
                return *this;
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                if(EBML_Element_Header::serialize(ar))
                    return;

                boost::uint64_t t = 
                    framework::system::BytesOrder::host_to_big_endian(v);
                ar & framework::container::make_array(
                    (boost::uint8_t *)&t + 8 - size, size);
            }

        public:
            static boost::uint32_t calc_size(
                boost::uint64_t n) //计算当前数据内容大小
            {//须由高位至地位比较
                if (0xFFFFFFFF00000000ULL & n) {
                    if(0xFFFF000000000000ULL & n) {
                        if(0xFF00000000000000ULL & n)
                            return 8;
                        else
                            return 7;
                    } else {
                        if(0xFF0000000000ULL & n)
                            return 6;
                        else
                            return 5;
                    }
                } else {
                    if(0xFFFF0000ULL & n) {
                        if(0xFF000000ULL & n)
                            return 4;
                        else
                            return 3;
                    } else {
                        if(0xFF00ULL & n)
                            return 2;
                        else
                            return 1;
                    }
                }
            }
        };

        struct EBML_Int
            : EBML_Element_Header
        {
            boost::int64_t v;

            EBML_Int(
                boost::uint32_t id)
                : EBML_Element_Header(id)
            {
            }

            EBML_Int(
                boost::uint32_t id,
                boost::int64_t v)
                : EBML_Element_Header(id, calc_size(v))
                , v(v)
            {
            }

            EBML_Int & operator=(
                boost::int64_t v)
            {
                this->v = v;
                this->size = calc_size(v);
                return *this;
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                if(EBML_Element_Header::serialize(ar))
                    return;

                boost::int64_t tmp =
                    framework::system::BytesOrder::host_to_big_endian(v);
                ar & framework::container::make_array(
                    (boost::uint8_t *)&tmp + 8 - size, size);
            }

        public:
            static boost::uint32_t calc_size(
                boost::int64_t n)
            {
                if(0x8000000000000000ULL & n) {
                    return (EBML_UInt::calc_size(n & 0x7FFFFFFFFFFFFFFFULL) + 1);
                } else {
                    return EBML_UInt::calc_size(n) + 1;
                }
            }
        };

        struct EBML_String
            : EBML_Element_Header
        {
            std::string v;

            EBML_String(
                boost::uint32_t id)
                : EBML_Element_Header(id)
            {
            }

            EBML_String(
                boost::uint32_t id,
                std::string str)
                : EBML_Element_Header(id, str.size())
                , v(str)
            {
            }

            EBML_String & operator=(
                std::string const & v)
            {
                this->v = v;
                this->size = v.size();
                return *this;
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                if(EBML_Element_Header::serialize(ar))
                    return;

                ar & framework::container::make_array(v.c_str(), size);
            }
        };

        struct EBML_Bool
            : EBML_Element_Header
        {
            boost::uint8_t v;

            EBML_Bool(
                boost::uint32_t id)
                : EBML_Element_Header(id)
            {
            }

            EBML_Bool(
                boost::uint32_t id,
                bool v)
                : EBML_Element_Header(id, 1)
                , v(v)
            {
            }

            EBML_Bool & operator=(
                bool b)
            {
                this->v = b ? 1 : 0;
                this->size = 1;
                return *this;
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                if(EBML_Element_Header::serialize(ar))
                    return;

                ar & v;
            }
        };

        struct EBML_Float
            : EBML_Element_Header
        {
            EBML_Float(
                boost::uint32_t id)
                : EBML_Element_Header(id)
            {
            }

            EBML_Float(
                boost::uint32_t id,
                float f)
                : EBML_Element_Header(id, 4)
                , f(f)
            {
            }

            EBML_Float(
                boost::uint32_t id,
                double d)
                : EBML_Element_Header(id, 8)
                , d(d)
            {
            }

            EBML_Float & operator=(
                float v)
            {
                this->f = v;
                this->size = 4;
                return *this;
            }

            EBML_Float & operator=(
                double v)
            {
                this->d = v;
                this->size = 8;
                return *this;
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                if(EBML_Element_Header::serialize(ar))
                    return;

                if (framework::system::VariableNumber<boost::uint32_t>((boost::uint32_t)4) == size)
                    ar & f;
                else
                    ar & d;
            }

        private:
            union
            {
                float f;
                double d;
            };
        };

        struct EBML_Binary
            : EBML_Element_Header
        {
            std::vector<boost::uint8_t> v;

            EBML_Binary(
                boost::uint32_t id)
                : EBML_Element_Header(id)
            {
            }

            EBML_Binary(
                boost::uint32_t id,
                std::vector<boost::uint8_t> v)
                : EBML_Element_Header(id, v.size())
                , v(v)
            {
            }
            
            EBML_Binary & operator=(
                std::vector<boost::uint8_t> &v)
            {
                this->v = v;
                this->size = v.size();
                return *this;
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                if(EBML_Element_Header::serialize(ar))
                    return;

                util::serialization::serialize_collection(
                    ar, v, v.size());
            }
        };

    }//avformat
}//ppbox

#endif

