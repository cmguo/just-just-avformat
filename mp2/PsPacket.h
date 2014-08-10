// PsPacket.h

#ifndef _PPBOX_AVFORMAT_MP2_PS_PACKET_H_
#define _PPBOX_AVFORMAT_MP2_PS_PACKET_H_

#include "ppbox/avformat/mp2/Mp2Vector.h"
#include "ppbox/avformat/mp2/Mp2Enum.h"

namespace ppbox
{
    namespace avformat
    {

        struct PsSystemHeader
        {
            boost::uint32_t system_header_start_code;
            boost::uint16_t header_length;
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint32_t marker_bit1 : 1;
                    boost::uint32_t rate_bound : 22;
                    boost::uint32_t marker_bit2 : 1;
                    boost::uint32_t audio_bound : 6;
                    boost::uint32_t fixed_flag : 1;
                    boost::uint32_t CSPS_flag : 1;
#else
                    boost::uint32_t CSPS_flag : 1;
                    boost::uint32_t fixed_flag : 1;
                    boost::uint32_t audio_bound : 6;
                    boost::uint32_t marker_bit2 : 1;
                    boost::uint32_t rate_bound : 22;
                    boost::uint32_t marker_bit1 : 1;
#endif
                };
                boost::uint32_t four_bytes;
            };
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint8_t system_audio_lock_flag : 1;
                    boost::uint8_t system_video_lock_flag : 1;
                    boost::uint32_t marker_bit3 : 1;
                    boost::uint8_t video_bound : 5;
#else
                    boost::uint8_t video_bound : 5;
                    boost::uint32_t marker_bit3 : 1;
                    boost::uint8_t system_video_lock_flag : 1;
                    boost::uint8_t system_audio_lock_flag : 1;
#endif
                };
                boost::uint8_t one_byte1;
            };
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint8_t packet_rate_restriction_flag : 1;
                    boost::uint8_t reserved_bits : 7;
#else
                    boost::uint8_t reserved_bits : 7;
                    boost::uint8_t packet_rate_restriction_flag : 1;
#endif
                };
                boost::uint8_t one_byte2;
            };

            struct Stream
            {
                boost::uint8_t stream_id;
                union {
                    struct {
#ifdef   BOOST_BIG_ENDIAN
                        boost::uint16_t bits11 : 2;
                        boost::uint16_t P_STD_buffer_bound_scale : 1;
                        boost::uint16_t P_STD_buffer_size_bound : 13;
#else
                        boost::uint16_t P_STD_buffer_size_bound : 13;
                        boost::uint16_t P_STD_buffer_bound_scale : 1;
                        boost::uint16_t bits11 : 2;
#endif
                    };
                    boost::uint16_t two_bytes;
                };

                Stream()
                    : stream_id(0)
                    , two_bytes(0)
                {
                }

                template <typename Archive>
                void serialize(
                    Archive & ar)
                {
                    ar & stream_id;
                    if (ar && (stream_id & 0x80) == 0) {
                        ar.fail();
                        return;
                    }
                    ar & two_bytes;
                }
            };

            Mp2Vector<Stream> streams;

            PsSystemHeader()
                : system_header_start_code(0)
                , header_length(0)
                , four_bytes(0)
                , one_byte1(0)
                , one_byte2(0)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & system_header_start_code;
                if (ar && system_header_start_code != 0x000001bb) {
                    ar.fail();
                    return;
                }
                ar & header_length;
                ar & four_bytes;
                ar & one_byte1;
                ar & one_byte2;
                ar & streams;
            }
        };

        // MP2-1 Pack Header
        // http://stnsoft.com/DVD/packhdr.html

        struct PsPacketHeader1
        {
            boost::uint32_t pack_start_code;
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint32_t bits0010 : 4;
                    boost::uint32_t system_clock_reference_base_32_30 : 3; // [32..30]
                    boost::uint32_t marker_bit1 : 1;
                    boost::uint32_t system_clock_reference_base_29_15 : 15;
                    boost::uint32_t marker_bit2 : 1;
                    boost::uint32_t system_clock_reference_base_14_7 : 8;
#else
                    boost::uint32_t system_clock_reference_base_14_7 : 8;
                    boost::uint32_t marker_bit2 : 1;
                    boost::uint32_t system_clock_reference_base_29_15 : 15;
                    boost::uint32_t marker_bit1 : 1;
                    boost::uint32_t system_clock_reference_base_32_30 : 3; // [32..30]
                    boost::uint32_t bits0010 : 4;
#endif
                };
                boost::uint32_t four_bytes1;
            };
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint32_t system_clock_reference_base_6_0 : 7;
                    boost::uint32_t marker_bit3 : 1;
                    boost::uint32_t marker_bit4 : 1;
                    boost::uint32_t program_mux_rate : 22;
                    boost::uint32_t marker_bit5 : 1;
#else
                    boost::uint32_t marker_bit5 : 1;
                    boost::uint32_t program_mux_rate : 22;
                    boost::uint32_t marker_bit4 : 1;
                    boost::uint32_t marker_bit3 : 1;
                    boost::uint32_t system_clock_reference_base_6_0 : 7;
#endif
                };
                boost::uint32_t four_bytes2;
            };
            Mp2Vector<PsSystemHeader> system_headers;

            PsPacketHeader1()
                : pack_start_code(0)
                , four_bytes1(0)
                , four_bytes2(0)
            {
            }

            boost::uint64_t pcr() const
            {
                return ((boost::uint64_t)system_clock_reference_base_32_30 << 30)
                    | (system_clock_reference_base_29_15 << 15)
                    | (system_clock_reference_base_14_7 << 7)
                    | system_clock_reference_base_6_0;
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & pack_start_code;
                if (ar && pack_start_code != 0x000001ba) {
                    ar.fail();
                    return;
                }
                ar & four_bytes1;
                ar & four_bytes2;
                ar & system_headers;
            }
        };

        // MP2-2 Pack Header

        struct PsPacketHeader
            : PsPacketHeader1
        {
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint32_t bits01 : 2;
                    boost::uint32_t system_clock_reference_base_32_30 : 3; // [32..30]
                    boost::uint32_t marker_bit1 : 1;
                    boost::uint32_t system_clock_reference_base_29_15 : 15;
                    boost::uint32_t marker_bit2 : 1;
                    boost::uint32_t system_clock_reference_base_14_5 : 10;
#else
                    boost::uint32_t system_clock_reference_base_14_5 : 10;
                    boost::uint32_t marker_bit2 : 1;
                    boost::uint32_t system_clock_reference_base_29_15 : 15;
                    boost::uint32_t marker_bit1 : 1;
                    boost::uint32_t system_clock_reference_base_32_30 : 3; // [32..30]
                    boost::uint32_t bits01 : 2;
#endif
                };
                boost::uint32_t four_bytes1;
            };
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint16_t system_clock_reference_base_4_0 : 5;
                    boost::uint16_t marker_bit3 : 1;
                    boost::uint16_t system_clock_reference_extension : 9;
                    boost::uint16_t marker_bit4 : 1;
#else
                    boost::uint16_t marker_bit4 : 1;
                    boost::uint16_t system_clock_reference_extension : 9;
                    boost::uint16_t marker_bit3 : 1;
                    boost::uint16_t system_clock_reference_base_4_0 : 5;
#endif
                };
                boost::uint16_t two_bytes;
            };
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint32_t program_mux_rate : 22;
                    boost::uint32_t marker_bit5 : 1;
                    boost::uint32_t marker_bit6 : 1;
                    boost::uint32_t reserved : 5;
                    boost::uint32_t pack_stuffing_length : 3;
#else
                    boost::uint32_t pack_stuffing_length : 3;
                    boost::uint32_t reserved : 5;
                    boost::uint32_t marker_bit6 : 1;
                    boost::uint32_t marker_bit5 : 1;
                    boost::uint32_t program_mux_rate : 22;
#endif
                };
                boost::uint32_t four_bytes2;
            };

            std::vector<boost::uint8_t> stuffing_bytes;

            PsPacketHeader()
                : four_bytes1(0)
                , two_bytes(0)
                , four_bytes2(0)
            {
                }

            boost::uint64_t pcr() const
            {
                if (bits01 != 0x01) {
                    return PsPacketHeader1::pcr();
                }
                return ((boost::uint64_t)system_clock_reference_base_32_30 << 30)
                    | (system_clock_reference_base_29_15 << 15)
                    | (system_clock_reference_base_14_5 << 5)
                    | system_clock_reference_base_4_0;
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & pack_start_code;
                if (ar && pack_start_code != 0x000001ba) {
                    while ((pack_start_code & 0xffffff00) == 0) {
                        boost::uint8_t b;
                        ar & b;
                        if (ar)
                            pack_start_code = (pack_start_code << 8) | b;
                        else
                            break;
                    } 
                    if (ar && pack_start_code != 0x000001ba) {
                        ar.fail();
                        return;
                    }
                }
                ar & four_bytes1;
                if (bits01 == 0x01) {
                    ar & two_bytes;
                    ar & four_bytes2;
                    if (pack_stuffing_length > 0) {
                        util::serialization::serialize_collection(ar, stuffing_bytes, pack_stuffing_length);
                    }
                } else {
                    PsPacketHeader1::four_bytes1 = four_bytes1;
                    ar & PsPacketHeader1::four_bytes2;
                }
                ar & system_headers;
            }
        };

        struct PsPacket
            : PsPacketHeader
        {
            static boost::uint32_t const TIME_SCALE = Mp2::TIME_SCALE;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP2_PS_PACKET_H_
