// Mp4BoxContext.h

#ifndef _JUST_AVFORMAT_MP4_BOX_MP4_BOX_CONTEXT_H_
#define _JUST_AVFORMAT_MP4_BOX_MP4_BOX_CONTEXT_H_

#include "just/avformat/mp4/box/Mp4Box.h"

namespace just
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
} // namespace just

#endif // _JUST_AVFORMAT_MP4_BOX_MP4_BOX_CONTEXT_H_
