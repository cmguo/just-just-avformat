// TsVector.h

#ifndef _PPBOX_AVFORMAT_TS_TS_VECTOR_H_
#define _PPBOX_AVFORMAT_TS_TS_VECTOR_H_

#include "ppbox/avformat/ts/TsArchive.h"

namespace ppbox
{
    namespace avformat
    {

        // size == 0 时，由数组元素自己检查是否已经到最后一个，此时ar置为failed状态，TsVector会将流回退到最后成功的位置。

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
                boost::uint64_t byte_cur = (boost::uint64_t)ar.tellg();
                boost::uint64_t byte_end = byte_cur + size_;
                while (size_ == 0 || byte_cur < byte_end) {
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
                        byte_cur = (boost::uint64_t)ar.tellg();
                    } else {
                        if (size_ == 0 && ar.failed()) {
                            ar.clear();
                            ar.seekg(byte_cur, std::ios::beg);
                        }
                        break;
                    }
                }
                if (has_suffer_) {
                    while (byte_cur < byte_end) {
                        boost::uint8_t suffer(0xff);
                        ar >> suffer;
                        ++byte_cur;
                    }
                }
                assert(!ar || size_ == 0 || byte_cur == byte_end);
            }

            template <typename Archive>
            void save(
                Archive & ar) const
            {
                boost::uint64_t byte_cur = (boost::uint64_t)ar.tellp();
                boost::uint64_t byte_end = byte_cur + size_;
                typename TsVector::const_iterator iter = this->begin();
                typename TsVector::const_iterator iend = this->end();
                while (size_ == 0 || byte_cur < byte_end && iter != iend) {
                    ar << *iter;
                    if (ar) {
                        byte_cur = (boost::uint64_t)ar.tellp();
                        ++iter;
                    } else {
                        break;
                    }
                }
                if (has_suffer_) {
                    while (byte_cur < byte_end) {
                        boost::uint8_t suffer(0xff);
                        ar << suffer;
                        ++byte_cur;
                    }
                }
                assert(!ar || size_ == 0 || (byte_cur == byte_end && iter == iend));
            }

        private:
            bool has_suffer_;
            boost::uint64_t size_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_TS_TS_VECTOR_H_
