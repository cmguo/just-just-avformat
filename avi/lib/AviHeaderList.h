// AviHeaderList.h

#ifndef _JUST_AVFORMAT_AVI_LIB_AVI_HEADER_LIST_H_
#define _JUST_AVFORMAT_AVI_LIB_AVI_HEADER_LIST_H_

#include "just/avformat/avi/box/AviHeaderBox.h"
#include "just/avformat/avi/lib/AviStream.h"
#include "just/avformat/avi/lib/AviBoxWrapper.h"

namespace just
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
} // namespace just

#endif // _JUST_AVFORMAT_AVI_LIB_AVI_HEADER_LIST_H_
