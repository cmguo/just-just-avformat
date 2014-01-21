// PmtPacket.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/ts/PmtPacket.h"
#include "ppbox/avformat/ts/TsEnum.h"

namespace ppbox
{
    namespace avformat
    {

        void PmtStream::add_descriptor(
            boost::uint8_t tag, 
            std::vector<boost::uint8_t> const & descriptor)
        {
            TsDescriptor d;
            d.descriptor_tag = tag;
            d.descriptor_length = (boost::uint8_t)descriptor.size();
            d.descriptor = descriptor;
            this->descriptor.push_back(d);
            es_info_length += 2 + d.descriptor_length;
        }

        PmtSection::PmtSection(
            boost::uint16_t program_number)
            : PsiTable(PsiTableId::program_map, 4)
            , byte12(0xe000)
            , byte34(0xf000)
        {
            transport_stream_id = program_number; // transport_stream_id is program_number for pmt
        }

        void PmtSection::add_descriptor(
            boost::uint8_t tag, 
            std::vector<boost::uint8_t> const & descriptor)
        {
            TsDescriptor d;
            d.descriptor_tag = tag;
            d.descriptor_length = (boost::uint8_t)descriptor.size();
            d.descriptor = descriptor;
            this->descriptor.push_back(d);
            program_info_length += 2 + d.descriptor_length;
        }

        void PmtSection::add_stream(
            boost::uint8_t type)
        {
            add_stream(PmtStream(type));
        }

        void PmtSection::add_stream(
            PmtStream const & stream)
        {
            boost::uint16_t pid = TsPid::stream_base + (boost::uint16_t)streams.size();
            if (streams.empty()) {
                pcr_pid = pid;
            }
            streams.push_back(stream);
            streams.back().elementary_pid = pid;
            section_length += stream.size();
        }

        PmtPayload::PmtPayload(
            TsPacket & packet, 
            boost::uint16_t program_number)
            : pointer(0)
            , packet_(packet)
        {
            sections.push_back(PmtSection(program_number));
        }

        PmtPacket::PmtPacket()
            : TsPacket(TsPid::pmt, 0)
            , PmtPayload(*this, default_program_number)
        {
        }

        void PmtPacket::complete()
        {
            fill_payload2(1 + sections.front().section_size());
        }

    } // namespace avformat
} // namespace ppbox
