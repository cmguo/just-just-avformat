// PsmPacket.h

#ifndef _PPBOX_AVFORMAT_MP2_PSM_PACKET_H_
#define _PPBOX_AVFORMAT_MP2_PSM_PACKET_H_

#include "ppbox/avformat/mp2/Mp2Vector.h"
#include "ppbox/avformat/mp2/Mp2Descriptor.h"
#include "ppbox/avformat/mp2/Mp2CrcBuffer.h"

namespace ppbox
{
    namespace avformat
    {

        struct PsmStream
        {
            boost::uint8_t stream_type;
            boost::uint8_t elementary_stream_id;
            boost::uint16_t elementary_stream_info_length;
            Mp2Vector<Mp2Descriptor> descriptor;

            PsmStream()
                : stream_type(0)
                , elementary_stream_id(0)
                , elementary_stream_info_length(0)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & stream_type;
                ar & elementary_stream_id;
                ar & elementary_stream_info_length;
                if (elementary_stream_info_length) {
                    descriptor.set_byte_size(elementary_stream_info_length);
                    ar & descriptor;
                }
            }
        };


        struct PsmPacket
        {
            boost::uint8_t packet_start_code_prefix1;
            boost::uint8_t packet_start_code_prefix2;
            boost::uint8_t packet_start_code_prefix3;
            boost::uint8_t map_stream_id;
            boost::uint16_t program_stream_map_length;
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint8_t current_next_indicator : 1;
                    boost::uint8_t reserved1 : 2;
                    boost::uint8_t program_stream_map_version : 5;
#else
                    boost::uint8_t program_stream_map_version : 5;
                    boost::uint8_t reserved1 : 2;
                    boost::uint8_t current_next_indicator : 1;
#endif
                };
                boost::uint8_t byte1;
            };
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint8_t reserved2 : 7;
                    boost::uint8_t marker_bit : 1;
#else
                    boost::uint8_t marker_bit : 1;
                    boost::uint8_t reserved2 : 7;
#endif
                };
                boost::uint8_t byte2;
            };

            boost::uint16_t program_stream_info_length;
            Mp2Vector<Mp2Descriptor> descriptor;

            boost::uint16_t elementary_stream_map_length;
            Mp2Vector<PsmStream> elems;

            boost::uint32_t crc;

            PsmPacket()
                : packet_start_code_prefix1(0)
                , packet_start_code_prefix2(0)
                , packet_start_code_prefix3(0)
                , map_stream_id(0)
                , program_stream_map_length(0)
                , byte1(0)
                , byte2(0)
                , program_stream_info_length(0)
                , elementary_stream_map_length(0)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp2AutoCrc<Archive> ac(ar, crc);

                ar & packet_start_code_prefix1;
                ar & packet_start_code_prefix2;
                ar & packet_start_code_prefix3;
                ar & map_stream_id;
                ar & program_stream_map_length;
                ar & byte1;
                ar & byte2;
                ar & program_stream_info_length;
                if (program_stream_info_length) {
                    descriptor.set_byte_size(program_stream_info_length);
                    ar & descriptor;
                }
                ar & elementary_stream_map_length;
                if (elementary_stream_map_length) {
                    descriptor.set_byte_size(elementary_stream_map_length);
                    ar & elems;
                }
            }
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP2_PSM_PACKET_H_
