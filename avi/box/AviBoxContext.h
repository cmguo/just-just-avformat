// AviBoxContext.h

#ifndef _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_CONTEXT_H_
#define _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_CONTEXT_H_

#include "ppbox/avformat/avi/box/AviBox.h"

namespace ppbox
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
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_CONTEXT_H_
