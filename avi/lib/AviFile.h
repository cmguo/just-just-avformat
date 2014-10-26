// AviFile.h

#ifndef _PPBOX_AVFORMAT_AVI_LIB_AVI_FILE_H_
#define _PPBOX_AVFORMAT_AVI_LIB_AVI_FILE_H_

#include "ppbox/avformat/avi/box/AviFileBox.h"
#include "ppbox/avformat/avi/lib/AviHeaderList.h"
#include "ppbox/avformat/avi/lib/AviIndex.h"
#include "ppbox/avformat/avi/lib/AviBoxWrapper.h"

namespace ppbox
{
    namespace avformat
    {

        class AviFile
            : public AviBoxWrapper2<AviFileBox>
        {
        public:
            AviFile();

            ~AviFile();

        public:
            bool open(
                AviBox * box, 
                boost::system::error_code & ec);

            bool create(
                boost::system::error_code & ec);

            void close();

        public:
            bool load(
                AviBoxIArchive & ia, 
                boost::system::error_code & ec);

            void save(
                AviBoxOArchive & oa);

        public:
            bool merge_begin(
                boost::system::error_code & ec);

            bool merge(
                AviFile & file, 
                boost::system::error_code & ec);

            bool merge_end(
                boost::system::error_code & ec);

        public:
            AviHeaderList * header_list()
            {
                return hdrl_;
            }

        private:
            AviHeaderList * hdrl_;
            AviBox * movi_;
            AviIndex * idx1_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_LIB_AVI_FILE_H_
