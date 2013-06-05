// MkvFormat.h

#ifndef _PPBOX_AVFORMAT_MKV_MKV_FORMAT_H_
#define _PPBOX_AVFORMAT_MKV_MKV_FORMAT_H_

#include "ppbox/avformat/Format.h"

namespace ppbox
{
    namespace avformat
    {

        class MkvFormat
            : public Format
        {
        public:
            MkvFormat();

        public:
            static boost::uint32_t stream_type(
                std::string const & type_str);

            static char const * stream_type_str(
                boost::uint32_t type);

        private:
            static CodecInfo const codecs_[];
            static char const * const type_strs[];
        };

        PPBOX_REGISTER_FORMAT("mkv", MkvFormat);

    } // namespace avformat
} // namespace ppbox


#endif // _PPBOX_AVFORMAT_MKV_MKV_FORMAT_H_
