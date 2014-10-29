// AviHeaderList.h

#ifndef _PPBOX_AVFORMAT_AVI_LIB_AVI_HEADER_LIST_H_
#define _PPBOX_AVFORMAT_AVI_LIB_AVI_HEADER_LIST_H_

#include "ppbox/avformat/avi/box/AviHeaderBox.h"
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

            AviHeaderList(
                AviBox & box, 
                AviIndex & index, 
                create_new_t);

        public:
            AviStream * create_stream(
                boost::uint32_t type);

            bool fixup(
                boost::system::error_code & ec);

        public:
            bool merge_begin(
                boost::system::error_code & ec);

            bool merge(
                AviHeaderList & hdrl, 
                boost::system::error_code & ec);

            bool merge_end(
                boost::system::error_code & ec);

        public:
            AviHeaderBox * header()
            {
                return avih_;
            }

            std::vector<AviStream *> & streams()
            {
                return streams_;
            }

        private:
            AviHeaderBox * avih_;
            std::vector<AviStream *> streams_;
            AviIndex * index_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_LIB_AVI_HEADER_LIST_H_
