// Mp4Descriptor.h

#ifndef _PPBOX_AVFORMAT_MP4_MP4_DESCRIPTOR_H_
#define _PPBOX_AVFORMAT_MP4_MP4_DESCRIPTOR_H_

#include "ppbox/avformat/mp4/box/Mp4BoxTraits.h"
#include "ppbox/avformat/mp4/box/Mp4Vector.h"

#include <ppbox/avbase/object/ObjectData.h>

#include <util/serialization/Serialization.h>
#include <util/serialization/SplitMember.h>
#include <util/serialization/NumberBits24.h>
#include <util/serialization/stl/string.h>

namespace ppbox
{
    namespace avformat
    {

        struct Mp4DescriptorType
        {
            enum Enum {
                ES                      = 0x03, 
                DECODER_CONFIG          = 0x04, 
                DECODER_SPECIFIC_INFO   = 0x05, 
                ES_ID_INC               = 0x0E, 
                ES_ID_REF               = 0x0F, 
            };
        };

        struct Mp4DescriptorHeader
        {
            typedef boost::uint8_t id_type;
            typedef boost::uint32_t size_type;

            boost::uint8_t Tag;
            boost::uint32_t Size;

            boost::uint8_t id() const
            {
                return Tag;
            }

            void id(
                boost::uint8_t id)
            {
                Tag = id;
            }

            boost::uint32_t head_size() const
            {
                boost::uint32_t n = 0;
                boost::uint32_t s = Size >> 7;
                while (s) {
                    ++n;
                    s >>= 7;
                }
                return 2 + n;
            }

            boost::uint32_t data_size() const
            {
                return Size;
            }

            void data_size(
                boost::uint32_t size)
            {
                Size = size;
            }

            SERIALIZATION_SPLIT_MEMBER();

            template <
                typename Archive
            >
            void save(
                Archive & ar) const
            {
                ar & Tag;
                boost::uint32_t n = Size;
                boost::uint8_t n1[4];
                size_t i = 0;
                while (true) {
                    boost::uint8_t c = n & 0x7f;
                    n >>= 7;
                    if (i) {
                        c |= 0x80;
                    }
                    n1[3 - i] = c;
                    ++i;
                    if (n == 0 || i == 4) {
                        break;
                    }
                }
                ar & make_array(n1 + 4 - i, i);
            }

            template <
                typename Archive
            >
            void load(
                Archive & ar)
            {
                ar & Tag;
                boost::uint8_t c = 0x80;
                size_t i = 0;
                Size = 0;
                while (c & 0x80 && i < 4) {
                    ar & c;
                    Size = (Size << 7) | (c & 0x7f);
                    ++i;
                }
            }
        };

        class Mp4DescriptorDataHelper;

        struct Mp4DescriptorTraits
            : Mp4BoxTraits
        {
            typedef Mp4DescriptorHeader header_type;

            static size_t const max_size = 64;

            typedef ppbox::avbase::ObjectHelper<Mp4DescriptorTraits> helper_t;

            typedef Mp4DescriptorDataHelper data_helper_t;
        };

        class Mp4DescriptorDataHelper
            : ppbox::avbase::ObjectDataHelper<Mp4DescriptorTraits>
        {
        public:
            Mp4DescriptorDataHelper(
                Mp4DescriptorTraits::helper_t & h)
                : ppbox::avbase::ObjectDataHelper<Mp4DescriptorTraits>(h)
                , ctx_((Mp4BoxContext *)h.context())
            {
                static char const * const descriptor_str[] = {
                    "", "", "", 
                    "ES", 
                    "DECODER_CONFIG", 
                    "DECODER_SPECIFIC_INFO", 
                    "", "", "", "", "", "", "", "", 
                    "ES_ID_INC", 
                    "ES_ID_REF", 
                };
                ctx_->path.append("/[");
                ctx_->path += descriptor_str[h.header().Tag];
                ctx_->path.append(1, ']');
                ctx_->stack.push_back(NULL);
                ctx_->data_ends.push_back(h.data_end());
            }

            ~Mp4DescriptorDataHelper()
            {
                ctx_->data_ends.pop_back();
                ctx_->stack.pop_back();
                ctx_->path.erase(ctx_->path.find_last_of("/"));
            }

        private:
            Mp4BoxContext * ctx_;
        };

        typedef ppbox::avbase::Object<Mp4DescriptorTraits> Mp4Descriptor;

        template <
            typename T, 
            Mp4DescriptorHeader::id_type id
        >
        struct Mp4DescriptorData
            : ppbox::avbase::ObjectData<Mp4DescriptorTraits, T, id>
        {
        };

        struct Mp4EsDescriptor
            : Mp4DescriptorData<Mp4EsDescriptor, Mp4DescriptorType::ES>
        {
            enum {
                f_stream_dependency = 1, 
                f_url               = 2, 
                f_ocr_stream        = 4, 
            };
            boost::uint16_t ES_ID;
            union {
                struct {
#ifdef BOOST_BIG_ENDIAN
                    boost::uint8_t Flags : 3;
                    boost::uint8_t StreamPriority : 5;
#else
                    boost::uint8_t StreamPriority : 5;
                    boost::uint8_t Flags : 3;
#endif
                };
                boost::uint8_t byte;
            };
            boost::uint16_t DependsOn;
            std::string Url;
            boost::uint16_t OcrEsId;
            Mp4Vector<Mp4Descriptor> sub_descriptors;

            Mp4EsDescriptor()
                : byte(0)
                , DependsOn(0)
                , OcrEsId(0)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & ES_ID
                    & byte;
                if (Flags & f_stream_dependency) {
                    ar & DependsOn;
                }
                if (Flags & f_url) {
                    ar & util::serialization::make_sized<boost::uint8_t>(Url);
                }
                if (Flags & f_ocr_stream) {
                    ar & DependsOn;
                }
                ar & sub_descriptors;
            }
        };

        struct Mp4EsIdIncDescriptor
            : Mp4DescriptorData<Mp4EsIdIncDescriptor, Mp4DescriptorType::ES_ID_INC>
        {
            boost::uint32_t TrackId;

            Mp4EsIdIncDescriptor()
                : TrackId(0)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & TrackId
;            }
        };

        struct Mp4EsIdRefDescriptor
            : Mp4DescriptorData<Mp4EsIdRefDescriptor, Mp4DescriptorType::ES_ID_REF>
        {
            boost::uint16_t RefIndex;

            Mp4EsIdRefDescriptor()
                : RefIndex(0)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & RefIndex;
            }
        };

        struct Mp4DecoderConfigDescriptor
            : Mp4DescriptorData<Mp4DecoderConfigDescriptor, Mp4DescriptorType::DECODER_CONFIG>
        {
            boost::uint8_t ObjectTypeIndication;
            union {
                struct {
#ifdef BOOST_BIG_ENDIAN
                    boost::uint8_t StreamType : 6;
                    boost::uint8_t StreamPriority : 1;
#else
                    boost::uint8_t StreamPriority : 1;
                    boost::uint8_t StreamType : 6;
#endif
                };
                boost::uint8_t byte;
            };
            framework::system::UInt24 BufferSize;
            boost::uint32_t MaxBitrate;
            boost::uint32_t AverageBitrate;
            Mp4Vector<Mp4Descriptor> sub_descriptors;

            Mp4DecoderConfigDescriptor()
                : ObjectTypeIndication(0)
                , byte(0)
                , MaxBitrate(0)
                , AverageBitrate(0)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & ObjectTypeIndication
                    & byte
                    & BufferSize
                    & MaxBitrate
                    & AverageBitrate
                    & sub_descriptors;
            }
        };

        struct Mp4DecoderSpecificInfoDescriptor
            : Mp4DescriptorData<Mp4DecoderSpecificInfoDescriptor, Mp4DescriptorType::DECODER_SPECIFIC_INFO>
        {
            Mp4Vector<boost::uint8_t> Info;

            Mp4DecoderSpecificInfoDescriptor()
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & Info;
            }
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_MP4_DESCRIPTOR_H_
