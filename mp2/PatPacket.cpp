// PatPacket.cpp

#include "just/avformat/Common.h"
#include "just/avformat/mp2/PatPacket.h"
#include "just/avformat/mp2/Mp2Enum.h"

namespace just
{
    namespace avformat
    {

        PatSection::PatSection(
            boost::uint16_t program_number, 
            boost::uint16_t program_map_id)
            : PsiTable(PsiTableId::program_association, 0)
        {
            programs.push_back(PatProgram(program_number, program_map_id));
            section_length += 4;
        }

        PatPayload::PatPayload(
            TsPacket & packet, 
            boost::uint16_t program_number, 
            boost::uint16_t program_map_id)
            : pointer(0)
            , packet_(packet)
        {
            sections.push_back(PatSection(program_number, program_map_id));
        }

        PatPacket::PatPacket()
            : TsPacket(TsPid::pat, 0)
            , PatPayload(*this, default_program_number, TsPid::pmt)
        {
            fill_payload2(1 + sections.front().section_size());
        }

    } // namespace avformat
} // namespace just
