// Mp4Algorithm.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/Mp4Algorithm.h"

#include <framework/system/BytesOrder.h>

namespace ppbox
{
    namespace avformat
    {

        size_t mp4_head_size(
            std::istream & is)
        {
            std::ios::pos_type old_pos = is.tellg();
            is.seekg(0, std::ios_base::end);
            size_t total_len = is.tellg();
            size_t head_len = 0;
            while (true) {
                if(total_len < head_len + 8) {
                    head_len = head_len + 8;
                    break;
                }
                union {
                    char c[8];
                    boost::uint32_t n[2];
                };
                is.seekg(head_len, std::ios_base::beg);
                is.read(c, 8);
                if (!is) {
                    head_len = 0; 
                    break;
                }
                if(0 == strncmp("mdat", c + 4, 4)) {
                    head_len += 8;
                    break;
                }
                n[0] = framework::system::BytesOrder::host_to_net_long(n[0]);
                if (n[0] < 8) {
                    head_len = 0; // invalid mp4 file
                    break;
                }
                head_len += n[0];
            }
            is.seekg(old_pos, std::ios::beg);
            return head_len;
        }

    } // namespace avformat
} // namespace ppbox
