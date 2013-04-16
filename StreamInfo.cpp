// Sample.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/StreamInfo.h"
#include "ppbox/avformat/Format.h"

namespace ppbox
{
    namespace avformat
    {

        StreamInfoBase::StreamInfoBase()
            : type(MEDIA_TYPE_NONE)
            , sub_type(VIDEO_TYPE_NONE)
            , time_scale(0)
            , start_time(0)
            , duration(0)
        {
        }

        StreamInfo::StreamInfo()
            : format_type(FormatType::none)
        {
        }

    } // namespace avformat
} // namespace ppbox
