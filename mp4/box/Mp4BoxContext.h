// Mp4BoxContext.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_CONTEXT_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_CONTEXT_H_

#include "ppbox/avformat/mp4/box/Mp4Box.h"

namespace ppbox
{
    namespace avformat
    {

        struct Mp4BoxContext
        {
            std::string path;
            std::vector<Mp4Box *> stack;
            std::vector<std::streampos> data_ends;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_CONTEXT_H_
