// Mp4SampleBox.h

#ifndef _JUST_AVFORMAT_MP4_BOX_MP4_SAMPLE_BOX_H_
#define _JUST_AVFORMAT_MP4_BOX_MP4_SAMPLE_BOX_H_

#include "just/avformat/mp4/box/Mp4BoxEnum.h"
#include "just/avformat/mp4/box/Mp4FullBox.h"
#include "just/avformat/mp4/box/Mp4BoxVector.h"

namespace just
{
    namespace avformat
    {

        struct Mp4SampleTableBox
            : Mp4BoxData<Mp4SampleTableBox, Mp4BoxType::stbl>
            , Mp4BoxContainer<Mp4SampleTableBox, Mp4BoxType::stbl>
        {
        };

        struct Mp4TimeToSampleBox
            : Mp4FullBoxData<Mp4TimeToSampleBox, Mp4BoxType::stts>
        {
            //boost::uint32_t entry_count;
            //for (i=0; i < entry_count; i++) {
            //    boost::uint32_t sample_count;
            //    boost::uint32_t sample_delta;
            //}
            struct Entry
            {
                boost::uint32_t sample_count;
                boost::uint32_t sample_delta;
                template <typename Archive>
                void serialize(
                    Archive & ar)
                {
                    ar & sample_count 
                        & sample_delta;
                }
            };
            std::vector<Entry> table;

            Mp4TimeToSampleBox()
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4FullBoxHeader::serialize(ar);
                ar & util::serialization::make_sized<boost::uint32_t>(table);
            }
        };

        struct Mp4CompositionOffsetBox
            : Mp4FullBoxData<Mp4CompositionOffsetBox, Mp4BoxType::ctts>
        {
            //boost::uint32_t entry_count;
            //for (i=0; i < entry_count; i++) {
            //    boost::uint32_t sample_count;
            //    boost::uint32_t sample_offset;
            //}
            struct Entry
            {
                boost::uint32_t sample_count;
                boost::uint32_t sample_offset;
                template <typename Archive>
                void serialize(
                    Archive & ar)
                {
                    ar & sample_count 
                        & sample_offset;
                }
            };
            std::vector<Entry> table;

            Mp4CompositionOffsetBox()
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4FullBoxHeader::serialize(ar);
                ar & util::serialization::make_sized<boost::uint32_t>(table);
            }
        };

        struct Mp4SyncSampleBox
            : Mp4FullBoxData<Mp4SyncSampleBox, Mp4BoxType::stss>
        {
            //boost::uint32_t entry_count;
            //for (i=0; i < entry_count; i++) {
            //    boost::uint32_t sample_number;
            //}
            std::vector<boost::uint32_t> table;

            Mp4SyncSampleBox()
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4FullBoxHeader::serialize(ar);
                ar & util::serialization::make_sized<boost::uint32_t>(table);
            }
        };

        struct Mp4SampleSizeBox
            : Mp4FullBoxData<Mp4SampleSizeBox, Mp4BoxType::stsz>
        {
            boost::uint32_t sample_size;
            boost::uint32_t sample_count;
            //if (sample_size==0) {
                //for (i=0; i < sample_count; i++) {
                //    boost::uint32_t entry_size;
                //}
            //}
            std::vector<boost::uint32_t> table;

            Mp4SampleSizeBox()
                : sample_size(0)
                , sample_count(0)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4FullBoxHeader::serialize(ar);
                ar & sample_size;
                if (sample_size) {
                    ar & sample_count;
                } else {
                    ar & util::serialization::make_sized<boost::uint32_t>(table);
                }
            }
        };

        struct Mp4SampleToChunkBox
            : Mp4FullBoxData<Mp4SampleToChunkBox, Mp4BoxType::stsc>
        {
                //boost::uint32_t entry_count;
            //for (i=1; i <= entry_count; i++) {
            //    boost::uint32_t first_chunk;
            //    boost::uint32_t samples_per_chunk;
            //    boost::uint32_t sample_description_index;
            //}
            struct Entry
            {
                boost::uint32_t first_chunk;
                boost::uint32_t samples_per_chunk;
                boost::uint32_t sample_description_index;
                template <typename Archive>
                void serialize(
                    Archive & ar)
                {
                    ar & first_chunk 
                        & samples_per_chunk 
                        & sample_description_index;
                }
            };
            std::vector<Entry> table;

            Mp4SampleToChunkBox()
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4FullBoxHeader::serialize(ar);
                ar & util::serialization::make_sized<boost::uint32_t>(table);
            }
        };

        struct Mp4ChunkOffsetBox
            : Mp4FullBoxData<Mp4ChunkOffsetBox, Mp4BoxType::stco>
        {
            //boost::uint32_t entry_count;
            //for (i=1; i <= entry_count; i++) {
            //    boost::uint32_t chunk_offset;
            //}
            std::vector<boost::uint32_t> table;

            Mp4ChunkOffsetBox()
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4FullBoxHeader::serialize(ar);
                ar & util::serialization::make_sized<boost::uint32_t>(table);
            }
        };

        struct Mp4ChunkLargeOffsetBox
            : Mp4FullBoxData<Mp4ChunkLargeOffsetBox, Mp4BoxType::co64>
        {
            //boost::uint32_t entry_count;
            //for (i=1; i <= entry_count; i++) {
            //    boost::uint64_t chunk_offset;
            //}
            std::vector<boost::uint64_t> table;

            Mp4ChunkLargeOffsetBox()
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4FullBoxHeader::serialize(ar);
                ar & util::serialization::make_sized<boost::uint32_t>(table);
            }
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MP4_BOX_MP4_SAMPLE_BOX_H_
