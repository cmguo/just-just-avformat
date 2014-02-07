// Mp4Movie.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/lib/Mp4Movie.h"
#include "ppbox/avformat/mp4/box/Mp4Box.hpp"

namespace ppbox
{
    namespace avformat
    {

        Mp4Movie::Mp4Movie(
            Mp4Box & box)
            : Mp4BoxWrapper2<Mp4MovieBox>(box)
            , header_(NULL)
        {
            header_ = find_item_as<Mp4MovieHeaderBox>("/mvhd");
            Mp4MovieBox & moov(data());
            for (Mp4BoxVector::iterator iter = moov.begin(); iter != moov.end(); ++iter) {
                if (iter->type == Mp4BoxType::trak) {
                    Mp4Track * track = new Mp4Track(*iter);
                    tracks_.push_back(track);
                }
            }
        }

        static void delete_track(
            Mp4Track * track)
        {
            delete track;
        }

        Mp4Movie::~Mp4Movie()
        {
            std::for_each(tracks_.begin(), tracks_.end(), delete_track);
            tracks_.clear();
            header_ = NULL;
        }

        bool Mp4Movie::merge(
            Mp4Movie const & movie, 
            boost::system::error_code & ec)
        {
            header_->duration += movie.header_->duration;
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

    } // namespace avformat
} // namespace ppbox
