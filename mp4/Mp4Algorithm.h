// Mp4Algorithm.h

#ifndef _PPBOX_AVFORMAT_MP4_MP4_ALGORITHM_H_
#define _PPBOX_AVFORMAT_MP4_MP4_ALGORITHM_H_

#include <istream>

namespace ppbox
{
    namespace avformat
    {

        size_t mp4_head_size(
            std::istream & is);

    } // namespace avformat
} // namespace ppbox

#endif // End _PPBOX_AVFORMAT_MP4_MP4_ALGORITHM_H_
