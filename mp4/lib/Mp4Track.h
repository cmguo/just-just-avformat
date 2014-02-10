// Mp4Track.h

#ifndef _PPBOX_AVFORMAT_MP4_LIB_MP4_TRACK_H_
#define _PPBOX_AVFORMAT_MP4_LIB_MP4_TRACK_H_

#include "ppbox/avformat/mp4/lib/Mp4BoxWrapper.h"
#include "ppbox/avformat/mp4/lib/Mp4SampleTable.h"
#include "ppbox/avformat/mp4/box/Mp4TrackBox.h"
#include "ppbox/avformat/mp4/box/Mp4MediaBox.h"

namespace ppbox
{
    namespace avformat
    {

        class Mp4Track
            : public Mp4BoxWrapper2<Mp4TrackBox>
        {
        public:
            Mp4Track(
                Mp4Box & box);

            Mp4Track(
                Mp4Box & box, 
                boost::uint32_t id, 
                boost::uint32_t type); // create new

        public:
            bool merge(
                Mp4Track const & track, 
                boost::system::error_code & ec);

            void shift(
                boost::int64_t offset);

        public:
            boost::uint32_t width() const;

            boost::uint32_t height() const;

            boost::uint32_t timescale() const;

            boost::uint64_t duration() const;

            boost::uint32_t type() const;

            void width(
                boost::uint32_t n);

            void height(
                boost::uint32_t n);

            void timescale(
                boost::uint32_t n);

            void duration(
                boost::uint64_t n);

            Mp4SampleTable & sample_table()
            {
                return table_;
            }

        private:
            Mp4TrackHeaderBox * tkhd_;
            Mp4MediaHeaderBox * mdhd_;
            Mp4HandlerBox * hdlr_;
            Mp4VideoMediaHeaderBox * vmhd_;
            Mp4SoundMediaHeaderBox * smhd_;
            Mp4SampleTable table_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_LIB_MP4_TRACK_H_
