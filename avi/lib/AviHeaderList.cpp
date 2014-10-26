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
        {
            for (AviBoxVector::iterator iter = data_->begin(); iter != data_->end(); ++iter) {
                AviBox * cbox = *iter;
                switch ((*iter)->id()) {
                    case AviBoxType::avih:
                        avih_ = new AviHeader(*cbox);
                        break;
                    case AviBoxType::strl:
                        streams_.push_back(new AviStream(*cbox, index, boost::uint32_t(streams_.size())));
                        break;
                    default:
                        break;
                }
            }
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
