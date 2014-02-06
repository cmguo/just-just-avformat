// Mp4File.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/lib/Mp4File.h"
#include "ppbox/avformat/mp4/box/Mp4FileBox.h"
#include "ppbox/avformat/mp4/box/Mp4Box.hpp"
#include "ppbox/avformat/Error.h"

#include <util/buffers/NullBuffer.h>

namespace ppbox
{
    namespace avformat
    {

        Mp4File::Mp4File()
            : ftyp_(NULL)
            , moov_(NULL)
            , mdat_(NULL)
            , movie_(NULL)
        {
        }

        Mp4File::~Mp4File()
        {
            close();
        }

        bool Mp4File::open(
            Mp4BoxIArchive & ia, 
            boost::system::error_code & ec)
        {
            Mp4BoxContext ctx;
            ia.context(&ctx);
            Mp4Box * box(new Mp4Box);
            while (ia >> *box) {
                if (open(box, ec)) {
                    return true;
                }
                if (ec) {
                    return false;
                }
                if (box->type == Mp4BoxType::mdat) {
                    ia.seekg((std::streamoff)box->data_size(), std::ios::cur);
                    if (!ia) {
                        box = NULL;
                        break;
                    }
                }
                box = new Mp4Box;
            }
            if (box)
                delete box;
            ec = error::file_stream_error;
            return false;
        }

        bool Mp4File::open(
            Mp4Box * box, 
            boost::system::error_code & ec)
        {
            boxes_.push_back(box);
            switch (box->type) {
                case Mp4BoxType::ftyp:
                    ftyp_ = box;
                    ftyp_->as<Mp4FileTypeBox>();
                    break;
                case Mp4BoxType::moov:
                    moov_ = box;
                    movie_ = new Mp4Movie(*moov_);
                    break;
                case Mp4BoxType::mdat:
                    mdat_ = box;
                    mdat_->as<Mp4MediaDataBox>();
                    break;
            }
            ec.clear();
            if (ftyp_ && moov_ && mdat_)
                return true;
            return false;
        }

        void Mp4File::close(
            Mp4BoxOArchive & oa)
        {
            Mp4BoxContext ctx;
            oa.context(&ctx);
            for (size_t i = 0; i < boxes_.size(); ++i) {
                oa << *boxes_[i];
            }
            close();
        }

        void Mp4File::close()
        {
            if (movie_) {
                delete movie_;
                movie_ = NULL;
            }
            mdat_ = NULL;
            moov_ = NULL;
            ftyp_ = NULL;
            for (size_t i = 0; i < boxes_.size(); ++i) {
                delete boxes_[i];
            }
            boxes_.clear();
        }

        bool Mp4File::merge_begin(
            boost::system::error_code & ec)
        {
            movie_->shift(-(boost::int64_t)head_size());
            ec.clear();
            return true;
        }

        bool Mp4File::merge(
            Mp4File & file, 
            boost::system::error_code & ec)
        {
            file.movie_->shift(file.mdat_->data_size() - file.head_size());
            mdat_->data_size(mdat_->data_size() + file.mdat_->data_size());
            return movie_->merge(*file.movie_, ec);
        }

        bool Mp4File::merge_end(
            boost::system::error_code & ec)
        {
            util::buffers::NullBuffer<boost::uint8_t> buf;
            Mp4BoxOArchive oa(buf);
            Mp4BoxContext ctx;
            oa.context(&ctx);
            oa << *moov_;
            movie_->shift((boost::int64_t)head_size());
            ec.clear();
            return true;
        }

        boost::uint64_t Mp4File::head_size() const
        {
            boost::uint64_t n = 0;
            for (size_t i = 0; i < boxes_.size(); ++i) {
                if (boxes_[i]->type == Mp4BoxType::mdat) {
                    n += boxes_[i]->head_size();
                    break;
                } else {
                    n += boxes_[i]->byte_size();
                }
            }
            return n;
        }

    } // namespace avformat
} // namespace ppbox
