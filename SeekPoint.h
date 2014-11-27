// SeekPoint.h

#ifndef _JUST_AVFORMAT_SEEK_POINT_H_
#define _JUST_AVFORMAT_SEEK_POINT_H_

namespace just
{
    namespace avformat
    {

        struct SeekPoint
        {
            boost::uint64_t time;
            boost::uint64_t offset;
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_SEEK_POINT_H_
