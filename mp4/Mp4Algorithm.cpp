// Mp4Algorithm.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/Mp4Algorithm.h"

#include "ppbox/demux/base/DemuxError.h"

#include <framework/system/BytesOrder.h>

namespace ppbox
{
    namespace avformat
    {

        boost::uint64_t mp4_head_size(
            std::basic_istream<boost::uint8_t> & is, 
            boost::system::error_code & ec)
        {
            std::ios::pos_type old_pos = is.tellg();
            is.seekg(0, std::ios_base::end);
            boost::uint64_t total_len = is.tellg();
            boost::uint64_t head_len = 0;
            while (true) {
                if(total_len < head_len + 8) {
                    head_len = head_len + 8;
                    ec = ppbox::demux::error::file_stream_error;
                    break;
                }
                union {
                    boost::uint8_t c[8];
                    boost::uint32_t n[2];
                };
                is.seekg(head_len, std::ios_base::beg);
                is.read(c, 8);
                if (!is) {
                    head_len = 0; 
                    ec = ppbox::demux::error::file_stream_error;
                    break;
                }
                if(0 == memcmp("mdat", c + 4, 4)) {
                    head_len += 8;
                    ec.clear();
                    break;
                }
                n[0] = framework::system::BytesOrder::host_to_net_long(n[0]);
                if (n[0] < 8) {
                    head_len = 0; // invalid mp4 file
                    ec = ppbox::demux::error::bad_file_format;
                    break;
                }
                head_len += n[0];
            }
            is.seekg(old_pos, std::ios::beg);
            return head_len;
        }

    } // namespace avformat
} // namespace ppbox
