// Mp4SampeMeta.h

#ifndef _JUST_AVFORMAT_MP4_LIB_MP4_SAMPLE_META_H_
#define _JUST_AVFORMAT_MP4_LIB_MP4_SAMPLE_META_H_

#include "just/avformat/mp4/lib/Mp4BoxWrapper.h"
#include "just/avformat/mp4/box/Mp4SampleBox.h"

namespace just
{
    namespace avformat
    {

        class Mp4TimeToSampleTable
            : public Mp4BoxWrapper<Mp4TimeToSampleBox>
        {
        public:
            Mp4TimeToSampleTable(
                Mp4Box * box);

        public:
            bool put(
                boost::uint64_t dts);

            bool put_eos();

        public:
            bool merge(
                Mp4TimeToSampleTable const & table);

        public:
            bool next();

            bool seek(
                boost::uint64_t sample_time, 
                boost::uint32_t & sample_index) const;

            bool seek(
                boost::uint32_t sample_index, 
                boost::uint64_t & sample_time) const;

            bool seek(
                boost::uint32_t sample_index);

        public:
            boost::uint32_t count() const;

            boost::uint64_t total_duration() const;

        public:
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
            : public Mp4BoxWrapper<Mp4CompositionOffsetBox>
        {
        public:
            Mp4CompositionOffsetTable(
                Mp4Box * box);

        public:
            bool put(
                boost::uint32_t cts_delta);

            bool put_eos();

        public:
            bool merge(
                Mp4CompositionOffsetTable const & table);

        public:
            bool next();

            bool seek(
                boost::uint32_t sample_index);

        public:
            boost::uint32_t cts_delta() const
            {
                return entry_.sample_offset;
            }

        private:
            Mp4CompositionOffsetBox::Entry entry_;
            size_t index_;
        };

        class Mp4SyncSampleTable
            : public Mp4BoxWrapper<Mp4SyncSampleBox>
        {
        public:
            Mp4SyncSampleTable(
                Mp4Box * box);

        public:
            bool put(
                bool is_sync);

            bool put_eos();

        public:
            bool merge(
                boost::uint32_t sample_count, 
                Mp4SyncSampleTable const & table);

        public:
            bool next();

            bool sync(
                boost::uint32_t & sample_index) const;

            bool seek(
                boost::uint32_t sample_index);

        public:
            bool is_sync() const
            {
                return entry_ == 1;
            }

        private:
            boost::uint32_t entry_;
            size_t index_;
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MP4_LIB_MP4_SAMPLE_META_H_
