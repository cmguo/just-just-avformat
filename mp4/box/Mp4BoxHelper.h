// Mp4BoxHelper.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_HELPER_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_HELPER_H_

#include "ppbox/avformat/mp4/box/Mp4BoxTraits.h"

#include <ppbox/avbase/object/ObjectHelper.h>
#include <ppbox/avbase/FourCC.h>

namespace ppbox
{
    namespace avformat
    {

        class Mp4BoxDataHelper
            : ppbox::avbase::ObjectDataHelper<Mp4BoxTraits>
        {
        public:
            Mp4BoxDataHelper(
                Mp4BoxTraits::helper_t & h)
                : ppbox::avbase::ObjectDataHelper<Mp4BoxTraits>(h)
                , ctx_((Mp4BoxContext *)h.context())
            {
                ctx_->path.append(1, '/');
                ctx_->path += ppbox::avbase::FourCC::to_string(h.header().type);
                ctx_->stack.push_back(static_cast<Mp4Box *>(&h.header()));
                ctx_->data_ends.push_back(h.data_end());
            }

            ~Mp4BoxDataHelper()
            {
                ctx_->data_ends.pop_back();
                ctx_->stack.pop_back();
                ctx_->path.erase(ctx_->path.size() - 5);
            }

        private:
            Mp4BoxContext * ctx_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_HELPER_H_
