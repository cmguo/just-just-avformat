// Mp4SampleData.cpp

#include "just/avformat/Common.h"
#include "just/avformat/mp4/lib/Mp4SampleData.h"
#include "just/avformat/mp4/box/Mp4Box.hpp"

#include <algorithm>
#include <numeric>

namespace just
{
    namespace avformat
    {

        // Mp4ChunkOffsetTable

        Mp4ChunkOffsetTable::Mp4ChunkOffsetTable(
            Mp4Box * box, 
            Mp4Box * box2)
            : Mp4BoxWrapper<Mp4ChunkOffsetBox>(box)
            , co64_(box2)
            , index_(0)
            , offset_(0)
        {
            if (data_) {
                if (!data_->table.empty())
                    offset_ = data_->table[index_];
            } else {
                if (co64_->table.empty())
                    offset_ = co64_->table[index_];
            }
        }

        bool Mp4ChunkOffsetTable::put(
            boost::uint64_t offset)
        {
            if (offset_ != offset) {
                if (data_)
                    data_->table.push_back(offset);
                else
                    co64_->table.push_back(offset);
                offset_ = offset;
            }
            return true;
        }

        bool Mp4ChunkOffsetTable::put_eos()
        {
            return true;
        }

        template <typename T>
        static void merge_vec(
            std::vector<T> & l, 
            std::vector<T> const & r)
        {
            l.insert(l.end(), r.begin(), r.end());
        }

        bool Mp4ChunkOffsetTable::merge(
            Mp4ChunkOffsetTable const & table)
        {
            if (data_) {
                merge_vec(data_->table, table.data_->table);
            } else {
                merge_vec(co64_->table, table.co64_->table);
            }
            return true;
        }

        template <typename T>
        static void transform(
            std::vector<T> & table, 
            boost::int64_t offset)
        {
            std::transform(table.begin(), table.end(), 
                table.begin(), std::bind2nd(std::plus<boost::uint32_t>(), (boost::uint32_t)offset));
        }

        void Mp4ChunkOffsetTable::shift(
            boost::int64_t offset)
        {
            if (data_) {
                transform(data_->table, offset); 
            } else {
                transform(co64_->table, offset); 
            }
        }

        bool Mp4ChunkOffsetTable::next()
        {
            ++index_;
            if (data_) {
                if (index_ >= data_->table.size())
                    return false;
                offset_ = data_->table[index_];
            } else {
                if (index_ >= co64_->table.size())
                    return false;
                offset_ = co64_->table[index_];
            }
            return true;
        }

        bool Mp4ChunkOffsetTable::next(
            boost::uint32_t size)
        {
            offset_ += size;
            return true;
        }

        // seek to chunk
        bool Mp4ChunkOffsetTable::seek(
            boost::uint32_t chunk_index)
        {
            if (data_) {
                if (chunk_index >= data_->table.size()) {
                    return false;
                }
                index_ = chunk_index;
                offset_ = data_->table[index_];
            } else {
                if (chunk_index >= co64_->table.size()) {
                    return false;
                }
                index_ = chunk_index;
                offset_ = co64_->table[index_];
            }
            return true;
        }

        template <typename T>
        static boost::uint32_t limit_(
            std::vector<T> const & table, 
            boost::uint64_t & offset)
        {
            typename std::vector<T>::const_iterator iter = 
                std::upper_bound(table.begin(), table.end(), offset);
            boost::uint32_t first_chunk = std::distance(table.begin(), iter);
            if (first_chunk == 0) {
                offset = 0;
            } else {
                --first_chunk;
                offset -= table[first_chunk];
            }
            return first_chunk;
        }

        bool Mp4ChunkOffsetTable::limit(
            boost::uint64_t & offset, 
            Mp4SampleToChunkBox::Entry & index) const
        {
            index.first_chunk = data_
                ? limit_(data_->table, offset)
                : limit_(co64_->table, offset);
            return true;
        }

        // Mp4SampleToChunkTable

        Mp4SampleToChunkTable::Mp4SampleToChunkTable(
            Mp4Box * box, 
            Mp4ChunkOffsetTable * chunk)
            : Mp4BoxWrapper<Mp4SampleToChunkBox>(box)
            , chunk_(chunk)
            , index_(0)
            , samples_per_chunk_(0)
            , first_chunk_(0)
        {
            if (data_->table.empty()) {
                entry_.first_chunk = 0;
                entry_.samples_per_chunk = 0;
                entry_.sample_description_index = 10; // make diff when compare with sample_description_index the first time
                first_chunk_ = 0;
            } else {
                entry_ = data_->table[index_];
                if (index_ + 1 < data_->table.size()) {
                    first_chunk_ = data_->table[index_ + 1].first_chunk;
                }
                chunk_->seek(entry_.first_chunk - 1);
                samples_per_chunk_ = entry_.samples_per_chunk;
            }
        }

        bool Mp4SampleToChunkTable::put(
            boost::uint32_t sample_description_index)
        {
            if (first_chunk_ != chunk_->count()) {
                if (samples_per_chunk_ != entry_.samples_per_chunk 
                    || sample_description_index != entry_.sample_description_index) {
                        if (first_chunk_ == 0) {
                            entry_.first_chunk = chunk_->count();
                            entry_.sample_description_index = sample_description_index;
                        } else if (first_chunk_ == 1) {
                            entry_.samples_per_chunk = samples_per_chunk_;
                        } else {
                            ++entry_.sample_description_index;
                            data_->table.push_back(entry_);
                            entry_.first_chunk = first_chunk_;
                            entry_.samples_per_chunk = samples_per_chunk_;
                            entry_.sample_description_index = sample_description_index;
                        }
                }
                first_chunk_ = chunk_->count();
                samples_per_chunk_ = 0;
            }
            ++samples_per_chunk_;
            return true;
        }

        bool Mp4SampleToChunkTable::put_eos()
        {
            if (first_chunk_ == 1) {
                entry_.samples_per_chunk = samples_per_chunk_;
            }
            ++entry_.sample_description_index;
            if (samples_per_chunk_ != entry_.samples_per_chunk) {
                data_->table.push_back(entry_);
                entry_.first_chunk = chunk_->count();
                entry_.samples_per_chunk = samples_per_chunk_;
            }
            data_->table.push_back(entry_);
            return true;
        }

        bool Mp4SampleToChunkTable::merge(
            Mp4SampleToChunkTable const & table)
        {
            std::vector<Mp4SampleToChunkBox::Entry> & l(data_->table);
            std::vector<Mp4SampleToChunkBox::Entry> const & r(table.data_->table);
            if (r.empty()) {
                return true;
            }
            if (l.empty()) {
                l = r;
                return true;
            }
            size_t n = l.size();
            if (l.back().samples_per_chunk == r.front().samples_per_chunk
                && l.back().sample_description_index == r.front().sample_description_index) {
                    l.insert(l.end(), r.begin() + 1, r.end());
            } else {
                l.insert(l.end(), r.begin(), r.end());
            }
            boost::uint32_t cn(chunk_->count());
            for (; n < l.size(); ++n) {
                l[n].first_chunk += cn;
            }
            return true;
        }

        bool Mp4SampleToChunkTable::next()
        {
            if (--samples_per_chunk_ == 0) {
                if (++entry_.first_chunk == first_chunk_) {
                    if (++index_ < data_->table.size()) {
                        entry_ = data_->table[index_];
                        if (index_ + 1 < data_->table.size()) {
                            first_chunk_ = data_->table[index_ + 1].first_chunk;
                        }
                    }
                }
                if (!chunk_->next()) {
                    return false;
                }
                samples_per_chunk_ = entry_.samples_per_chunk;
            }
            return true;
        }

        bool Mp4SampleToChunkTable::seek(
            boost::uint32_t sample_index, 
            boost::uint32_t & sample_index2)
        {
            size_t table_index = 0;
            boost::uint32_t first_chunk = 0; // next first_chunk_
            if (1 < data_->table.size()) {
                first_chunk = data_->table[1].first_chunk;
            }
            Mp4SampleToChunkBox::Entry entry = data_->table[table_index];
            while (entry.samples_per_chunk * (first_chunk - entry.first_chunk) <= sample_index) {
                sample_index -= entry.samples_per_chunk * (first_chunk - entry.first_chunk);
                if (++table_index < data_->table.size()) {
                    entry = data_->table[table_index];
                    if (table_index + 1 < data_->table.size()) {
                        first_chunk = data_->table[table_index + 1].first_chunk;
                    } else {
                        --first_chunk;
                    }
                } else {
                    assert(false);
                    return false;
                }
            }
            entry.first_chunk += sample_index / entry.samples_per_chunk;
            sample_index2 = sample_index %= entry.samples_per_chunk;
            if (!chunk_->seek(entry.first_chunk - 1)) {
                return false;
            }
            entry_ = entry;
            index_ = table_index;
            samples_per_chunk_ = entry.samples_per_chunk - sample_index;
            first_chunk_ = first_chunk;
            return true;
        }

        static bool less_chunk_index(
            Mp4SampleToChunkBox::Entry const & l, 
            Mp4SampleToChunkBox::Entry const & r)
        {
            return l.first_chunk < r.first_chunk;
        }

        static Mp4SampleToChunkBox::Entry sum_sample_count(
            Mp4SampleToChunkBox::Entry const & l, 
            Mp4SampleToChunkBox::Entry const & r)
        {
            Mp4SampleToChunkBox::Entry result = {r.first_chunk, r.samples_per_chunk, 
                l.sample_description_index + l.samples_per_chunk * (r.first_chunk - l.first_chunk)};
            return result;
        }

        bool Mp4SampleToChunkTable::limit(
            boost::uint64_t & offset, 
            Mp4SampleToChunkBox::Entry & index) const
        {
            // calc sample index by chunk index, 
            Mp4SampleToChunkBox::Entry point = {index.first_chunk + 1, 0, 0};
            std::vector<Mp4SampleToChunkBox::Entry>::const_iterator iter2 = 
                std::upper_bound(data_->table.begin(), data_->table.end(), point, less_chunk_index);
            index = std::accumulate(data().table.begin(), iter2, point, sum_sample_count);
            index.sample_description_index += index.samples_per_chunk * (point.first_chunk - index.first_chunk);
            index.first_chunk = point.first_chunk - 1;
            return true;
        }

        // Mp4SampleSizeTable

        Mp4SampleSizeTable::Mp4SampleSizeTable(
            Mp4Box * box, 
            Mp4ChunkOffsetTable * chunk)
            : Mp4BoxWrapper<Mp4SampleSizeBox>(box)
            , chunk_(chunk)
            , index_(0)
        {
            entry_ = data_->sample_size 
                ? data_->sample_size : (data_->table.empty() ? 0 : data_->table[index_]);
        }

        bool Mp4SampleSizeTable::put(
            boost::uint32_t size)
        {
            if (entry_ == 0) {
                if (data_->sample_size == 0) {
                    data_->sample_size = size;
                } else if (data_->sample_size != size) {
                    data_->table.resize(index_, data_->sample_size);
                    data_->sample_size = 0;
                    data_->table.push_back(size);
                    entry_ = size;
                }
            } else {
                data_->table.push_back(size);
            }
            chunk_->next(size);
            ++index_;
            return true;
        }

        bool Mp4SampleSizeTable::put_eos()
        {
            return true;
        }

        bool Mp4SampleSizeTable::merge(
            Mp4SampleSizeTable const & table)
        {
            std::vector<boost::uint32_t> & l(data_->table);
            std::vector<boost::uint32_t> const & r(table.data_->table);
            l.insert(l.end(), r.begin(), r.end());
            return true;
        }

        bool Mp4SampleSizeTable::next()
        {
            ++index_;
            chunk_->next(entry_);
            if (data_->sample_size == 0) {
                if (index_ < data_->table.size()) {
                    entry_ = data_->table[index_];
                } else {
                    return false;
                }
            }
            return true;
        }

        bool Mp4SampleSizeTable::seek(
            boost::uint32_t sample_index, 
            boost::uint32_t sample_index2)
        {
            if (data_->sample_size) {
                index_ = sample_index;
                chunk_->next(data_->sample_size * sample_index2);
                return true;
            }
            if (sample_index < data_->table.size()) {
                index_ = sample_index;
                entry_ = data_->table[index_];
                sample_index2 = std::accumulate(
                    data_->table.begin() + sample_index - sample_index2, 
                    data_->table.begin() + sample_index, 0);
                chunk_->next(sample_index2);
                return true;
            }
            return false;
        }

        bool Mp4SampleSizeTable::limit(
            boost::uint64_t & offset, 
            Mp4SampleToChunkBox::Entry  & index) const
        {
            if (data_->sample_size) {
                index.sample_description_index += (boost::uint32_t)(offset / data_->sample_size);
                offset %= data_->sample_size;
            } else {
                while (index.samples_per_chunk && data_->table[index.sample_description_index] <= offset) {
                    offset -= data_->table[index.sample_description_index];
                    --index.samples_per_chunk;
                    ++index.sample_description_index;
                }
            }
            return true;
        }

    } // namespace avformat
} // namespace just
