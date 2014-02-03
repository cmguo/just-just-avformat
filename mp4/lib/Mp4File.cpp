// Mp4File.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/lib/Mp4File.h"
#include "ppbox/avformat/mp4/box/Mp4Box.hpp"

namespace ppbox
{
    namespace avformat
    {

        Mp4File::Mp4File()
            : ftyp(Mp4BoxType::ftyp)
            , moov(Mp4BoxType::moov)
            , mdat(Mp4BoxType::mdat)
            , movie_(moov)
        {
            ftyp.as<Mp4FileTypeBox>();
            mdat.as<Mp4MediaDataBox>();
        }

        bool Mp4File::parse(
            boost::system::error_code & ec)
        {
            return movie_.parse(ec);
        }

    } // namespace avformat
} // namespace ppbox
