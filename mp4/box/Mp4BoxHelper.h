// Mp4BoxHelper.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_HELPER_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_HELPER_H_

#include "ppbox/avformat/mp4/box/Mp4BoxTraits.h"

#include <ppbox/avbase/object/ObjectHelper.h>
#include <ppbox/avbase/FourCC.h>

#include <framework/logger/Logger.h>
#include <framework/logger/StreamRecord.h>
#include <framework/logger/DataRecord.h>

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
                ctx_->stack.push_back(&h.object());
                ctx_->data_ends.push_back(h.data_end());
            }

            ~Mp4BoxDataHelper()
            {
                if (h_.object().empty()) {
                    LOG_DEBUG("unknown box type: " << ctx_->path);
                    Mp4Box::raw_data_t raw_data(h_.object().raw_data());
                    LOG_DATA(framework::logger::Trace, ("raw data", raw_data.address(), raw_data.size()));
                }
                ctx_->data_ends.pop_back();
                ctx_->stack.pop_back();
                ctx_->path.erase(ctx_->path.size() - 5);
            }

        private:
            FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("ppbox.avformat.Mp4BoxDataHelper", framework::logger::Debug);

        private:
            Mp4BoxContext * ctx_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_HELPER_H_
