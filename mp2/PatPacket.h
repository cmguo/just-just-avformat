// PatPacket.h

#ifndef _JUST_AVFORMAT_MP2_PAT_PACKET_H_
#define _JUST_AVFORMAT_MP2_PAT_PACKET_H_

#include "just/avformat/mp2/PsiPacket.h"
#include "just/avformat/mp2/Mp2Vector.h"
#include "just/avformat/mp2/Mp2CrcBuffer.h"

namespace just
{
    namespace avformat
    {

        struct PatProgram
        {
            boost::uint16_t number;
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint16_t reserved : 3;
                    boost::uint16_t map_id : 13;
#else
                    boost::uint16_t map_id : 13;
                    boost::uint16_t reserved : 3;
#endif
                };
                boost::uint16_t byte34;
            };

            PatProgram()
                : number(0)
                , byte34(0xe000)
            {
            }

            PatProgram(
                boost::uint16_t number, 
                boost::uint16_t map_id)
                : number(number)
                , byte34(0xe000 | (map_id & 0x1fff))
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & number;
                ar & byte34;
            }
        };

        struct PatSection
            : PsiTable
        {
            Mp2Vector<PatProgram> programs;

            boost::uint32_t crc;

            PatSection()
            {
            }

            // construct pat section with only one program
            PatSection(
                boost::uint16_t program_number, 
                boost::uint16_t program_map_id);

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp2AutoCrc<Archive> ac(ar, crc);

                PsiTable::serialize(ar);
                programs.set_byte_size(body_size());
                ar & programs;
            }
        };

        struct PatPayload
        {
            boost::uint8_t pointer;
            Mp2Vector<PatSection> sections;

            PatPayload(
                TsPacket & packet)
                : pointer(0)
                , packet_(packet)
            {
            }

            // construct pat with only one section, and only one program
            PatPayload(
                TsPacket & packet, 
                boost::uint16_t program_number, 
                boost::uint16_t program_map_id);

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & pointer;
                sections.set_byte_size_with_suffer(packet_.payload_size() - 1);
                ar & sections;
            }

        private:
            TsPacket & packet_;
        };

        struct PatPacket
            : TsPacket
            , PatPayload
        {
            // construct pat with only one section, and only one program (default id)
            PatPacket();

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                TsPacket::serialize(ar);
                PatPayload::serialize(ar);
            }
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MP2_PAT_PACKET_H_
