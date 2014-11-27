// Mp4MediaBox.h

#ifndef _JUST_AVFORMAT_MP4_BOX_MP4_MEDIA_BOX_H_
#define _JUST_AVFORMAT_MP4_BOX_MP4_MEDIA_BOX_H_

#include "just/avformat/mp4/box/Mp4BoxEnum.h"
#include "just/avformat/mp4/box/Mp4FullBox.h"
#include "just/avformat/mp4/box/Mp4BoxVector.h"

namespace just
{
    namespace avformat
    {

        struct Mp4MediaBox
            : Mp4BoxData<Mp4MediaBox, Mp4BoxType::mdia>
            , Mp4BoxContainer<Mp4MediaBox, Mp4BoxType::mdia>
        {
        };

        struct Mp4MediaHeaderBox
            : Mp4FullBoxData<Mp4MediaHeaderBox, Mp4BoxType::mdhd>
        {
            boost::uint64_t creation_time;
            boost::uint64_t modification_time;
            boost::uint32_t timescale;
            boost::uint64_t duration;
            // bit(1) pad = 0;
            // unsigned int(5)[3] language;
//            union {
//                struct {
//#ifdef BOOST_BIG_ENDIAN
//                    boost::uint16_t language : 15;
//                    boost::uint16_t pad : 1;
//#else
//                    boost::uint16_t pad : 1;
//                    boost::uint16_t language : 15;
//#endif
//                };
//                boost::uint16_t word;
//            };
            char language[3];
            boost::uint16_t pre_defined; // = 0;

            Mp4MediaHeaderBox()
                : creation_time(0)
                , modification_time(0)
                , timescale(0)
                , duration(0)
                , pre_defined(0)
            {
                language[0] = language[1] = language[2] = 0;
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4FullBoxHeader::serialize(ar);
                if (version == 1) {
                    ar & creation_time
                        & modification_time
                        & timescale
                        & duration;
                } else {
                    boost::uint32_t creation_time32 = (boost::uint32_t)creation_time;
                    boost::uint32_t modification_time32 = (boost::uint32_t)modification_time;
                    boost::uint32_t duration32 = (boost::uint32_t)duration;
                    ar & creation_time32
                        & modification_time32
                        & timescale
                        & duration32;
                    creation_time = creation_time32;
                    modification_time = modification_time32;
                    duration = duration32;
                }
                boost::uint16_t word = 0;
                for (int i = 0; i < 3; ++i) {
                    word = (word << 5) | ((language[i] - 0x60) & 0x1f);
                }
                ar & word;
                for (int i = 0; i < 3; ++i) {
                    language[2 - i] = (word & 0x1f) + 0x60;
                    word >>= 5;
                }
                ar & pre_defined;
            }
        };

        struct Mp4HandlerBox
            : Mp4FullBoxData<Mp4HandlerBox, Mp4BoxType::hdlr>
        {
            boost::uint32_t pre_defined;// = 0;
            union {
                boost::uint32_t handler_type;
                char handler_type_str[4];
            };
            boost::uint32_t reserved[3]; // = 0;
            std::string name;

            Mp4HandlerBox()
                : pre_defined(0)
                , handler_type(0)
            {
                reserved[0] = reserved[1] = reserved[2] = 0;
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4FullBoxHeader::serialize(ar);
                ar & pre_defined
                    & make_array(handler_type_str)
                    & make_array(reserved);
                if (Archive::is_saving::value) {
                    ar & make_array(name.c_str(), name.size() + 1);
                } else {
                    Mp4BoxContext * ctx = (Mp4BoxContext *)ar.context();
                    size_t n = (size_t)ctx->stack.back()->data_size() - 24;
                    name.resize(n);
                    ar & make_array(name.c_str(), name.size());
                    name.erase(name.size() - 1);
                }
            }
        };

        struct Mp4MediaInformationBox
            : Mp4BoxData<Mp4MediaInformationBox, Mp4BoxType::minf>
            , Mp4BoxContainer<Mp4MediaInformationBox, Mp4BoxType::minf>
        {
        };

        struct Mp4VideoMediaHeaderBox
            : Mp4FullBoxData<Mp4VideoMediaHeaderBox, Mp4BoxType::vmhd>
        {
            boost::uint16_t graphicsmode; // = 0; // copy, see below
            boost::uint16_t opcolor[3]; //  = {0, 0, 0};

            Mp4VideoMediaHeaderBox()
                : graphicsmode(0)
            {
                // version = 0, 1
                opcolor[0] = opcolor[1] = opcolor[2] = 0;
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4FullBoxHeader::serialize(ar);
                ar & graphicsmode
                    & make_array(opcolor);
            }
        };

        struct Mp4SoundMediaHeaderBox
            : Mp4FullBoxData<Mp4SoundMediaHeaderBox, Mp4BoxType::smhd>
        {
            boost::int16_t balance; // = 0;
            boost::uint16_t reserved; // = 0;

            Mp4SoundMediaHeaderBox()
                : balance(0)
                , reserved(0)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4FullBoxHeader::serialize(ar);
                ar & balance
                    & reserved;
            }
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MP4_BOX_MP4_MEDIA_BOX_H_
