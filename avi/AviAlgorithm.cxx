// AviAlgorithm.cpp

#include "just/avformat/Common.h"
#include "just/avformat/avi/AviAlgorithm.h"
#include "just/avformat/Error.h"

#include <just/avbase/FourCC.h>

#include <framework/system/BytesOrder.h>

namespace just
{
    namespace avformat
    {

        boost::uint64_t avi_head_size(
            std::basic_istream<boost::uint8_t> & is, 
            boost::system::error_code & ec)
        {
            boost::uint32_t const fourccs[] = {
                MAKE_FOURC_TYPE('f', 't', 'y', 'p'), 
                MAKE_FOURC_TYPE('m', 'o', 'o', 'v'), 
                MAKE_FOURC_TYPE('m', 'd', 'a', 't'), 
            };

            std::ios::pos_type old_pos = is.tellg();
            assert(is);
            boost::uint64_t head_len = 0;
            boost::uint32_t find_flag = 0;
            while (true) {
                //if(total_len < head_len + 8) {
                //    head_len = head_len + 8;
                //    ec = error::file_stream_error;
                //    break;
                //}
                union {
                    boost::uint8_t c[8];
                    boost::uint32_t n[2];
                };
                is.seekg(head_len, std::ios_base::beg);
                is.read(c, 8);
                if (!is) {
                    is.clear();
                    assert(is);
                    head_len += 8;
                    ec = error::file_stream_error;
                    break;
                }
                for (size_t i = 0; i < sizeof(fourccs) / sizeof(fourccs[0]); ++i) {
                    if(0 == memcmp(fourccs + i, c + 4, 4)) {
                        find_flag |= 1 << i;
                        break;
                    }
                }
                n[0] = framework::system::BytesOrder::host_to_net_long(n[0]);
                if (n[0] < 8) {
                    head_len = 0; // invalid avi file
                    ec = error::bad_media_format;
                    break;
                }
                if (find_flag == (1 << sizeof(fourccs) / sizeof(fourccs[0])) - 1) {
                    ec.clear();
                    if(0 == memcmp("mdat", c + 4, 4)) {
                        head_len += 8;
                    } else {
                        head_len += n[0];
                        is.seekg(head_len, std::ios_base::beg);
                        if (!is) {
                            is.clear();
                            assert(is);
                            ec = error::file_stream_error;
                        }
                    }
                    break;
                }
                head_len += n[0];
            }
            is.seekg(old_pos, std::ios::beg);
            assert(is);
            return head_len;
        }

    } // namespace avformat
} // namespace just
