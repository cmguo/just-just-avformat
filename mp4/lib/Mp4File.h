// Mp4File.h

#ifndef _JUST_AVFORMAT_MP4_LIB_MP4_FILE_H_
#define _JUST_AVFORMAT_MP4_LIB_MP4_FILE_H_

#include "just/avformat/mp4/box/Mp4Box.h"
#include "just/avformat/mp4/lib/Mp4Movie.h"

namespace just
{
    namespace avformat
    {

        class Mp4File
        {
        public:
            Mp4File();

            ~Mp4File();

        public:
            bool open(
                Mp4Box * box, 
                boost::system::error_code & ec);

            bool create(
                boost::system::error_code & ec);

            bool fixup(
                boost::system::error_code & ec);

            void close();

        public:
            bool load(
                Mp4BoxIArchive & ia, 
                boost::system::error_code & ec);

            void save(
                Mp4BoxOArchive & oa);

        public:
            bool merge_begin(
                boost::system::error_code & ec);

            bool merge(
                Mp4File & file, 
                boost::system::error_code & ec);

            bool merge_end(
                boost::system::error_code & ec);

        public:
            std::vector<Mp4Box *> const boxes() const
            {
                return boxes_;
            }

            Mp4Movie & movie()
            {
                return *movie_;
            }

            Mp4Movie const & movie() const
            {
                return *movie_;
            }

            boost::uint64_t head_size() const;

        private:
            std::vector<Mp4Box *> boxes_;
            Mp4Box * ftyp_;
            Mp4Box * moov_;
            Mp4Box * mdat_;
            Mp4Movie * movie_;
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MP4_LIB_MP4_FILE_H_
