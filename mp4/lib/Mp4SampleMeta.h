// Mp4SampeMeta.h

#ifndef _PPBOX_AVFORMAT_MP4_LIB_MP4_SAMPLE_META_H_
#define _PPBOX_AVFORMAT_MP4_LIB_MP4_SAMPLE_META_H_

#include "ppbox/avformat/mp4/lib/Mp4BoxWrapper.h"
#include "ppbox/avformat/mp4/box/Mp4SampleBox.h"

namespace ppbox
{
    namespace avformat
    {

        class Mp4TimeToSampleTable
            : Mp4BoxWrapper<Mp4TimeToSampleBox>
        {
        public:
            Mp4TimeToSampleTable(
                Mp4Box * box);

            bool next();

            bool seek(
                boost::uint64_t sample_time, 
                boost::uint32_t & sample_index);

            bool seek(
                boost::uint32_t sample_index);

            boost::uint32_t count() const;

            boost::uint64_t dts() const
            {
                return value_;
            }

            boost::uint32_t duration() const
            {
                return entry_.sample_delta;
            }

        private:
            Mp4TimeToSampleBox::Entry entry_;
            size_t index_;
            boost::uint64_t value_;
        };

        class Mp4CompositionOffsetTable
            : Mp4BoxWrapper<Mp4CompositionOffsetBox>
        {
        public:
            Mp4CompositionOffsetTable(
                Mp4Box * box);

            bool next();

            bool seek(
                boost::uint32_t sample_index);

            boost::uint32_t cts_delta() const
            {
                return entry_.sample_offset;
            }

        private:
            Mp4CompositionOffsetBox::Entry entry_;
            size_t index_;
        };

        class Mp4SyncSampleTable
            : Mp4BoxWrapper<Mp4SyncSampleBox>
        {
        public:
            Mp4SyncSampleTable(
                Mp4Box * box);

            bool next();

            bool sync(
                boost::uint32_t & index) const;

            bool seek(
                boost::uint32_t sample_index);

            bool is_sync() const
            {
                return entry_ == 1;
            }

        private:
            boost::uint32_t entry_;
            size_t index_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_LIB_MP4_SAMPLE_META_H_
