// Mp4SampleData.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/lib/Mp4SampleData.h"
#include "ppbox/avformat/mp4/box/Mp4Box.hpp"

#include <algorithm>
#include <numeric>

namespace ppbox
{
    namespace avformat
    {

        // Mp4ChunkOffseTable

        Mp4ChunkOffseTable::Mp4ChunkOffseTable(
            Mp4Box * box)
            : Mp4BoxWrapper<Mp4ChunkOffsetBox>(box)
            , index_(0)
        {
            offset_ = data_->table[index_];
        }

        bool Mp4ChunkOffseTable::next()
        {
            ++index_;
            if (index_ >= data_->table.size()) {
                return false;
            }
            offset_ = data_->table[index_];
            return true;
        }

        bool Mp4ChunkOffseTable::next(
            boost::uint32_t size)
        {
            offset_ += size;
            return true;
        }

        // seek to chunk
        bool Mp4ChunkOffseTable::seek(
            boost::uint32_t chunk_index)
        {
            index_ = chunk_index;
            if (index_ >= data_->table.size()) {
                return false;
            }
            offset_ = data_->table[index_];
            return true;
        }

        bool Mp4ChunkOffseTable::limit(
            boost::uint64_t & offset, 
            Mp4SampleToChunkBox::Entry  & index) const
        {
            std::vector<boost::uint32_t>::iterator iter = 
                std::upper_bound(data_->table.begin(), data_->table.end(), offset);
            index.first_chunk = std::distance(data_->table.begin(), iter);
            if (index.first_chunk == 0) {
                offset = 0;
            } else {
                --index.first_chunk;
                offset -= data_->table[index.first_chunk];
            }
            return true;
        }

        // Mp4SampleToChunkTable

        Mp4SampleToChunkTable::Mp4SampleToChunkTable(
            Mp4Box * box, 
            Mp4ChunkOffseTable * chunk)
            : Mp4BoxWrapper<Mp4SampleToChunkBox>(box)
            , chunk_(chunk)
            , index_(0)
            , first_chunk_(0)
        {
            entry_ = data_->table[index_];
            if (index_ + 1 < data_->table.size()) {
                first_chunk_ = data_->table[index_ + 1].first_chunk;
            }
            chunk_->seek(entry_.first_chunk - 1);
            samples_per_chunk_ = entry_.samples_per_chunk;
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
            while (entry.samples_per_chunk * (first_chunk - entry.first_chunk) < sample_index) {
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
            entry_ =entry;
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
            Mp4SampleToChunkBox::Entry  & index) const
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
            Mp4ChunkOffseTable * chunk)
            : Mp4BoxWrapper<Mp4SampleSizeBox>(box)
            , chunk_(chunk)
            , index_(0)
        {
            entry_ = data_->sample_size 
                ? data_->sample_size : data_->table[index_];
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
} // namespace ppbox
