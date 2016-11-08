// Mp4SampeMeta.cpp

#include "just/avformat/Common.h"
#include "just/avformat/mp4/lib/Mp4SampleMeta.h"
#include "just/avformat/mp4/lib/Mp4SampleTable.h"
#include "just/avformat/mp4/box/Mp4Box.hpp"

namespace just
{
    namespace avformat
    {

        // Mp4TimeToSampleTable

        Mp4TimeToSampleTable::Mp4TimeToSampleTable(
            Mp4Box * box)
            : Mp4BoxWrapper<Mp4TimeToSampleBox>(box)
              , index_(0)
              , value_(0)
        {
            if (data_->table.empty()) {
                entry_.sample_count = 0;
                entry_.sample_delta = 0;
            } else {
                entry_ = data_->table[index_];
            }
        }

        bool Mp4TimeToSampleTable::put(
            boost::uint64_t dts)
        {
            if (dts < value_)
                return false;
            if (entry_.sample_count == 0) {
            } else if (entry_.sample_count == 1) {
                entry_.sample_delta = (boost::uint32_t)(dts - value_);
            } else if (value_ + entry_.sample_delta == dts) {
            } else {
                --entry_.sample_count;
                data_->table.push_back(entry_);
                entry_.sample_count = 1;
                entry_.sample_delta = (boost::uint32_t)(dts - value_);
            }
            value_ = dts;
            ++entry_.sample_count;
            return true;
        }

        bool Mp4TimeToSampleTable::put_eos()
        {
            data_->table.push_back(entry_);
            entry_.sample_count = 0;
            return true;
        }

        bool Mp4TimeToSampleTable::merge(
            Mp4TimeToSampleTable const & table)
        {
            std::vector<Mp4TimeToSampleBox::Entry> & l(data_->table);
            std::vector<Mp4TimeToSampleBox::Entry> const & r(table.data_->table);
            if (r.empty()) {
                return true;
            }
            if (l.empty()) {
                l = r;
                return true;
            }
            if (l.back().sample_delta == r.front().sample_delta) {
                l.back().sample_count += r.front().sample_count;
                l.insert(l.end(), r.begin() + 1, r.end());
            } else {
                l.insert(l.end(), r.begin(), r.end());
            }
            return true;
        }

        bool Mp4TimeToSampleTable::next()
        {
            value_ += entry_.sample_delta;
            if (--entry_.sample_count == 0) {
                if (++index_ < data_->table.size()) {
                    entry_ = data_->table[index_];
                } else {
                    return false;
                }
            }
            return true;
        }

        bool Mp4TimeToSampleTable::seek(
            boost::uint64_t sample_time, 
            boost::uint32_t & sample_index) const
        {
            sample_index = 0;
            size_t table_index = 0;
            boost::uint32_t flag = boost::uint32_t((sample_time & Mp4SampleTable::SEEK_TO_UPPER) >> 32);
            sample_time &= ~Mp4SampleTable::SEEK_TO_UPPER;
            Mp4TimeToSampleBox::Entry entry = data_->table[table_index];
            while (entry.sample_count * entry.sample_delta <= sample_time) {
                sample_time -= (boost::uint64_t)entry.sample_delta * entry_.sample_count;
                sample_index += entry.sample_count;
                if (++table_index < data_->table.size()) {
                    entry = data_->table[table_index];
                } else {
                    return false;
                }
            }
            if (sample_time) {
                boost::uint32_t count = (boost::uint32_t)(sample_time / entry_.sample_delta);
                if (flag && (sample_time % entry_.sample_delta)) {
                    ++count;
                    if (count == entry.sample_count) {
                        ++table_index;
                    }
                }
                sample_index += count;
                sample_index |= flag;
                return table_index < data_->table.size();
            }
            sample_index |= flag;
            return true;
        }

        bool Mp4TimeToSampleTable::seek(
            boost::uint32_t sample_index, 
            boost::uint64_t & sample_time) const
        {
            size_t table_index = 0;
            sample_time = 0;
            Mp4TimeToSampleBox::Entry entry = data_->table[table_index];
            while (entry.sample_count <= sample_index) {
                sample_index -= entry.sample_count;
                sample_time += (boost::uint64_t)entry.sample_delta * entry.sample_count;
                if (++table_index < data_->table.size()) {
                    entry = data_->table[table_index];
                } else {
                    return false;
                }
            }
            sample_time += (boost::uint64_t)entry.sample_delta * sample_index;
            return true;
        }

        bool Mp4TimeToSampleTable::seek(
            boost::uint32_t sample_index)
        {
            size_t table_index = 0;
            boost::uint64_t sample_time = 0;
            Mp4TimeToSampleBox::Entry entry = data_->table[table_index];
            while (entry.sample_count <= sample_index) {
                sample_index -= entry.sample_count;
                sample_time += (boost::uint64_t)entry.sample_delta * entry.sample_count;
                if (++table_index < data_->table.size()) {
                    entry = data_->table[table_index];
                } else {
                    return false;
                }
            }
            sample_time += (boost::uint64_t)entry.sample_delta * sample_index;
            entry.sample_count -= sample_index;
            entry_ = entry;
            index_ = table_index;
            value_ = sample_time;
            return true;
        }

        boost::uint32_t Mp4TimeToSampleTable::count() const
        {
            boost::uint32_t n = 0;
            for (size_t i = 0; i < data_->table.size(); ++i)
                n += data_->table[i].sample_count;
            return n;
        }

        boost::uint64_t Mp4TimeToSampleTable::total_duration() const
        {
            boost::uint64_t n = 0;
            for (size_t i = 0; i < data_->table.size(); ++i)
                n += (boost::uint64_t)data_->table[i].sample_delta * data_->table[i].sample_count;
            return n;
        }

        // Mp4CompositionOffsetTable

        Mp4CompositionOffsetTable::Mp4CompositionOffsetTable(
            Mp4Box * box)
            : Mp4BoxWrapper<Mp4CompositionOffsetBox>(box)
              , index_(0)
        {
            Mp4CompositionOffsetBox::Entry entry = {0, 0};
            entry_ = (data_ && !data_->table.empty()) ? data_->table[index_] : entry;
        }

        bool Mp4CompositionOffsetTable::put(
            boost::uint32_t cts_delta)
        {
            if (entry_.sample_count == 0) {
                entry_.sample_offset = cts_delta;
            } else if (entry_.sample_offset != cts_delta) {
                data_->table.push_back(entry_);
                entry_.sample_count = 0;
                entry_.sample_offset = cts_delta;
            }
            ++entry_.sample_count;
            return true;
        }

        bool Mp4CompositionOffsetTable::put_eos()
        {
            if (data_)
                data_->table.push_back(entry_);
            return true;
        }

        bool Mp4CompositionOffsetTable::merge(
            Mp4CompositionOffsetTable const & table)
        {
            if (data_ == NULL && table.data_ == NULL)
                return true;
            std::vector<Mp4CompositionOffsetBox::Entry> & l(data_->table);
            std::vector<Mp4CompositionOffsetBox::Entry> const & r(table.data_->table);
            if (r.empty()) {
                return true;
            }
            if (l.empty()) {
                l = r;
                return true;
            }
            if (l.back().sample_offset == r.front().sample_offset) {
                l.back().sample_count += r.front().sample_count;
                l.insert(l.end(), r.begin() + 1, r.end());
            } else {
                l.insert(l.end(), r.begin(), r.end());
            }
            return true;
        }

        bool Mp4CompositionOffsetTable::next()
        {
            if (--entry_.sample_count == 0) {
                if (++index_ < data_->table.size()) {
                    entry_ = data_->table[index_];
                } else {
                    return false;
                }
            }
            return true;
        }

        bool Mp4CompositionOffsetTable::seek(
            boost::uint32_t sample_index)
        {
            if (!data_)
                return true;
            size_t table_index = 0;
            Mp4CompositionOffsetBox::Entry entry = data_->table[table_index];
            while (entry.sample_count <= sample_index) {
                sample_index -= entry.sample_count;
                if (++table_index < data_->table.size()) {
                    entry = data_->table[table_index];
                } else {
                    return false;
                }
            }
            entry.sample_count -= sample_index;
            entry_ = entry;
            index_ = table_index;
            return true;
        }

        // Mp4SyncSampleTable

        Mp4SyncSampleTable::Mp4SyncSampleTable(
            Mp4Box * box)
            : Mp4BoxWrapper<Mp4SyncSampleBox>(box)
              , index_(0)
        {
            entry_ = data_ ? data_->table[index_] : 1;
        }

        bool Mp4SyncSampleTable::put(
            bool is_sync)
        {
            if (data_ == NULL && is_sync) {
                ++index_;
                ++entry_;
                return true;
            }
            while (data_->table.size() < index_) {
                data_->table.push_back((boost::uint32_t)data_->table.size() + 1);
            }
            ++entry_;
            if (is_sync) {
                data_->table.push_back(entry_);
            }
            return true;
        }

        bool Mp4SyncSampleTable::put_eos()
        {
            return true;
        }

        bool Mp4SyncSampleTable::merge(
            boost::uint32_t sample_count, 
            Mp4SyncSampleTable const & table)
        {
            if (data_ == NULL && table.data_ == NULL)
                return true;
            std::vector<boost::uint32_t> & l(data_->table);
            std::vector<boost::uint32_t> const & r(table.data_->table);
            size_t n = l.size();
            l.insert(l.end(), r.begin(), r.end());
            for (; n < l.size(); ++n) {
                l[n] += sample_count;
            }
            return true;
        }

        bool Mp4SyncSampleTable::next()
        {
            if (!data_)
                return true;
            if (--entry_ == 0) {
                if (++index_ < data_->table.size()) {
                    entry_ = data_->table[index_] - data_->table[index_ - 1];
                }
            }
            return true;
        }

        static boost::uint32_t const SEEK_TO_UPPER_32 = 0x80000000ULL;

        bool Mp4SyncSampleTable::sync(
            boost::uint32_t & sample_index) const
        {
            if (!data_)
                return true;
            boost::uint32_t flag = sample_index & SEEK_TO_UPPER_32;
            sample_index &= ~SEEK_TO_UPPER_32;
            std::vector<boost::uint32_t>::const_iterator iter = 
                std::lower_bound(data_->table.begin(), data_->table.end(), sample_index + 1);
            if (iter == data_->table.end())
                return false;
            if (flag && *iter != sample_index) {
                ++iter;
                if (iter == data_->table.end())
                    return false;
            }
            sample_index = *iter - 1;
            return true;
        }

        bool Mp4SyncSampleTable::sync(
            boost::uint32_t & sample_index,
            bool end) const
        {
            if (!data_)
                return true;
            std::vector<boost::uint32_t>::const_iterator iter;
            if (end){
                iter = std::lower_bound(data_->table.begin(), data_->table.end(), sample_index + 1);
                if (iter != data_->table.end())
                    sample_index =  *(iter) - 1;
                else
                    sample_index = -1;
            }else{
                iter = std::upper_bound(data_->table.begin(), data_->table.end(), sample_index + 1);
                if (iter != data_->table.begin())
                    sample_index =  *(--iter) - 1;
                else 
                    sample_index = 0;
            }
            return true;
        }

        bool Mp4SyncSampleTable::seek(
            boost::uint32_t sample_index)
        {
            if (!data_)
                return true;
            size_t table_index = 0;
            boost::uint32_t entry = data_->table[table_index];
            while (entry <= sample_index) {
                if (++table_index < data_->table.size()) {
                    entry = data_->table[table_index];
                } else {
                    entry = sample_index;
                    break;
                }
            }
            entry -= sample_index;
            entry_ = entry;
            index_ = table_index;
            return true;
        }

    } // namespace avformat
} // namespace just
