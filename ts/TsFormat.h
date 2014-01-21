// TsFormat.h

#ifndef _PPBOX_AVFORMAT_TS_TS_FORMAT_H_
#define _PPBOX_AVFORMAT_TS_TS_FORMAT_H_

#include "ppbox/avformat/Format.h"

namespace ppbox
{
    namespace avformat
    {

		struct TsContext
		{
			boost::uint8_t hdmv_type;
			boost::uint8_t misc_type;
			boost::uint32_t regd_type;
		};

        class TsFormat
            : public Format
        {
        public:
            TsFormat();

        public:
            virtual  CodecInfo const * codec_from_stream(
                boost::uint32_t category, 
                boost::uint32_t stream_type, 
				void const * context, 
                boost::system::error_code & ec);

        private:
            static CodecInfo const codecs_[];
        };

        PPBOX_REGISTER_FORMAT("ts", TsFormat);

    } // namespace avformat
} // namespace ppbox


#endif // _PPBOX_AVFORMAT_TS_TS_FORMAT_H_
