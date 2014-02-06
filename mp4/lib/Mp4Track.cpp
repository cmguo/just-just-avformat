// Mp4Track.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/lib/Mp4Track.h"
#include "ppbox/avformat/mp4/box/Mp4Box.hpp"

namespace ppbox
{
    namespace avformat
    {

        Mp4Track::Mp4Track(
            Mp4Box & box)
            : box_(box)
            , tkhd_(Mp4BoxVector::find_item_as<Mp4TrackHeaderBox>(box, "/tkhd"))
            , mdhd_(Mp4BoxVector::find_item_as<Mp4MediaHeaderBox>(box, "/mdia/mdhd"))
            , hdlr_(Mp4BoxVector::find_item_as<Mp4HandlerBox>(box, "/mdia/hdlr"))
            , table_(*Mp4BoxVector::find_item(box, "/mdia/minf/stbl"))
        {
            box.as<Mp4TrackBox>();
            Mp4BoxVector::find_item_as<Mp4MediaBox>(box, "/mdia");
            Mp4BoxVector::find_item_as<Mp4MediaInformationBox>(box, "/mdia/minf");
            Mp4BoxVector::find_item_as<Mp4VideoMediaHeaderBox>(box, "/mdia/minf/vmhd");
            Mp4BoxVector::find_item_as<Mp4SoundMediaHeaderBox>(box, "/mdia/minf/smhd");
        }

        bool Mp4Track::merge(
            Mp4Track const & track, 
            boost::system::error_code & ec)
        {
            tkhd_->duration += track.tkhd_->duration;
            mdhd_->duration += track.mdhd_->duration;
            return table_.merge(track.table_, ec);
        }

        void Mp4Track::shift(
            boost::int64_t offset)
        {
            table_.shift(offset);
        }

        boost::uint32_t Mp4Track::type() const
        {
            return hdlr_ ? hdlr_->handler_type : 0;
        }

        boost::uint32_t Mp4Track::timescale() const
        {
            return mdhd_ ? mdhd_->timescale : 0;
        }

        boost::uint64_t Mp4Track::duration() const
        {
            return mdhd_ ? mdhd_->duration : 0;
        }

    } // namespace avformat
} // namespace ppbox
