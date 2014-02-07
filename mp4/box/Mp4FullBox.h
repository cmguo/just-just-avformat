// Mp4FullBox.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_FULL_BOX_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_FULL_BOX_H_

#include "ppbox/avformat/mp4/box/Mp4Vector.h"
#include "ppbox/avformat/mp4/box/Mp4BoxData.h"
#include "ppbox/avformat/mp4/box/Mp4Box.h"

namespace ppbox
{
    namespace avformat
    {

        struct Mp4FullBoxHeader
        {
            union {
                struct {
#ifdef BOOST_BIG_ENDIAN
                    boost::uint32_t version : 8;
                    boost::uint32_t flags : 24;
#else
                    boost::uint32_t flags : 24;
                    boost::uint32_t version : 8;
#endif
                };
                boost::uint32_t dword;
            };

            Mp4FullBoxHeader()
                : dword(0)
            {
            }

            Mp4FullBoxHeader(
                boost::uint8_t v, 
                boost::uint32_t f)
                : dword(v << 24 | f)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & dword;
            }
        };

        template <
            typename T, 
            typename Mp4BoxHeader::id_type id
        >
        struct Mp4FullBoxData
            : Mp4BoxData<T, id>
            , Mp4FullBoxHeader
        {
        };


    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_FULL_BOX_H_
