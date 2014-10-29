// AviHeaderList.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/avi/lib/AviHeaderList.h"
#include "ppbox/avformat/avi/box/AviBox.hpp"
#include "ppbox/avformat/avi/box/AviBoxVector.hpp"
#include "ppbox/avformat/Error.h"

namespace ppbox
{
    namespace avformat
    {

        AviHeaderList::AviHeaderList(
            AviBox & box, 
            AviIndex & index)
            : AviBoxWrapper2<AviHeaderListBox>(box)
            , index_(&index)
        {
            for (AviBoxVector::iterator iter = data_->begin(); iter != data_->end(); ++iter) {
                AviBox * cbox = *iter;
                switch ((*iter)->id()) {
                    case AviBoxType::avih:
                        avih_ = &cbox->as<AviHeaderBox>();
                        break;
                    case AviBoxType::strl:
                        streams_.push_back(new AviStream(*cbox, index, boost::uint32_t(streams_.size())));
                        break;
                    default:
                        break;
                }
            }
        }

        AviHeaderList::AviHeaderList(
            AviBox & box, 
            AviIndex & index, 
            create_new_t create_new)
            : AviBoxWrapper2<AviHeaderListBox>(box)
            , index_(&index)
        {
            avih_ = create_item_as<AviHeaderBox>("/avih");
            avih_->dwFlags = AviHeaderFlags::HasIndex | AviHeaderFlags::IsInterleaved | AviHeaderFlags::MustUseIndex;
        }

        AviStream * AviHeaderList::create_stream(
            boost::uint32_t type)
        {
            AviStream * stream = new AviStream(*create_item("/strl"), *index_, avih_->dwStreams++, type);
            streams_.push_back(stream);
            return stream;
        }

        bool AviHeaderList::fixup(
            boost::system::error_code & ec)
        {
            for (size_t i = 0; i < streams_.size(); ++i) {
                if (!streams_[i]->type() == AviStreamType::vids) {
                    avih_->dwWidth = streams_[i]->video().biWidth;
                    avih_->dwHeight = streams_[i]->video().biHeight;
                    break;
                }
            }
            return true;
        }

        bool AviHeaderList::merge_begin(
            boost::system::error_code & ec)
        {
            return true;
        }

        bool AviHeaderList::merge(
            AviHeaderList & hdrl, 
            boost::system::error_code & ec)
        {
            return false;
        }

        bool AviHeaderList::merge_end(
            boost::system::error_code & ec)
        {
            return true;
        }

    } // namespace avformat
} // namespace ppbox
