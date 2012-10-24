// AacConfig.h

#ifndef _PPBOX_AVFORMAT_CODEC_AAC_AAC_CONFIG_H_
#define _PPBOX_AVFORMAT_CODEC_AAC_AAC_CONFIG_H_

#include "ppbox/avformat/BitsType.h"

namespace ppbox
{
    namespace avformat
    {

        // AAC Setup Data

        class AacConfig
        {
        public:
            U<5> object_type;
            U<6> object_type_minus_32; // if object_type == 31
            U<4> frequency_index;
            U<24> frequency; // if frequency_index == 15
            U<4> channel_configuration;
            U<1> frame_length_flag;
            U<1> depends_on_core_coder;
            U<1> extension_flag;

        public:
            AacConfig();

            AacConfig(
                boost::uint8_t const * buf, 
                boost::uint32_t size);

            AacConfig(
                std::vector<boost::uint8_t> const & buf);

        public:
            void from_data(
                std::vector<boost::uint8_t> const & buf);

            void to_data(
                std::vector<boost::uint8_t> & buf) const;

        public:
            boost::uint32_t get_frequency() const
            {
                return (frequency_index == 15) 
                    ? (boost::uint32_t)frequency 
                    : frequency_table[frequency_index];
            }

            void set_frequency(
                size_t frequency);

            boost::uint32_t get_object_type() const
            {
                return (object_type == 31) 
                    ? (boost::uint32_t)object_type_minus_32 + 32 
                    : (boost::uint32_t)object_type;
            }

            void set_object_type(
                size_t object_type)
            {
                if (object_type < 31) {
                    this->object_type = object_type;
                } else {
                    this->object_type = 31;
                    this->object_type_minus_32 = object_type - 32;
                }
            }

        private:
            static boost::uint32_t frequency_table[];
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_CODEC_AAC_AAC_CONFIG_H_
