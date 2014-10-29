// AviIndex.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/avi/lib/AviIndex.h"
#include "ppbox/avformat/avi/box/AviBox.hpp"
#include "ppbox/avformat/avi/box/AviBoxVector.hpp"

namespace ppbox
{
    namespace avformat
    {

        AviIndex::AviIndex(
            AviBox & box, 
            boost::uint64_t movie_offset)
            : AviBoxWrapper<AviIndexBox>(box)
            , movie_offset_(movie_offset + 8)
        {
        }

        void AviIndex::set_movie_offset(
            boost::uint64_t movie_offset)
        {
            movie_offset_ = movie_offset + 8;
        }

        bool AviIndex::put(
            boost::uint32_t stream, 
            ppbox::avbase::Sample const & sample)
        {
            AviIndexBox::Entry entry;
            entry.dwChunkId = stream;
            entry.dwFlags = 0;
            if (sample.flags & sample.f_sync) {
                entry.dwFlags |= AviIndexFlags::KeyFrame;
            }
            entry.dwOffset = sample.time - movie_offset_;
            entry.dwSize = sample.size - 8;
            data_->table.push_back(entry);
            return true;
        }

        bool AviIndex::merge(
            AviIndex const & track)
        {
            return false;
        }

        void AviIndex::shift(
            boost::int64_t offset)
        {
        }

    bool AviIndex::next(
            boost::uint32_t & index) const
        {
            boost::uint32_t stream = data_->table[index].dwChunkId;
            for (++index; index < data_->table.size(); ++index) {
                if (stream == data_->table[index].dwChunkId)
                    return true;
            }
            return false;
        }

        static bool less_offset(
            AviIndexBox::Entry const & l, 
            AviIndexBox::Entry const & r)
        {
            return l.dwOffset < r.dwOffset;
        }

        bool AviIndex::limit(
            boost::uint64_t offset, 
            boost::uint32_t & index) const
        {
            AviIndexBox::Entry entry = {{0}, 0, offset, 0};
            std::vector<AviIndexBox::Entry>::const_iterator iter = 
                std::lower_bound(data_->table.begin(), data_->table.end(), entry, less_offset);
            if (iter == data_->table.end()) {
                index = 0;
            } else {
                index = (boost::uint32_t)(iter - data_->table.begin());
            }
            return true;
        }

        void AviIndex::get(
            boost::uint32_t index, 
            ppbox::avbase::Sample & sample) const
        {
            AviIndexBox::Entry const & entry(data_->table[index]);
            sample.flags = (entry.dwFlags & AviIndexFlags::KeyFrame) ? sample.f_sync : 0;
            sample.time = movie_offset_ + entry.dwOffset; // use time for offset
            sample.size = entry.dwSize + 8;
        }

        AviIndexStream::AviIndexStream(
            AviIndex & index, 
            boost::uint32_t stream)
            : avi_index_(index)
            , all_sync_(true)
        {
            std::vector<AviIndexBox::Entry> const & table(avi_index_.table());
            boost::uint32_t index1 = 0;
            for (size_t i = 0; i < table.size(); ++i) {
                AviIndexBox::Entry const & entry(table[i]);
                if (entry.dwChunkId == stream) {
                    if ((entry.dwFlags & AviIndexFlags::KeyFrame)) {
                        if (!all_sync_ || (index1 & 0xf) == 0) {
                            MapItem m = {index1, (boost::uint32_t)i};
                            index_map_.push_back(m);
                        }
                    } else {
                        all_sync_ = false;
                    }
                    ++index1;
                }
            }
            index1_ = 0;
            index2_ = index_map_[0].index2;
        }

        AviIndexStream::AviIndexStream(
            AviIndex & index, 
            boost::uint32_t stream, 
            void *)
            : avi_index_(index)
            , all_sync_(true)
        {
            index1_ = 0;
            index2_ = stream;
        }

        bool AviIndexStream::put(
            ppbox::avbase::Sample const & sample)
        {
            return avi_index_.put(index2_, sample);
        }

        bool AviIndexStream::next()
        {
            ++index1_;
            return avi_index_.next(index2_);
        }

        static bool less_index1(
            AviIndexStream::MapItem const & l, 
            AviIndexStream::MapItem const & r)
        {
            return l.index1 < r.index1;
        }

        bool AviIndexStream::seek(
            boost::uint32_t & index)
        {
            MapItem item = {index, 0};
            std::vector<MapItem>::const_iterator iter = 
                std::lower_bound(index_map_.begin(), index_map_.end(), item, less_index1);
            if (all_sync_) {
                item = *iter;
                while (item.index1 < index) {
                    if (!avi_index_.next(item.index2))
                        return false;
                    ++item.index1;
                }
                index1_ = index;
                index2_ = item.index2;
            } else {
                index = index1_ = iter->index1;
                index2_ = iter->index2;
            }
            return true;
        }

        void AviIndexStream::rewind()
        {
            index1_ = 0;
            index2_ = index_map_[0].index2;
        }

        static bool less_index2(
            AviIndexStream::MapItem const & l, 
            AviIndexStream::MapItem const & r)
        {
            return l.index2 < r.index2;
        }

        bool AviIndexStream::limit(
            boost::uint64_t offset, 
            boost::uint32_t & index)
        {
            MapItem item = {0, 0};
            if (!avi_index_.limit(offset, item.index2)) {
                return false;
            }
            std::vector<MapItem>::const_iterator iter = 
                std::lower_bound(index_map_.begin(), index_map_.end(), item, less_index2);
            index = iter->index1;
            item.index1 = iter->index2;
            while (item.index1 < item.index2) {
                if (!avi_index_.next(item.index1))
                    return false;
                ++index;
            }
            return true;
        }

        void AviIndexStream::get(
            ppbox::avbase::Sample & sample) const
        {
            avi_index_.get(index2_, sample);
            sample.dts = index1_;
            //std::cout << "sample track = " << sample.itrack 
            //    << ", offset = " << sample.time
            //    << ", size = " << sample.size 
            //    << std::endl;
        }

    } // namespace avformat
} // namespace ppbox
