// AviHeaderList.h

#ifndef _PPBOX_AVFORMAT_AVI_LIB_AVI_HEADER_LIST_H_
#define _PPBOX_AVFORMAT_AVI_LIB_AVI_HEADER_LIST_H_

#include "ppbox/avformat/avi/lib/AviHeader.h"
#include "ppbox/avformat/avi/lib/AviStream.h"
#include "ppbox/avformat/avi/lib/AviBoxWrapper.h"

namespace ppbox
{
    namespace avformat
    {

        class AviHeaderList
            : public AviBoxWrapper2<AviHeaderListBox>
        {
        public:
            AviHeaderList(
                AviBox & box, 
                AviIndex & index);

        public:
            bool merge_begin(
                boost::system::error_code & ec);

            bool merge(
                AviHeaderList & hdrl, 
                boost::system::error_code & ec);

            bool merge_end(
                boost::system::error_code & ec);

        public:
            AviHeader * header()
            {
                return avih_;
            }

            std::vector<AviStream *> & streams()
            {
                return streams_;
            }

        private:
            AviHeader * avih_;
            std::vector<AviStream *> streams_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_LIB_AVI_HEADER_LIST_H_
