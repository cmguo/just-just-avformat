// Mp4SampleData.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/lib/Mp4SampleData.h"

namespace ppbox
{
    namespace avformat
    {

        // Mp4SampleToChunkTable

        Mp4SampleToChunkTable::Mp4SampleToChunkTable(
            Mp4Box * box, 
            Mp4Box * box2)
            : Mp4BoxWrapper<Mp4SampleToChunkBox>(box)
            , box2_(&box2->as<Mp4ChunkOffsetBox>())
            , index_(0)
            , first_chunk_(0)
        {
            entry_ = data_->table[index_];
            if (index_ + 1 < data_->table.size()) {
                first_chunk_ = data_->table[index_ + 1].first_chunk;
            }
            offset_ = box2_->table[entry_.first_chunk - 1];
            samples_per_chunk_ = entry_.samples_per_chunk;
        }

        bool Mp4SampleToChunkTable::next(
            boost::uint32_t size)
        {
            offset_ += size;
            if (--samples_per_chunk_ == 0) {
                if (++entry_.first_chunk == first_chunk_) {
                    if (++index_ < data_->table.size()) {
                        entry_ = data_->table[index_];
                        if (index_ + 1 < data_->table.size()) {
                            first_chunk_ = data_->table[index_ + 1].first_chunk;
                        }
                    }
                }
                if (entry_.first_chunk <= box2_->table.size()) {
                    offset_ = box2_->table[entry_.first_chunk - 1];
                } else {
                    return false;
                }
                samples_per_chunk_ = entry_.samples_per_chunk;
            }
            return true;
        }

        bool Mp4SampleToChunkTable::seek(
            boost::uint32_t sample_index)
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
                    return false;
                }
            }
            entry.first_chunk += sample_index / entry.samples_per_chunk;
            if (entry.first_chunk > box2_->table.size()) {
                return false;
            }
            entry_ =entry;
            index_ = table_index;
            samples_per_chunk_ = entry.samples_per_chunk - (sample_index % entry.samples_per_chunk);
            first_chunk_ = first_chunk;
            offset_ = box2_->table[entry_.first_chunk - 1];
            return true;
        }

        // Mp4SampleSizeTable

        Mp4SampleSizeTable::Mp4SampleSizeTable(
            Mp4Box * box)
            : Mp4BoxWrapper<Mp4SampleSizeBox>(box)
            , index_(0)
        {
            entry_ = data_->sample_size 
                ? data_->sample_size : data_->table[index_];
        }

        bool Mp4SampleSizeTable::next()
        {
            ++index_;
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
            boost::uint32_t sample_index)
        {
            if (data_->sample_size) {
                index_ = sample_index;
                return true;
            }
            if (sample_index < data_->table.size()) {
                index_ = sample_index;
                entry_ = data_->table[index_];
                return true;
            }
            return false;
        }

    } // namespace avformat
} // namespace ppbox
