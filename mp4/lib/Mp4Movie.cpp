// Mp4Movie.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/lib/Mp4Movie.h"
#include "ppbox/avformat/mp4/box/Mp4Box.hpp"

namespace ppbox
{
    namespace avformat
    {

        Mp4Movie::Mp4Movie(
            Mp4Box & box)
            : Mp4BoxWrapper2<Mp4MovieBox>(box)
            , header_(NULL)
        {
            boost::system::error_code ec;
            parse(ec);
        }

        bool Mp4Movie::parse(
            boost::system::error_code & ec)
        {
            header_ = find_item_as<Mp4MovieHeaderBox>("/mvhd");
            Mp4MovieBox & moov(data());
            for (Mp4BoxVector::iterator iter = moov.begin(); iter != moov.end(); ++iter) {
                if (iter->type == Mp4BoxType::trak) {
                    Mp4Track * track = new Mp4Track(*iter);
                    tracks_.push_back(track);
                }
            }
            ec.clear();
            return true;
        }

    } // namespace avformat
} // namespace ppbox
