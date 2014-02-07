// Mp4SampleTable.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/lib/Mp4SampleTable.h"
#include "ppbox/avformat/mp4/box/Mp4Box.hpp"
#include "ppbox/avformat/mp4/box/Mp4BoxVector.hpp"

namespace ppbox
{
    namespace avformat
    {

        Mp4SampleTable::Mp4SampleTable(
            Mp4Box & box)
            : Mp4BoxWrapper2<Mp4SampleTableBox>(box)
            , stsd_(find_item("/stsd"))
            , stts_(find_item("/stts"))
            , ctts_(find_item("/ctts"))
            , stss_(find_item("/stss"))
            , stco_(find_item("/stco"))
            , stsc_(find_item("/stsc"), &stco_)
            , stsz_(find_item("/stsz"), &stco_)
        {
        }

        bool Mp4SampleTable::merge(
            Mp4SampleTable const & table, 
            boost::system::error_code & ec)
        {
            bool result = stts_.merge(table.stts_)
                && ctts_.merge(table.ctts_)
                && stss_.merge(count(), table.stss_)
                && stsc_.merge(table.stsc_) // must before stco_.merge, depends on stco_'s old entry count
                && stco_.merge(table.stco_)
                && stsz_.merge(table.stsz_);
            if (result) {
                ec.clear();
            } else {
                ec = framework::system::logic_error::out_of_range;
            }
            return result;
        }

        void Mp4SampleTable::shift(
            boost::int64_t offset)
        {
            stco_.shift(offset);
        }

        boost::uint32_t Mp4SampleTable::count() const
        {
            boost::uint32_t n = stsz_.count();
            if (n == 0)
                n = stts_.count();
            return n;
        }

        void Mp4SampleTable::get(
            ppbox::avbase::Sample & sample) const
        {
            sample.flags = stss_.is_sync() ? sample.f_sync : 0;
            sample.dts = stts_.dts();
            sample.cts_delta = ctts_.cts_delta();
            sample.duration = stts_.duration();
            sample.time = stco_.offset(); // use time for offset
            sample.size = stsz_.size();
        }

        bool Mp4SampleTable::next(
            boost::system::error_code & ec)
        {
            bool result = stts_.next() 
                && ctts_.next() 
                && stss_.next()
                && stsz_.next() // must call before stsc_.next()
                && stsc_.next();
            if (result) {
                ec.clear();
            } else {
                ec = framework::system::logic_error::out_of_range;
            }
            return result;
        }

        bool Mp4SampleTable::seek(
            boost::uint64_t & time, // dts
            boost::system::error_code & ec)
        {
            boost::uint32_t sample_index = 0;
            boost::uint32_t sample_index2 = 0;
            bool result = stts_.seek(time, sample_index)
                && stss_.sync(sample_index);
            if (result) {
                result = stts_.seek(sample_index)
                    && ctts_.seek(sample_index)
                    && stss_.seek(sample_index)
                    && stsc_.seek(sample_index, sample_index2)
                    && stsz_.seek(sample_index, sample_index2);
            }
            if (result) {
                ec.clear();
            } else {
                ec = framework::system::logic_error::out_of_range;
            }
            return result;
        }

        void Mp4SampleTable::rewind()
        {
            boost::uint32_t sample_index2 = 0;
            stss_.seek(0);
            ctts_.seek(0);
            stss_.seek(0);
            stsc_.seek(0, sample_index2);
            stsz_.seek(0, sample_index2);
        }

        bool Mp4SampleTable::limit(
            boost::uint64_t offset, 
            boost::uint64_t & time, // dts
            boost::system::error_code & ec) const
        {
            Mp4SampleToChunkBox::Entry index;
            return stco_.limit(offset, index)
                && stsc_.limit(offset, index)
                && stsz_.limit(offset, index)
                && stts_.seek(index.sample_description_index, time);
        }

    } // namespace avformat
} // namespace ppbox
