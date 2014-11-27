// PesPacket.h

#ifndef _JUST_AVFORMAT_MP2_PES_PACKET_H_
#define _JUST_AVFORMAT_MP2_PES_PACKET_H_

#include "just/avformat/mp2/Mp2Enum.h"

#include <util/serialization/SplitMember.h>

#include <framework/container/Array.h>

namespace just
{
    namespace avformat
    {

        struct StdBuffer
        {
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint16_t flag : 2;
                    boost::uint16_t scale : 1;
                    boost::uint16_t size : 13;
#else
                    boost::uint16_t size : 13;
                    boost::uint16_t scale : 1;
                    boost::uint16_t flag : 2;
#endif
                };
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint8_t byte1;
                    boost::uint8_t byte2;
#else
                    boost::uint8_t byte2;
                    boost::uint8_t byte1;
#endif
                };
                boost::uint16_t byte12;
            };

            StdBuffer()
                : byte12(0)
            {
            }

            static boost::uint8_t const SIZE = 2;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & byte12;
            }
        };

        struct PtsDts
        {
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint8_t flag : 4;
                    boost::uint8_t bits_32_30 : 3;
                    boost::uint8_t marker_bit1 : 1;
#else
                    boost::uint8_t marker_bit1 : 1;
                    boost::uint8_t bits_32_30 : 3;
                    boost::uint8_t flag : 4;
#endif
                };
                boost::uint8_t byte1;
            };

            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint16_t bits_29_15 : 15;
                    boost::uint8_t marker_bit2 : 1;
#else
                    boost::uint16_t marker_bit2 : 1;
                    boost::uint16_t bits_29_15 : 15;
#endif
                };
                boost::uint16_t byte23;
            };

            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint16_t bits_14_0 : 15;
                    boost::uint16_t marker_bit3 : 1;
#else
                    boost::uint16_t marker_bit3 : 1;
                    boost::uint16_t bits_14_0 : 15;
#endif
                };
                boost::uint16_t byte45;
            };

            PtsDts()
                : byte1(0)
                , byte23(0)
                , byte45(0)
            {
            }

            PtsDts(
                boost::uint8_t flag, 
                boost::uint64_t value)
                : byte1((flag << 4) | ((boost::uint8_t)(value >> 29) & 0x0e) | 1)
                , byte23(((boost::uint16_t)(value >> 14) & 0xfffe) | 1)
                , byte45(((boost::uint16_t)(value << 1) & 0xfffe) | 1)
            {
            }

            boost::uint64_t value() const
            {
                return (boost::uint64_t)bits_32_30 << 30 | (boost::uint64_t)bits_29_15 << 15 | bits_14_0;
            }

            static boost::uint8_t const SIZE = 5;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & byte1;
                ar & byte23;
                ar & byte45;
            }
        };

        struct OneByte
        {
            boost::uint8_t byte1;

            OneByte()
                : byte1(0)
            {
            }

            static boost::uint8_t const SIZE = 1;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & byte1;
            }
        };

        template <typename T, boost::uint8_t F>
        struct FlagField
            : public T
        {

            FlagField()
            {
            }

            template <typename A1>
            FlagField(A1 a1)
                : T(a1)
            {
            }

            template <typename A1, typename A2>
            FlagField(A1 a1, A2 a2)
                : T(a1, a2)
            {
            }

            template <typename A1, typename A2, typename A3>
            FlagField(A1 a1, A2 a2, A3 a3)
                : T(a1, a2, a3)
            {
            }

            SERIALIZATION_SPLIT_MEMBER();

            template <typename Archive>
            void load(
                Archive & ar)
            {
                boost::uint8_t next = ar.rdbuf()->sgetc();
                if ((next & F) == F) {
                    T::serialize(ar);
                } else {
                    T::byte1 = 0;
                }
            }

            template <typename Archive>
            void save(
                Archive & ar) const
            {
                if ((T::byte1 & F) == F) {
                    const_cast<FlagField *>(this)->T::serialize(ar);
                }
            }

            boost::uint8_t size() const
            {
                if ((T::byte1 & F) == F) {
                    return T::SIZE;
                } else {
                    return 0;
                }
            }
        };

        struct PesPacket1
        {
            boost::uint8_t packet_start_code_prefix1;
            boost::uint8_t packet_start_code_prefix2;
            boost::uint8_t packet_start_code_prefix3;
            boost::uint8_t stream_id;
            boost::uint16_t packet_length;

            std::vector<boost::uint8_t> stuffing_byte;

            FlagField<StdBuffer, 0x40> std_buffer;
            FlagField<PtsDts, 0x20> pts_bits;
            PtsDts dts_bits;
            boost::uint8_t  one_byte;

            PesPacket1()
                : packet_start_code_prefix1(0)
                , packet_start_code_prefix2(0)
                , packet_start_code_prefix3(0)
                , stream_id(0)
                , packet_length(0)
                , one_byte(0)
            {
            }

            PesPacket1(
                boost::uint8_t stream_id, 
                boost::uint32_t size, 
                boost::uint64_t cts);

            PesPacket1(
                boost::uint8_t stream_id, 
                boost::uint32_t size, 
                boost::uint64_t cts, 
                boost::uint64_t dts);

            bool special() const
            {
                return stream_id == Mp2StreamId::private_stream_2;
            }

            boost::uint32_t payload_length() const
            {
                if (special()) {
                    return packet_length;
                }
                boost::uint8_t header_size[] = {1, 0, 5, 10};
                return packet_length 
                    - stuffing_byte.size()
                    - std_buffer.size() 
                    - header_size[pts_bits.flag & 3];
            }

            template <typename Archive>
            void serialize_suffing_byte(
                Archive & ar, 
                boost::mpl::false_)
            {
                stuffing_byte.clear();
                FlagField<OneByte, 0xff> stuffing;
                while ((ar & stuffing) && stuffing.byte1 == 0xff)
                    stuffing_byte.push_back(stuffing.byte1);
            }

            template <typename Archive>
            void serialize_suffing_byte(
                Archive & ar, 
                boost::mpl::true_)
            {
                ar & framework::container::make_array(&stuffing_byte.front(), stuffing_byte.size());
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & packet_start_code_prefix1;
                ar & packet_start_code_prefix2;
                ar & packet_start_code_prefix3;
                ar & stream_id;
                if (packet_start_code_prefix1 != 0 
                    || packet_start_code_prefix2 != 0
                    || packet_start_code_prefix3 != 1
                    || stream_id < Mp2StreamId::program_stream_map) {
                        ar.fail();
                        return;
                }
                ar & packet_length;
                if (special()) {
                    return;
                }
                serialize_suffing_byte(ar, typename Archive::is_saving());
                ar & std_buffer;
                ar & pts_bits;
                if (pts_bits.flag == 3) {
                    ar & dts_bits;
                } else if (pts_bits.flag == 0) {
                    ar & one_byte;
                }
            }
        };

        struct PesPacket
            : PesPacket1
        {
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint8_t reserved : 2;
                    boost::uint8_t PES_scrambling_control : 2;
                    boost::uint8_t PES_priority : 1;
                    boost::uint8_t data_alignment_indicator : 1;
                    boost::uint8_t copyright : 1;
                    boost::uint8_t original_or_copy : 1;
#else
                    boost::uint8_t original_or_copy : 1;
                    boost::uint8_t copyright : 1;
                    boost::uint8_t data_alignment_indicator : 1;
                    boost::uint8_t PES_priority : 1;
                    boost::uint8_t PES_scrambling_control : 2;
                    boost::uint8_t reserved : 2;
#endif
                };
                boost::uint8_t byte1;
            };

            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint8_t PTS_DTS_flags : 2;
                    boost::uint8_t ESCR_flag : 1;
                    boost::uint8_t ES_rate_flag : 1;
                    boost::uint8_t DSM_trick_mode_flag : 1;
                    boost::uint8_t additional_copy_info_flag : 1;
                    boost::uint8_t PES_CRC_flag : 1;
                    boost::uint8_t PES_extension_flag : 1;
#else
                    boost::uint8_t PES_extension_flag : 1;
                    boost::uint8_t PES_CRC_flag : 1;
                    boost::uint8_t additional_copy_info_flag : 1;
                    boost::uint8_t DSM_trick_mode_flag : 1;
                    boost::uint8_t ES_rate_flag : 1;
                    boost::uint8_t ESCR_flag : 1;
                    boost::uint8_t PTS_DTS_flags : 2;
#endif
                };
                boost::uint8_t byte2;
            };

            boost::uint8_t header_data_length;

            PesPacket()
                : byte1(0)
                , byte2(0)
                , header_data_length(0)
            {
            }

            PesPacket(
                boost::uint8_t stream_id, 
                boost::uint32_t size, 
                boost::uint64_t cts);

            PesPacket(
                boost::uint8_t stream_id, 
                boost::uint32_t size, 
                boost::uint64_t cts, 
                boost::uint64_t dts);

            bool special() const
            {
                return stream_id == Mp2StreamId::program_stream_map
                    || stream_id == Mp2StreamId::padding_stream
                    || stream_id == Mp2StreamId::private_stream_2
                    || stream_id == Mp2StreamId::ecm_stream
                    || stream_id == Mp2StreamId::emm_stream
                    || stream_id == Mp2StreamId::dsmcc_stream
                    || stream_id == Mp2StreamId::itu_t_rec_h_222_1_E
                    || stream_id == Mp2StreamId::program_stream_directory;
            }

            boost::uint32_t payload_length() const
            {
                if (special()) {
                    return packet_length;
                }
                if (reserved != 2)
                    return PesPacket1::payload_length();
                return packet_length ? packet_length - 3 - header_data_length : 0;
            }

            template <typename Archive>
            bool is_version1(
                Archive & ar, 
                boost::mpl::true_)
            {
                return reserved != 2;
            }

            template <typename Archive>
            bool is_version1(
                Archive & ar, 
                boost::mpl::false_)
            {
                return (ar.rdbuf()->sgetc() & 0xC0) != 0x80;
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & packet_start_code_prefix1;
                ar & packet_start_code_prefix2;
                ar & packet_start_code_prefix3;
                ar & stream_id;
                if (packet_start_code_prefix1 != 0 
                    || packet_start_code_prefix2 != 0
                    || packet_start_code_prefix3 != 1
                    || stream_id < Mp2StreamId::program_stream_map) {
                        ar.fail();
                        return;
                }
                ar & packet_length;
                if (special()) {
                    return;
                }
                if (is_version1(ar, typename Archive::is_saving())) {
                    serialize_suffing_byte(ar, typename Archive::is_saving());
                    ar & std_buffer;
                    ar & pts_bits;
                    if (pts_bits.flag == 3) {
                        ar & dts_bits;
                    } else if (pts_bits.flag == 0) {
                        ar & one_byte;
                    }
                    return;
                }
                ar & byte1;
                ar & byte2;
                ar & header_data_length;
                boost::uint8_t l = header_data_length;
                if (PTS_DTS_flags == 3) { // pts & dts
                    ar & (PtsDts &)pts_bits;
                    ar & (PtsDts &)dts_bits;
                    l -= 10;
                } else if (PTS_DTS_flags == 2) { // pts
                    ar & (PtsDts &)pts_bits;
                    l -= 5;
                } else {
                    pts_bits.flag = 0;
                }
                if (l) {
                    stuffing_byte.resize(l);
                    ar & framework::container::make_array(&stuffing_byte.front(), stuffing_byte.size());
                }
            }
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MP2_PES_PACKET_H_
