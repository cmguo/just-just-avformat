// Mp4Movie.h

#ifndef _PPBOX_AVFORMAT_MP4_LIB_MP4_MOVIE_H_
#define _PPBOX_AVFORMAT_MP4_LIB_MP4_MOVIE_H_

#include "ppbox/avformat/mp4/lib/Mp4BoxWrapper.h"
#include "ppbox/avformat/mp4/box/Mp4MovieBox.h"
#include "ppbox/avformat/mp4/lib/Mp4Track.h"

namespace ppbox
{
    namespace avformat
    {

        class Mp4Movie
            : public Mp4BoxWrapper2<Mp4MovieBox>
        {
        public:
            Mp4Movie(
                Mp4Box & box);

            ~Mp4Movie();

        public:
            bool merge(
                Mp4Movie const & movie, 
                boost::system::error_code & ec);

            void shift(
                boost::int64_t offset);

        public:
            std::vector<Mp4Track *> & tracks()
            {
                return tracks_;
            }

            boost::uint32_t time_scale() const
            {
                return header_->timescale;
            }

            boost::uint64_t duration() const
            {
                return header_->duration;
            }

        private:
            Mp4MovieHeaderBox * header_;
            std::vector<Mp4Track *> tracks_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_LIB_MP4_MOVIE_H_
