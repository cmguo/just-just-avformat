// PsiPacket.h

#ifndef _PPBOX_AVFORMAT_TS_PSI_PACKET_H_
#define _PPBOX_AVFORMAT_TS_PSI_PACKET_H_

#include "ppbox/avformat/ts/TsPacket.h"

#include <boost/asio/buffer.hpp>

namespace ppbox
{
    namespace avformat
    {

        struct PsiTable
        {
            boost::uint8_t pointer;
            boost::uint8_t table_id;

            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint16_t section_syntax_indicator : 1;
                    boost::uint16_t undef : 1;
                    boost::uint16_t reserved : 2;
                    boost::uint16_t section_length : 12;
#else
                    boost::uint16_t section_length : 12;
                    boost::uint16_t reserved : 2;
                    boost::uint16_t undef : 1;
                    boost::uint16_t section_syntax_indicator : 1;
#endif
                };
                boost::uint16_t byte34;
            };

            boost::uint16_t transport_stream_id;

            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint8_t reserved1 : 2;
                    boost::uint8_t version_number : 5;
                    boost::uint8_t current_next_indicator : 1;
#else
                    boost::uint8_t current_next_indicator : 1;
                    boost::uint8_t version_number : 5;
                    boost::uint8_t reserved1 : 2;
#endif
                };
                boost::uint8_t byte7;
            };

            boost::uint8_t section_number;
            boost::uint8_t last_section_number;

            PsiTable()
                : pointer(0)
                , table_id(0)
                , byte34(0)
                , byte7(0)
                , section_number(0)
                , last_section_number(0)
            {
            }

            PsiTable(
                boost::uint8_t id, 
                boost::uint16_t body_length);

            boost::uint32_t section_size() const
            {
                return 4 + section_length;
            }

            boost::uint32_t body_size() const
            {
                return section_length - 5 - 4;
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & pointer;
                ar & table_id;
                ar & byte34;
                ar & transport_stream_id;
                ar & byte7;
                ar & section_number;
                ar & last_section_number;
            }
        };

        struct PatSection
        {
            boost::uint16_t program_number;
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint16_t reserved : 3;
                    boost::uint16_t program_map_id : 13;
#else
                    boost::uint16_t program_map_id : 13;
                    boost::uint16_t reserved : 3;
#endif
                };
                boost::uint16_t byte34;
            };

            PatSection()
                : program_number(0)
                , byte34(0xe000)
            {
            }

            PatSection(
                boost::uint16_t program_number, 
                boost::uint16_t program_map_id)
                : program_number(program_number)
                , byte34(0xe000 | (program_map_id & 0x1fff))
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & program_number;
                ar & byte34;
            }
        };

        struct PatPacket
            : TsPacket
        {
            PsiTable table;
            std::vector<PatSection> sections;

            PatPacket();

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                TsPacket::serialize(ar);
                ar & table;
                util::serialization::serialize_collection(ar, sections, table.body_size() / 4);
            }
        };

        struct TsStreamInfo
        {
            boost::uint8_t stream_type;
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint16_t reserved : 3;
                    boost::uint16_t elementary_pid : 13;
#else
                    boost::uint16_t elementary_pid : 13;
                    boost::uint16_t reserved : 3;
#endif
                };
                boost::uint16_t byte34;
            };

            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint16_t reserved1 : 4;
                    boost::uint16_t es_info_length : 12;
#else
                    boost::uint16_t es_info_length : 12;
                    boost::uint16_t reserved1 : 4;
#endif
                };
                boost::uint16_t byte56;
            };

            //for (i = 0; i < N; i++) {
            //    descriptor()
            //}

            TsStreamInfo()
                : stream_type(0)
                , byte34(0)
                , byte56(0)
            {
            }

            TsStreamInfo(
                boost::uint8_t stream_type, 
                boost::uint16_t elementary_pid)
                : stream_type(stream_type)
                , byte34(0xe000 | (elementary_pid & 0x1fff))
                , byte56(0xf000)
            {
            }

            boost::uint32_t size() const
            {
                return 5 + es_info_length;
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & stream_type;
                ar & byte34;
                ar & byte56;
            }
        };

        struct PmtPacket
            : TsPacket
        {
            PsiTable table;

            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint16_t reserved : 3;
                    boost::uint16_t pcr_pid : 13;
#else
                    boost::uint16_t pcr_pid : 13;
                    boost::uint16_t reserved : 3;
#endif
                };
                boost::uint16_t byte12;
            };

            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint16_t reserved1 : 4;
                    boost::uint16_t program_info_length : 12;
#else
                    boost::uint16_t program_info_length : 12;
                    boost::uint16_t reserved1 : 4;
#endif
                };
                boost::uint16_t byte34;
            };

            //for (i = 0; i < N; i++) {
            //    descriptor()
            //}

            std::vector<TsStreamInfo> streams;

            PmtPacket();

            void add_stream(
                bool video, 
                boost::uint8_t type, 
                boost::uint32_t index);

            void complete();

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                TsPacket::serialize(ar);
                ar & table;
                ar & byte12;
                ar & byte34;
                util::serialization::serialize_collection(ar, streams, (table.body_size() - 4) / 5);
            }
        };

        boost::uint32_t psi_calc_crc(
            boost::uint8_t const * data, 
            boost::uint32_t data_size);

        boost::uint32_t psi_calc_crc(
            boost::asio::const_buffer const & data);

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_TS_PSI_PACKET_H_
