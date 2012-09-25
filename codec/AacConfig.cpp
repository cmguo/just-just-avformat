// AacConfig.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/codec/AacConfig.h"
#include "ppbox/avformat/BitsIStream.h"
#include "ppbox/avformat/BitsOStream.h"

#include <util/archive/ArchiveBuffer.h>

namespace ppbox
{
    namespace avformat
    {

        boost::uint32_t AacConfig::frequency_table[] = 
        {
            96000, 88200, 64000, 48000, 
            44100, 32000, 24000, 22050, 
            16000, 12000, 11025, 8000,  
            7350,  
        };

        template <
            typename Archive
        >
        void serialize(
            Archive & ar, 
            AacConfig & c)
        {
            ar & c.object_type;
            if (c.object_type == 31)
                ar & c.object_type_minus_32;
            ar & c.frequency_index;
            if (c.frequency_index == 15)
                ar & c.frequency;
            ar & c.channel_configuration
                & c.frame_length_flag
                & c.depends_on_core_coder
                & c.extension_flag;
        }

        AacConfig::AacConfig()
        {
        }

        AacConfig::AacConfig(
            boost::uint8_t const * buf, 
            boost::uint32_t size)
        {
            std::vector<boost::uint8_t> vec(buf, buf + size);
            from_data(vec);
        }

        AacConfig::AacConfig(
            std::vector<boost::uint8_t> const & vec)
        {
            from_data(vec);
        }

        void AacConfig::from_data(
            std::vector<boost::uint8_t> const & buf)
        {
            util::archive::ArchiveBuffer<boost::uint8_t> abuf((boost::uint8_t *)&buf[0], buf.size(), buf.size());
            ppbox::avformat::BitsIStream<boost::uint8_t> is(abuf);
            is >> *this;
        }

        void AacConfig::to_data(
            std::vector<boost::uint8_t> & buf)
        {
            buf.resize(16);
            util::archive::ArchiveBuffer<boost::uint8_t> abuf((boost::uint8_t *)&buf[0], buf.size());
            ppbox::avformat::BitsOStream<boost::uint8_t> os(abuf);
            os << *this;
            while (!os.byte_aligned())
                os << U<1>(0);
            buf.resize(abuf.size());
        }

        void AacConfig::set_frequency(
            size_t frequency)
        {
            for (size_t i = 0; i < sizeof(frequency_table) / sizeof(frequency_table[0]); ++i) {
                if (frequency == frequency_table[i]) {
                    frequency_index = i;
                    return;
                }
            }
            frequency_index = 15;
            this->frequency = frequency;
        }

    } // namespace avformat
} // namespace ppbox
