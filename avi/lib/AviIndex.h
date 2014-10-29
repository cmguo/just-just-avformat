// AviIndex.h

#ifndef _PPBOX_AVFORMAT_AVI_LIB_AVI_INDEX_H_
#define _PPBOX_AVFORMAT_AVI_LIB_AVI_INDEX_H_

#include "ppbox/avformat/avi/lib/AviBoxWrapper.h"
#include "ppbox/avformat/avi/box/AviIndexBox.h"

#include <ppbox/avbase/Sample.h>

namespace ppbox
{
    namespace avformat
    {

        class AviIndex
            : public AviBoxWrapper<AviIndexBox>
        {
        public:
            AviIndex(
                AviBox & box, 
                boost::uint64_t movie_offset);

        public:
            void set_movie_offset(
                boost::uint64_t movie_offset);

            bool put(
                boost::uint32_t stream,
                ppbox::avbase::Sample const & sample);

        public:
            bool merge(
                AviIndex const & track);

            void shift(
                boost::int64_t offset);

        public:
            // next in same stream
            bool next(
                boost::uint32_t & index) const;

            bool limit(
                boost::uint64_t offset, 
                boost::uint32_t & index) const;

        public:
            void get(
                boost::uint32_t index, 
                ppbox::avbase::Sample & sample) const;

        public:
            std::vector<AviIndexBox::Entry> const & table() const
            {
                return data_->table;
            }

        private:
            boost::uint64_t movie_offset_;
        };

        class AviIndexStream
        {
        public:
            AviIndexStream(
                AviIndex & index, 
                boost::uint32_t stream);

            AviIndexStream(
                AviIndex & index, 
                boost::uint32_t stream, 
                void *); // create_new

        public:
            bool put(
                ppbox::avbase::Sample const & sample);

        public:
            bool next();

            bool seek(
                boost::uint32_t & index);

            void rewind();

            bool limit(
                boost::uint64_t offset, 
                boost::uint32_t & index);

        public:
            void get(
                ppbox::avbase::Sample & sample) const;

        public:
            struct MapItem
            {
                boost::uint32_t index1;
                boost::uint32_t index2;
            };

        private:
            AviIndex & avi_index_;
            std::vector<MapItem> index_map_;
            bool all_sync_;
            boost::uint32_t index1_;
            boost::uint32_t index2_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_LIB_AVI_INDEX_H_
