// Mp4Movie.h

#ifndef _JUST_AVFORMAT_MP4_LIB_MP4_MOVIE_H_
#define _JUST_AVFORMAT_MP4_LIB_MP4_MOVIE_H_

#include "just/avformat/mp4/lib/Mp4BoxWrapper.h"
#include "just/avformat/mp4/box/Mp4MovieBox.h"
#include "just/avformat/mp4/lib/Mp4Track.h"

namespace just
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

            bool fixup(
                boost::system::error_code & ec);

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
} // namespace just

#endif // _JUST_AVFORMAT_MP4_LIB_MP4_MOVIE_H_
