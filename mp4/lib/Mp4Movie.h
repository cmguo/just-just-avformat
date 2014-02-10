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

            Mp4Movie(
                Mp4Box & box, 
                create_new_t);

            ~Mp4Movie();

        public:
            Mp4Track * create_track(
                boost::uint32_t type);

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
                return mvhd_->timescale;
            }

            boost::uint64_t duration() const
            {
                return mvhd_->duration;
            }

            void time_scale(
                boost::uint32_t n);

            void duration(
                boost::uint64_t n);

        private:
            Mp4MovieHeaderBox * mvhd_;
            std::vector<Mp4Track *> tracks_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_LIB_MP4_MOVIE_H_
