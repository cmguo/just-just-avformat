// Mp4Track.cpp

#include "just/avformat/Common.h"
#include "just/avformat/mp4/lib/Mp4Track.h"
#include "just/avformat/mp4/box/Mp4Box.hpp"
#include "just/avformat/mp4/box/Mp4BoxVector.hpp"

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/date.hpp>

namespace just
{
    namespace avformat
    {

        Mp4Track::Mp4Track(
            Mp4Box & box)
            : Mp4BoxWrapper2<Mp4TrackBox>(box)
            , tkhd_(find_item_as<Mp4TrackHeaderBox>("/tkhd"))
            , mdhd_(find_item_as<Mp4MediaHeaderBox>("/mdia/mdhd"))
            , hdlr_(find_item_as<Mp4HandlerBox>("/mdia/hdlr"))
            , vmhd_(find_item_as<Mp4VideoMediaHeaderBox>("/mdia/minf/vmhd"))
            , smhd_(find_item_as<Mp4SoundMediaHeaderBox>("/mdia/minf/smhd"))
            , table_(*find_item("/mdia/minf/stbl"))
        {
            find_item_as<Mp4MediaBox>("/mdia");
            find_item_as<Mp4MediaInformationBox>("/mdia/minf");
        }

        Mp4Track::Mp4Track(
            Mp4Box & box, 
            boost::uint32_t id, 
            boost::uint32_t type)
            : Mp4BoxWrapper2<Mp4TrackBox>(box)
            , tkhd_(create_item_as<Mp4TrackHeaderBox>("/tkhd"))
            , mdhd_(create_item_as<Mp4MediaHeaderBox>("/mdia/mdhd"))
            , hdlr_(create_item_as<Mp4HandlerBox>("/mdia/hdlr"))
            , vmhd_(type == Mp4HandlerType::vide ? create_item_as<Mp4VideoMediaHeaderBox>("/mdia/minf/vmhd") : NULL)
            , smhd_(type == Mp4HandlerType::soun ? create_item_as<Mp4SoundMediaHeaderBox>("/mdia/minf/smhd") : NULL)
            , table_(*create_item("/mdia/minf/stbl"), create_new_t())
        {
            boost::posix_time::ptime t_1904_01_01(boost::gregorian::date(1904, 1, 1));
            boost::posix_time::ptime now(boost::posix_time::second_clock::universal_time());
            tkhd_->flags = 1; // ENABLED
            tkhd_->creation_time = (now - t_1904_01_01).total_seconds();
            tkhd_->modification_time = tkhd_->creation_time;
            tkhd_->track_ID = id;
            hdlr_->handler_type = type;
            if (type == Mp4HandlerType::vide) {
                boost::int32_t matrix[9] = { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 };
                memcpy(tkhd_->matrix, matrix, sizeof(matrix));
                hdlr_->name = "GPAC ISO Video Handler";
            } else if (type == Mp4HandlerType::soun) {
                tkhd_->volume = 0x0100;
                hdlr_->name = "GPAC ISO Audio Handler";
            }
            // media
            mdhd_->creation_time = tkhd_->creation_time;
            mdhd_->modification_time = mdhd_->creation_time;
            mdhd_->language[0] = 'u'; // "und"
            mdhd_->language[1] = 'n';
            mdhd_->language[2] = 'd';
            if (vmhd_)
                vmhd_->flags = 1;
        }

        bool Mp4Track::fixup(
            boost::uint32_t timescale, 
            boost::system::error_code & ec)
        {
            if (mdhd_->duration == 0)
                mdhd_->duration = table_.duration();
            tkhd_->duration = mdhd_->duration * timescale / mdhd_->timescale;
            return true;
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

        boost::uint64_t Mp4Track::track_duration() const
        {
            return tkhd_ ? tkhd_->duration: 0;
        }

        boost::uint32_t Mp4Track::width() const
        {
            return tkhd_ ? (tkhd_->width >> 16) : 0;
        }

        boost::uint32_t Mp4Track::height() const
        {
            return tkhd_ ? (tkhd_->height >> 16) : 0;
        }

        boost::uint32_t Mp4Track::timescale() const
        {
            return mdhd_ ? mdhd_->timescale : 0;
        }

        boost::uint64_t Mp4Track::duration() const
        {
            return mdhd_ ? mdhd_->duration : 0;
        }

        boost::uint32_t Mp4Track::type() const
        {
            return hdlr_ ? hdlr_->handler_type : 0;
        }

        void Mp4Track::width(
            boost::uint32_t n)
        {
            tkhd_->width = n << 16;
        }

        void Mp4Track::height(
            boost::uint32_t n)
        {
            tkhd_->height = n << 16;
        }

        void Mp4Track::timescale(
            boost::uint32_t n)
        {
            mdhd_->timescale = n;
        }

        void Mp4Track::duration(
            boost::uint64_t n)
        {
            mdhd_->duration = n;
        }

    } // namespace avformat
} // namespace just
