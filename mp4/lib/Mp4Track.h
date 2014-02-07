// Mp4Track.h

#ifndef _PPBOX_AVFORMAT_MP4_LIB_MP4_TRACK_H_
#define _PPBOX_AVFORMAT_MP4_LIB_MP4_TRACK_H_

#include "ppbox/avformat/mp4/box/Mp4Box.h"
#include "ppbox/avformat/mp4/box/Mp4BoxEnum.h"
#include "ppbox/avformat/mp4/box/Mp4TrackBox.h"
#include "ppbox/avformat/mp4/box/Mp4MediaBox.h"
#include "ppbox/avformat/mp4/lib/Mp4SampleTable.h"

namespace ppbox
{
    namespace avformat
    {

        class Mp4Track
        {
        public:
            Mp4Track(
                Mp4Box & box);

        public:
            bool merge(
                Mp4Track const & track, 
                boost::system::error_code & ec);

            void shift(
                boost::int64_t offset);

        public:
            boost::uint32_t type() const;

            boost::uint32_t timescale() const;

            boost::uint64_t duration() const;

            Mp4SampleTable & sample_table()
            {
                return table_;
            }

        private:
            Mp4Box & box_;
            Mp4TrackHeaderBox * tkhd_;
            Mp4MediaHeaderBox * mdhd_;
            Mp4HandlerBox * hdlr_;
            Mp4SampleTable table_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_LIB_MP4_TRACK_H_
