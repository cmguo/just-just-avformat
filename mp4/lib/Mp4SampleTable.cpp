// Mp4SampleTable.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/lib/Mp4SampleTable.h"
#include "ppbox/avformat/mp4/box/Mp4Box.hpp"

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
            , stsc_(find_item("/stsc"), find_item("/stco"))
            , stsz_(find_item("/stsz"))
        {
            box.as<Mp4SampleTableBox>();
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
            sample.time = stsc_.offset(); // use time for offset
            sample.size = stsz_.size();
        }

        bool Mp4SampleTable::next(
            boost::system::error_code & ec)
        {
            bool result = stts_.next() 
                && ctts_.next() 
                && stss_.next()
                && stsc_.next(stsz_.size()) 
                && stsz_.next();
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
            boost::uint32_t sample_index;
            bool result = stts_.seek(time, sample_index)
                && stss_.sync(sample_index)
                && stss_.seek(sample_index)
                && ctts_.seek(sample_index)
                && stss_.seek(sample_index)
                && stsc_.seek(sample_index)
                && stsz_.seek(sample_index);
            if (result) {
                ec.clear();
            } else {
                ec = framework::system::logic_error::out_of_range;
            }
            return result;
        }

        void Mp4SampleTable::rewind()
        {
            stss_.seek(0);
            ctts_.seek(0);
            stss_.seek(0);
            stsc_.seek(0);
            stsz_.seek(0);
        }

    } // namespace avformat
} // namespace ppbox
