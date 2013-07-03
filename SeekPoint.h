// SeekPoint.h

#ifndef _PPBOX_AVFORMAT_SEEK_POINT_H_
#define _PPBOX_AVFORMAT_SEEK_POINT_H_

namespace ppbox
{
    namespace avformat
    {

        struct SeekPoint
        {
            boost::uint64_t time;
            boost::uint64_t offset;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_SEEK_POINT_H_
