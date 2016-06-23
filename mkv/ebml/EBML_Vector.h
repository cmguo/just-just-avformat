// EBML_Vector.h

#ifndef _JUST_AVFORMAT_MKV_EBML_EBML_VECTOR_H_
#define _JUST_AVFORMAT_MKV_EBML_EBML_VECTOR_H_

namespace just
{
    namespace avformat
    {

        template <typename _Ty>
        struct EBML_Vector
            : std::vector<_Ty>
        {
            EBML_Vector()
            {
            }

            SERIALIZATION_SPLIT_MEMBER();

            template <typename Archive>
            void load(
                Archive & ar)
            {
                this->clear();
                while (true) {
                    _Ty item;
                    ar >> item;
                    if (item.empty()) {
                        break;
                    }
                    this->push_back(item);
                }
            }

            template <typename Archive>
            void save(
                Archive & ar) const
            {
                typename EBML_Vector::const_iterator iter = this->begin();
                typename EBML_Vector::const_iterator iend = this->end();
                while (iter != iend) {
                    ar << *iter;
                    if (ar) {
                        ++iter;
                    } else {
                        break;
                    }
                }
            }
        };


    } // namespace avformat
} // namespace just

namespace util
{
    namespace serialization
    {
        template <
            typename _Ar, 
            typename _Ty
        >
        struct is_single_unit<_Ar, just::avformat::EBML_Vector<_Ty> >
            : boost::true_type
        {
        };

    }
}

#endif // _JUST_AVFORMAT_MKV_EBML_EBML_VECTOR_H_
