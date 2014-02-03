// Mp4File.h

#ifndef _PPBOX_AVFORMAT_MP4_LIB_MP4_FILE_H_
#define _PPBOX_AVFORMAT_MP4_LIB_MP4_FILE_H_

#include "ppbox/avformat/mp4/box/Mp4Box.h"
#include "ppbox/avformat/mp4/box/Mp4FileBox.h"
#include "ppbox/avformat/mp4/lib/Mp4Movie.h"

namespace ppbox
{
    namespace avformat
    {

        class Mp4File
        {
        public:
            Mp4Box ftyp;
            Mp4Box moov;
            Mp4Box mdat;

        public:
            Mp4File();

        public:
            bool parse(
                boost::system::error_code & ec);

        public:
            Mp4Movie & movie()
            {
                return movie_;
            }

            Mp4Movie const & movie() const
            {
                return movie_;
            }

        private:
            Mp4Movie movie_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_LIB_MP4_FILE_H_
