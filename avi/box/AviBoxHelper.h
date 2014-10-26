// AviBoxHelper.h

#ifndef _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_HELPER_H_
#define _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_HELPER_H_

#include "ppbox/avformat/avi/box/AviBoxTraits.h"

#include <ppbox/avbase/object/ObjectHelper.h>
#include <ppbox/avbase/FourCC.h>

#include <framework/logger/Logger.h>
#include <framework/logger/StreamRecord.h>
#include <framework/logger/DataRecord.h>

namespace ppbox
{
    namespace avformat
    {

        class AviBoxDataHelper
            : ppbox::avbase::ObjectDataHelper<AviBoxTraits>
        {
        public:
            AviBoxDataHelper(
                AviBoxTraits::helper_t & h)
                : ppbox::avbase::ObjectDataHelper<AviBoxTraits>(h)
                , ctx_((AviBoxContext *)h.context())
            {
                ctx_->path.append(1, '/');
                ctx_->path += ppbox::avbase::FourCC::to_string(h.header().id());
                ctx_->stack.push_back(&h.object());
                ctx_->data_ends.push_back(h.data_end());
            }

            ~AviBoxDataHelper()
            {
                if (h_.object().empty()) {
                    LOG_DEBUG("unknown box type: " << ctx_->path);
                    AviBox::raw_data_t raw_data(h_.object().raw_data());
                    LOG_DATA(framework::logger::Trace, ("raw data", raw_data.address(), raw_data.size()));
                }
                ctx_->data_ends.pop_back();
                ctx_->stack.pop_back();
                ctx_->path.erase(ctx_->path.size() - 5);
            }

        private:
            FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("ppbox.avformat.AviBoxDataHelper", framework::logger::Debug);

        private:
            AviBoxContext * ctx_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_HELPER_H_
