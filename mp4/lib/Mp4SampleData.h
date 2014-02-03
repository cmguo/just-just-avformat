// Mp4SampleData.h

#ifndef _PPBOX_AVFORMAT_MP4_LIB_MP4_SAMPLE_DATA_H_
#define _PPBOX_AVFORMAT_MP4_LIB_MP4_SAMPLE_DATA_H_

#include "ppbox/avformat/mp4/box/Mp4SampleBox.h"
#include "ppbox/avformat/mp4/lib/Mp4BoxWrapper.h"

namespace ppbox
{
    namespace avformat
    {

        struct Mp4SampleToChunkTable
            : Mp4BoxWrapper<Mp4SampleToChunkBox>
        {
        public:
            Mp4SampleToChunkTable(
                Mp4Box * box, 
                Mp4Box * box2);

            bool next(
                boost::uint32_t size);

            bool seek(
                boost::uint32_t sample_index);

            boost::uint64_t offset() const
            {
                return offset_;
            }

            boost::uint32_t description() const
            {
                return entry_.sample_description_index - 1;
            }

        private:
            Mp4ChunkOffsetBox const * box2_;
            Mp4SampleToChunkBox::Entry entry_;
            size_t index_;
            boost::uint32_t samples_per_chunk_;
            boost::uint32_t first_chunk_; // next first_chunk_
            boost::uint64_t offset_;
        };

        struct Mp4SampleSizeTable
            : Mp4BoxWrapper<Mp4SampleSizeBox>
        {
        public:
            Mp4SampleSizeTable(
                Mp4Box * box);

            bool next();

            bool seek(
                boost::uint32_t sample_index);

            boost::uint32_t count() const
            {
                return data_->table.size();
            }

            boost::uint32_t size() const
            {
                return entry_;
            }

        private:
            boost::uint32_t entry_;
            size_t index_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_LIB_MP4_SAMPLE_DATA_H_
