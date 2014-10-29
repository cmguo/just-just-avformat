// AviStream.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/avi/lib/AviStream.h"
#include "ppbox/avformat/avi/lib/AviIndex.h"
#include "ppbox/avformat/avi/box/AviBox.hpp"
#include "ppbox/avformat/avi/box/AviBoxVector.hpp"

namespace ppbox
{
    namespace avformat
    {

        AviStream::AviStream(
            AviBox & box, 
            AviIndex & avi_index, 
            boost::uint32_t index)
            : AviBoxWrapper2<AviStreamBox>(box)
            , strh_(find_item_as<AviStreamHeaderBox>("/strh"))
            , strf_(find_item_as<AviStreamFormatBox>("/strf"))
            , index_(NULL)
        {
            index_ = new AviIndexStream(avi_index, id(index));
        }

        AviStream::AviStream(
            AviBox & box, 
            AviIndex & avi_index, 
            boost::uint32_t index, 
            boost::uint32_t type)
            : AviBoxWrapper2<AviStreamBox>(box)
            , strh_(create_item_as<AviStreamHeaderBox>("/strh"))
            , strf_(create_item_as<AviStreamFormatBox>("/strf"))
            , index_(NULL)
        {
            strh_->dwType = type;
            index_ = new AviIndexStream(avi_index, id(index), NULL);
        }

        bool AviStream::put(
            ppbox::avbase::Sample const & sample)
        {
            return index_->put(sample);
        }

        bool AviStream::put_eos()
        {
            return true;
        }

        bool AviStream::fixup(
            boost::system::error_code & ec)
        {
            if (strh_->dwType == AviStreamType::vids) {
                strh_->dwHandler = strf_->video.biCompression;
            } else if (strh_->dwType == AviStreamType::auds) {
                //strh_->dwHandler = ???;
                strh_->dwSampleSize = strf_->audio.nBlockAlign;
            }
            return true;
        }


        bool AviStream::merge(
            AviStream const & track, 
            boost::system::error_code & ec)
        {
            return false;
        }

        bool AviStream::next(
            boost::system::error_code & ec)
        {
            if (index_->next()) {
                ec.clear();
                return true;
            } else {
                ec = framework::system::logic_error::out_of_range;
                return false;
            }
        }

        bool AviStream::seek(
            boost::uint64_t & time, // dts
            boost::system::error_code & ec)
        {
            boost::uint32_t index = time / strh_->dwScale;
            if (index_->seek(index)) {
                time =  strh_->dwScale * index;
                ec.clear();
                return true;
            } else {
                ec = framework::system::logic_error::out_of_range;
                return false;
            }
        }

        void AviStream::rewind()
        {
            index_->rewind();
        }

        bool AviStream::limit(
            boost::uint64_t offset, 
            boost::uint64_t & time, // dts
            boost::system::error_code & ec) const
        {
            boost::uint32_t index = 0;
            if (index_->limit(offset, index)) {
                time =  strh_->dwScale * index;
                ec.clear();
                return true;
            } else {
                ec = framework::system::logic_error::out_of_range;
                return false;
            }
        }

        void AviStream::get(
            ppbox::avbase::Sample & sample) const
        {
            index_->get(sample);
            sample.dts = strh_->dwScale * sample.dts;
            sample.duration = strh_->dwScale;
            sample.time += 8; // offset
            sample.size -= 8;
        }

        boost::uint32_t AviStream::id(
             boost::uint32_t index) const
        {
            AviIndexBox::Entry entry;
            entry.fccChunkId[0] = boost::uint8_t((index >> 8) & 0xff) + '0';
            entry.fccChunkId[1] = boost::uint8_t(index & 0xff) + '0';
            switch (strh_->dwType) {
                case AviStreamType::vids:
                    entry.fccChunkId[2] = 'd';
                    entry.fccChunkId[3] = 'c';
                    break;
                case AviStreamType::auds:
                    entry.fccChunkId[2] = 'w';
                    entry.fccChunkId[3] = 'b';
                    break;
                default:
                    break;
            }
            return entry.dwChunkId;
        }

        boost::uint64_t AviStream::duration() const
        {
            return (boost::uint64_t)strh_->dwScale * strh_->dwLength;
        }

        void AviStream::handler(
            boost::uint32_t n)
        {
            strh_->dwHandler = n;
        }

        void AviStream::sample_duration(
            boost::uint32_t n)
        {
            strh_->dwScale = n;
        }

        void AviStream::timescale(
            boost::uint32_t n)
        {
            strh_->dwRate = n;
        }

        void AviStream::duration(
            boost::uint64_t n)
        {
            strh_->dwLength = (n / strh_->dwScale);
        }

    } // namespace avformat
} // namespace ppbox
