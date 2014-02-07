// Mp4MovieBox.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_MOVIE_BOX_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_MOVIE_BOX_H_

#include "ppbox/avformat/mp4/box/Mp4BoxEnum.h"
#include "ppbox/avformat/mp4/box/Mp4FullBox.h"
#include "ppbox/avformat/mp4/box/Mp4BoxVector.h"

namespace ppbox
{
    namespace avformat
    {

        struct Mp4MovieBox
            : Mp4BoxData<Mp4MovieBox, Mp4BoxType::moov>
            , Mp4BoxContainer<Mp4MovieBox, Mp4BoxType::moov>
        {
            static std::string name;
        };

        struct Mp4MovieHeaderBox
            : Mp4FullBoxData<Mp4MovieHeaderBox, Mp4BoxType::mvhd>
        {
            boost::uint64_t creation_time;
            boost::uint64_t modification_time;
            boost::uint32_t timescale;
            boost::uint64_t duration;
            boost::int32_t rate; // = 0x00010000; // typically 1.0
            boost::int16_t volume; // = 0x0100; // typically, full volume
            boost::int16_t reserved; // = 0;
            boost::uint32_t reserved2[2]; // = 0;
            boost::int32_t matrix[9]; // = { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 };
            // Unity matrix
            boost::uint32_t pre_defined[6]; // = 0;
            boost::uint32_t next_track_ID;

            Mp4MovieHeaderBox()
                : creation_time(0)
                , modification_time(0)
                , timescale(0)
                , duration(0)
                , rate(0)
                , volume(0)
                , reserved(0)
                , next_track_ID(0)
            {
                reserved2[0] = reserved2[1] = 0;
                for (int i = 0; i < 9; ++i) {
                    matrix[i] = 0;
                }
                for (int i = 0; i < 6; ++i) {
                    pre_defined[i] = 0;
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
                        & timescale
                        & duration;
                } else {
                    boost::uint32_t creation_time32 = (boost::uint32_t)creation_time;
                    boost::uint32_t modification_time32 = (boost::uint32_t)modification_time;
                    boost::uint32_t duration32 = (boost::uint32_t)duration;
                    ar & creation_time32
                        & modification_time32
                        & timescale
                        & duration32;
                    creation_time = creation_time32;
                    modification_time = modification_time32;
                    duration = duration32;
                }
                ar & rate
                    & volume
                    & reserved;
                ar & make_array(reserved2);
                ar & make_array(matrix);
                ar & make_array(pre_defined);
                ar & next_track_ID;
            }
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_MOVIE_BOX_H_
