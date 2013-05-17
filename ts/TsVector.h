// TsVector.h

#ifndef _PPBOX_AVFORMAT_TS_TS_VECTOR_H_
#define _PPBOX_AVFORMAT_TS_TS_VECTOR_H_

#include "ppbox/avformat/ts/TsArchive.h"

namespace ppbox
{
    namespace avformat
    {

        template <typename _Ty>
        struct TsVector
            : std::vector<_Ty>
        {
            TsVector()
                : has_suffer_(false)
                , size_(0)
            {
            }

            SERIALIZATION_SPLIT_MEMBER();

            void set_byte_size(
                boost::uint64_t size)
            {
                has_suffer_ = false;
                size_ = size;
            }

            void set_byte_size_with_suffer(
                boost::uint64_t size)
            {
                has_suffer_ = true;
                size_ = size;
            }

            template <typename Archive>
            void load(
                Archive & ar)
            {
                this->clear();
                boost::uint64_t byte_end = (boost::uint64_t)ar.tellg() + size_;
                while ((boost::uint64_t)ar.tellg() < byte_end) {
                    if (has_suffer_) {
                        std::basic_streambuf<boost::uint8_t> * buf = ar.rdbuf();
                        if (buf->sgetc() == 0xff) {
                            break;
                        }
                    }
                    _Ty t;
                    ar >> t;
                    if (ar) {
                        this->push_back(t);
                    } else {
                        break;
                    }
                }
                if (has_suffer_) {
                    while ((boost::uint64_t)ar.tellg() < byte_end) {
                        boost::uint8_t suffer(0xff);
                        ar >> suffer;
                    }
                }
                assert(!ar || (boost::uint64_t)ar.tellg() == byte_end);
            }

            template <typename Archive>
            void save(
                Archive & ar) const
            {
                boost::uint64_t byte_end = (boost::uint64_t)ar.tellp() + size_;
                typename TsVector::const_iterator iter = this->begin();
                typename TsVector::const_iterator iend = this->end();
                while ((boost::uint64_t)ar.tellp() < byte_end && iter != iend) {
                    ar << *iter;
                    if (ar) {
                        ++iter;
                    } else {
                        break;
                    }
                }
                if (has_suffer_) {
                    while ((boost::uint64_t)ar.tellp() < byte_end) {
                        boost::uint8_t suffer(0xff);
                        ar << suffer;
                    }
                }
                assert(!ar || ((boost::uint64_t)ar.tellp() == byte_end && iter == iend));
            }

        private:
            bool has_suffer_;
            boost::uint64_t size_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_TS_TS_VECTOR_H_
