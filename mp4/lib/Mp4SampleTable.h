// Mp4SampleTable.h

#ifndef _JUST_AVFORMAT_MP4_LIB_MP4_SAMPLE_TABLE_H_
#define _JUST_AVFORMAT_MP4_LIB_MP4_SAMPLE_TABLE_H_

#include "just/avformat/mp4/lib/Mp4BoxWrapper.h"
#include "just/avformat/mp4/lib/Mp4SampleMeta.h"
#include "just/avformat/mp4/lib/Mp4SampleData.h"
#include "just/avformat/mp4/lib/Mp4SampleDescription.h"

#include <just/avbase/Sample.h>

namespace just
{
    namespace avformat
    {

        class Mp4SampleTable
            : public Mp4BoxWrapper2<Mp4SampleTableBox>
        {
        public:
            Mp4SampleTable(
                Mp4Box & box);

            Mp4SampleTable(
                Mp4Box & box, 
                create_new_t);

        public:
            bool put(
                just::avbase::Sample const & sample);

            bool put_eos();

        public:
            bool merge(
                Mp4SampleTable const & table, 
                boost::system::error_code & ec);

            void shift(
                boost::int64_t offset);

        public:
            static boost::uint64_t const SEEK_TO_UPPER = 0x8000000000000000ULL;

            bool next(
                boost::system::error_code & ec);

            bool seek(
                boost::uint64_t & time, // dts
                bool lower,
                boost::system::error_code & ec);
            bool seek(
                boost::uint64_t & time, // dts
                boost::system::error_code & ec);

            void rewind();

            bool limit(
                boost::uint64_t offset, 
                boost::uint64_t & time, // dts
                boost::system::error_code & ec) const;

        public:
            boost::uint32_t count() const;

            boost::uint64_t duration() const;

            Mp4SampleDescriptionTable & description_table()
            {
                return stsd_;
            }

        public:
            void get(
                just::avbase::Sample & sample) const;

            boost::uint64_t dts() const
            {
                return stts_.dts();
            }

            boost::uint32_t cts_delta() const
            {
                return ctts_.cts_delta();
            }

            bool is_sync() const
            {
                return stss_.is_sync();
            }

            Mp4SampleEntry const * description() const
            {
                return stsd_.description(stsc_.description());
            }

            boost::uint64_t offset() const
            {
                return stco_.offset();
            }

        private:
            Mp4SampleDescriptionTable stsd_;
            Mp4TimeToSampleTable stts_;
            Mp4CompositionOffsetTable ctts_;
            Mp4SyncSampleTable stss_;
            Mp4ChunkOffsetTable stco_;
            Mp4SampleToChunkTable stsc_;
            Mp4SampleSizeTable stsz_;

            boost::uint32_t sample_count_;
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MP4_LIB_MP4_SAMPLE_TABLE_H_
