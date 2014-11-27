// AviBoxHelper.h

#ifndef _JUST_AVFORMAT_AVI_BOX_AVI_BOX_HELPER_H_
#define _JUST_AVFORMAT_AVI_BOX_AVI_BOX_HELPER_H_

#include "just/avformat/avi/box/AviBoxTraits.h"

#include <just/avbase/object/ObjectHelper.h>
#include <just/avbase/FourCC.h>

#include <framework/logger/Logger.h>
#include <framework/logger/StreamRecord.h>
#include <framework/logger/DataRecord.h>

namespace just
{
    namespace avformat
    {

        class AviBoxDataHelper
            : just::avbase::ObjectDataHelper<AviBoxTraits>
        {
        public:
            AviBoxDataHelper(
                AviBoxTraits::helper_t & h)
                : just::avbase::ObjectDataHelper<AviBoxTraits>(h)
                , ctx_((AviBoxContext *)h.context())
            {
                ctx_->path.append(1, '/');
                ctx_->path += just::avbase::FourCC::to_string(h.header().id());
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
            FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("just.avformat.AviBoxDataHelper", framework::logger::Debug);

        private:
            AviBoxContext * ctx_;
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_AVI_BOX_AVI_BOX_HELPER_H_
