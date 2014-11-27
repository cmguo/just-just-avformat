// Mp4Movie.cpp

#include "just/avformat/Common.h"
#include "just/avformat/mp4/lib/Mp4Movie.h"
#include "just/avformat/mp4/box/Mp4Box.hpp"
#include "just/avformat/mp4/box/Mp4BoxVector.hpp"

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/date.hpp>

namespace just
{
    namespace avformat
    {

        Mp4Movie::Mp4Movie(
            Mp4Box & box)
            : Mp4BoxWrapper2<Mp4MovieBox>(box)
            , mvhd_(NULL)
        {
            mvhd_ = find_item_as<Mp4MovieHeaderBox>("/mvhd");
            Mp4MovieBox & moov(data());
            for (Mp4BoxVector::iterator iter = moov.begin(); iter != moov.end(); ++iter) {
                if ((*iter)->type == Mp4BoxType::trak) {
                    Mp4Track * track = new Mp4Track(**iter);
                    tracks_.push_back(track);
                }
            }
        }

        Mp4Movie::Mp4Movie(
            Mp4Box & box, 
            create_new_t)
            : Mp4BoxWrapper2<Mp4MovieBox>(box)
            , mvhd_(NULL)
        {
            mvhd_ = create_item_as<Mp4MovieHeaderBox>("/mvhd");
            boost::posix_time::ptime t_1904_01_01(boost::gregorian::date(1904, 1, 1));
            boost::posix_time::ptime now(boost::posix_time::second_clock::universal_time());
            mvhd_->creation_time = (now - t_1904_01_01).total_seconds();
            mvhd_->modification_time = mvhd_->creation_time;
            mvhd_->rate = 0x00010000;
            mvhd_->volume = 0x0100;
            mvhd_->next_track_ID = 1;
        }

        Mp4Movie::~Mp4Movie()
        {
            for (size_t i = 0; i < tracks_.size(); ++i) {
                delete tracks_[i];
            }
            tracks_.clear();
            mvhd_ = NULL;
        }

        Mp4Track * Mp4Movie::create_track(
            boost::uint32_t type)
        {
            Mp4Track * track = new Mp4Track(*create_item("/trak"), mvhd_->next_track_ID++, type);
            tracks_.push_back(track);
            return track;
        }

        bool Mp4Movie::fixup(
            boost::system::error_code & ec)
        {
            for (size_t i = 0; i < tracks_.size(); ++i) {
                if (!tracks_[i]->fixup(mvhd_->timescale, ec))
                    return false;
            }
            if (mvhd_->duration == 0) {
                for (size_t i = 0; i < tracks_.size(); ++i) {
                    boost::uint64_t d = tracks_[i]->track_duration();
                    if (mvhd_->duration < d)
                        mvhd_->duration = d;
                }
            }
            return true;
        }

        bool Mp4Movie::merge(
            Mp4Movie const & movie, 
            boost::system::error_code & ec)
        {
            mvhd_->duration += movie.mvhd_->duration;
            for (size_t i = 0; i < tracks_.size(); ++i) {
                if (!tracks_[i]->merge(*movie.tracks_[i], ec))
                    return false;
            }
            return true;
        }

        void Mp4Movie::shift(
            boost::int64_t offset)
        {
            for (size_t i = 0; i < tracks_.size(); ++i) {
                tracks_[i]->shift(offset);
            }
        }

        void Mp4Movie::time_scale(
            boost::uint32_t n)
        {
            mvhd_->timescale = n;
        }

        void Mp4Movie::duration(
            boost::uint64_t n)
        {
            mvhd_->duration = n;
        }


    } // namespace avformat
} // namespace just
