// AviFile.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/avi/lib/AviFile.h"
#include "ppbox/avformat/avi/box/AviMovieBox.h"
#include "ppbox/avformat/avi/box/AviBox.hpp"
#include "ppbox/avformat/avi/box/AviBoxVector.hpp"
#include "ppbox/avformat/Error.h"

namespace ppbox
{
    namespace avformat
    {

        AviFile::AviFile()
            : AviBoxWrapper2<AviFileBox>(NULL)
            , hdrl_(NULL)
            , movi_(NULL)
            , idx1_(NULL)
        {
        }

        AviFile::~AviFile()
        {
            close();
        }

        bool AviFile::open(
            AviBox * box, 
            boost::system::error_code & ec)
        {
            if (box_ == NULL) {
                assign(new AviBox(AviFileBox()));
                box_->type = 0;// RIFF
            }
            if (box)
                data_->push_back(box);

            boost::uint64_t byte_size = box_->head_size();
            AviBox * hdrl = NULL;
            AviBox * movi = NULL;
            AviBox * idx1 = NULL;
            for (AviBoxVector::iterator iter = data_->begin(); iter != data_->end(); ++iter) {
                AviBox * cbox = *iter;
                switch ((*iter)->id()) {
                    case AviBoxType::hdrl:
                        hdrl = cbox;
                        break;
                    case AviBoxType::movi:
                        cbox->is<AviMovieBox>();
                        movi = cbox;
                        break;
                    case AviBoxType::idx1:
                        idx1 = cbox;
                        break;
                    default:
                        break;
                }
                if (!movi)
                    byte_size += (*iter)->byte_size();
            }
            if (hdrl && movi && idx1) {
                idx1_ = new AviIndex(*idx1, byte_size);
                hdrl_ = new AviHeaderList(*hdrl, *idx1_);
                movi_ = movi;
                return true;
            }
            // return false, but no error
            // ec = error::file_stream_error;
            return false;
        }

        bool AviFile::create(
            boost::system::error_code & ec)
        {
            assign(new AviBox(AviFileBox()));
            //avih_ = new AviHeader(*create_item("/avih"));
            movi_ = create_item("/movi");
            ec.clear();
            return true;
        }

        void AviFile::close()
        {
            movi_ = NULL;
            idx1_ = NULL;
            if (hdrl_) {
                delete hdrl_;
                hdrl_ = NULL;
            }
            if (box_) {
                delete box_;
                assign(NULL);
            }
        }

        bool AviFile::load(
            AviBoxIArchive & ia, 
            boost::system::error_code & ec)
        {
            AviBoxContext ctx;
            ia.context(&ctx);
            AviBox * box(new AviBox);
            if (ia >> *box) {
                assign(box);
                return open(NULL, ec);
            } else {
                delete box;
                ec = error::file_stream_error;
                return false;
            }
        }

        void AviFile::save(
            AviBoxOArchive & oa)
        {
            AviBoxContext ctx;
            oa.context(&ctx);
            oa << *box_;
        }

        bool AviFile::merge_begin(
            boost::system::error_code & ec)
        {
            return true;
        }

        bool AviFile::merge(
            AviFile & file, 
            boost::system::error_code & ec)
        {
            return false;
        }

        bool AviFile::merge_end(
            boost::system::error_code & ec)
        {
            return true;
        }

    } // namespace avformat
} // namespace ppbox
