// Mp4SampleTable.h

#ifndef _PPBOX_AVFORMAT_MP4_LIB_MP4_SAMPLE_TABLE_H_
#define _PPBOX_AVFORMAT_MP4_LIB_MP4_SAMPLE_TABLE_H_

#include "ppbox/avformat/mp4/lib/Mp4BoxWrapper.h"
#include "ppbox/avformat/mp4/lib/Mp4SampleMeta.h"
#include "ppbox/avformat/mp4/lib/Mp4SampleData.h"
#include "ppbox/avformat/mp4/lib/Mp4SampleDescription.h"

#include <ppbox/avbase/Sample.h>

namespace ppbox
{
    namespace avformat
    {

        class Mp4SampleTable
            : public Mp4BoxWrapper2<Mp4SampleTableBox>
        {
        public:
            Mp4SampleTable(
                Mp4Box & box);

        public:
            boost::uint32_t count() const;

            void get(
                ppbox::avbase::Sample & sample) const;

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

            Mp4SampleEntry const & description() const
            {
                return stsd_.description(stsc_.description());
            }

            boost::uint64_t offset() const
            {
                return stsc_.offset();
            }

        public:
            bool next(
                boost::system::error_code & ec);

            bool seek(
                boost::uint64_t & time, // dts
                boost::system::error_code & ec);

            void rewind();

        private:
            Mp4SampleDescriptionTable stsd_;
            Mp4TimeToSampleTable stts_;
            Mp4CompositionOffsetTable ctts_;
            Mp4SyncSampleTable stss_;
            Mp4SampleToChunkTable stsc_;
            Mp4SampleSizeTable stsz_;

            boost::uint32_t sample_count_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_LIB_MP4_SAMPLE_TABLE_H_