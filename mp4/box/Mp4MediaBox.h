// Mp4MediaBox.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_MEDIA_BOX_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_MEDIA_BOX_H_

#include "ppbox/avformat/mp4/box/Mp4BoxEnum.h"
#include "ppbox/avformat/mp4/box/Mp4FullBox.h"
#include "ppbox/avformat/mp4/box/Mp4BoxVector.h"

namespace ppbox
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
            union {
                struct {
#ifdef BOOST_BIG_ENDIAN
                    boost::uint16_t language : 15;
                    boost::uint16_t pad : 1;
#else
                    boost::uint16_t pad : 1;
                    boost::uint16_t language : 15;
#endif
                };
                boost::uint16_t word;
            };
            boost::uint16_t pre_defined; // = 0;

            Mp4MediaHeaderBox()
                : creation_time(0)
                , modification_time(0)
                , timescale(0)
                , duration(0)
                , word(0)
                , pre_defined(0)
            {
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
                ar & word;
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
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_MEDIA_BOX_H_
