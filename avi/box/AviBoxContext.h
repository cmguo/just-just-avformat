// AviBoxContext.h

#ifndef _JUST_AVFORMAT_AVI_BOX_AVI_BOX_CONTEXT_H_
#define _JUST_AVFORMAT_AVI_BOX_AVI_BOX_CONTEXT_H_

#include "just/avformat/avi/box/AviBox.h"

namespace just
{
    namespace avformat
    {

        struct AviBoxContext
        {
            std::string path;
            std::vector<AviBox *> stack;
            std::vector<std::streampos> data_ends;
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_AVI_BOX_AVI_BOX_CONTEXT_H_
