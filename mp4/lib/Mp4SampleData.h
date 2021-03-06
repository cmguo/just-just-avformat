// Mp4SampleData.h

#ifndef _JUST_AVFORMAT_MP4_LIB_MP4_SAMPLE_DATA_H_
#define _JUST_AVFORMAT_MP4_LIB_MP4_SAMPLE_DATA_H_

#include "just/avformat/mp4/box/Mp4SampleBox.h"
#include "just/avformat/mp4/lib/Mp4BoxWrapper.h"

namespace just
{
    namespace avformat
    {

        class Mp4ChunkOffsetTable
            : public Mp4BoxWrapper<Mp4ChunkOffsetBox>
        {
        public:
            Mp4ChunkOffsetTable(
                Mp4Box * box,
                Mp4Box * box2);

        public:
            bool put(
                boost::uint64_t offset);

            bool put_eos();

        public:
            bool merge(
                Mp4ChunkOffsetTable const & table);

            void shift(
                boost::int64_t offset);

        public:
            // next chunk
            bool next();

            bool next(
                boost::uint32_t size);

            // seek to chunk
            bool seek(
                boost::uint32_t chunk_index);

            bool limit(
                boost::uint64_t & offset, 
                Mp4SampleToChunkBox::Entry & index) const;

        public:
            boost::uint32_t count() const
            {
                return data_ ? data_->table.size() : co64_->table.size();
            }

            boost::uint64_t offset() const
            {
                return offset_;
            }

        private:
            Mp4BoxWrapper<Mp4ChunkLargeOffsetBox> co64_;
            size_t index_;
            boost::uint64_t offset_;
        };

        class Mp4SampleToChunkTable
            : public Mp4BoxWrapper<Mp4SampleToChunkBox>
        {
        public:
            Mp4SampleToChunkTable(
                Mp4Box * box, 
                Mp4ChunkOffsetTable * chunk);

        public:
            bool put(
                boost::uint32_t sample_description_index);

            bool put_eos();

        public:
            bool merge(
                Mp4SampleToChunkTable const & table);

        public:
            bool next();

            // out sample_index2 sample index in current chunk
            bool seek(
                boost::uint32_t sample_index, 
                boost::uint32_t & sample_index2);

            bool limit(
                boost::uint64_t & offset, 
                Mp4SampleToChunkBox::Entry  & index) const;

        public:
            boost::uint32_t description() const
            {
                return entry_.sample_description_index - 1;
            }

        private:
            Mp4ChunkOffsetTable * chunk_;
            Mp4SampleToChunkBox::Entry entry_;
            size_t index_;
            boost::uint32_t samples_per_chunk_;
            boost::uint32_t first_chunk_; // next first_chunk_
        };

        class Mp4SampleSizeTable
            : public Mp4BoxWrapper<Mp4SampleSizeBox>
        {
        public:
            Mp4SampleSizeTable(
                Mp4Box * box, 
                Mp4ChunkOffsetTable * chunk);

        public:
            bool put(
                boost::uint32_t size);

            bool put_eos();

        public:
            bool merge(
                Mp4SampleSizeTable const & table);

        public:
            bool next();

            bool seek(
                boost::uint32_t sample_index, 
                boost::uint32_t sample_index2);

            bool limit(
                boost::uint64_t & offset, 
                Mp4SampleToChunkBox::Entry  & index) const;

        public:
            boost::uint32_t count() const
            {
                return data_->table.size();
            }

            boost::uint32_t size() const
            {
                return entry_;
            }

        private:
            Mp4ChunkOffsetTable * chunk_;
            boost::uint32_t entry_;
            size_t index_;
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MP4_LIB_MP4_SAMPLE_DATA_H_
