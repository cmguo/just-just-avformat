// EBML_Vector.h

#ifndef _PPBOX_AVFORMAT_MKV_EBML_EBML_VECTOR_H_
#define _PPBOX_AVFORMAT_MKV_EBML_EBML_VECTOR_H_

namespace ppbox
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
} // namespace ppbox

namespace util
{
    namespace serialization
    {
        template <
            typename _Ty
        >
        struct is_sigle_unit<ppbox::avformat::EBML_Vector<_Ty> >
            : boost::true_type
        {
        };

    }
}

#endif // _PPBOX_AVFORMAT_MKV_EBML_EBML_VECTOR_H_
