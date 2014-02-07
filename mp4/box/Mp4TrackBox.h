// Mp4TrackBox.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_TRACK_BOX_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_TRACK_BOX_H_

#include "ppbox/avformat/mp4/box/Mp4BoxEnum.h"
#include "ppbox/avformat/mp4/box/Mp4FullBox.h"

namespace ppbox
{
    namespace avformat
    {

        struct Mp4TrackBox
            : Mp4BoxData<Mp4TrackBox, Mp4BoxType::trak>
            , Mp4BoxContainer<Mp4TrackBox, Mp4BoxType::trak>
        {
        };

        struct Mp4TrackHeaderBox
            : Mp4FullBoxData<Mp4TrackHeaderBox, Mp4BoxType::tkhd>
        {
            boost::uint64_t creation_time;
            boost::uint64_t modification_time;
            boost::uint32_t track_ID;
            boost::uint32_t reserved; // = 0;
            boost::uint64_t duration;
            boost::uint32_t reserved2[2]; // = 0;
            boost::int16_t layer; // = 0;
            boost::int16_t alternate_group; // = 0;
            boost::int16_t volume; // = {if track_is_audio 0x0100 else 0};
            boost::uint16_t reserved3; // = 0;
            boost::int32_t matrix[9]; // = { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 };
            // unity matrix
            boost::uint32_t width;
            boost::uint32_t height;

            Mp4TrackHeaderBox()
                : creation_time(0)
                , modification_time(0)
                , track_ID(0)
                , reserved(0)
                , duration(0)
                , layer(0)
                , alternate_group(0)
                , volume(0)
                , reserved3(0)
                , width(0)
                , height(0)
            {
                reserved2[0] = reserved2[1] = 0;
                for (int i = 0; i < 9; ++i) {
                    matrix[i] = 0;
                }
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4FullBoxHeader::serialize(ar);
                if (version == 1) {
                    ar & creation_time
                        & modification_time
                        & track_ID
                        & reserved
                        & duration;
                } else {
                    boost::uint32_t creation_time32 = (boost::uint32_t)creation_time;
                    boost::uint32_t modification_time32 = (boost::uint32_t)modification_time;
                    boost::uint32_t duration32 = (boost::uint32_t)duration;
                    ar & creation_time32
                        & modification_time32
                        & track_ID
                        & reserved
                        & duration32;
                    creation_time = creation_time32;
                    modification_time = modification_time32;
                    duration = duration32;
                }
                ar & make_array(reserved2);
                ar & layer
                    & alternate_group
                    & volume
                    & reserved3;
                ar & make_array(matrix);
                ar & width
                    & height;
            }
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_TRACK_BOX_H_
