// AacConfigHelper.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/codec/aac/AacConfigHelper.h"
#include "ppbox/avformat/codec/aac/AacConfig.h"
#include "ppbox/avformat/codec/aac/AacAdts.h"
#include "ppbox/avformat/BitsIStream.h"
#include "ppbox/avformat/BitsOStream.h"

#include <util/archive/ArchiveBuffer.h>

namespace ppbox
{
    namespace avformat
    {

        boost::uint32_t AacConfigHelper::frequency_table[] = 
        {
            96000, 88200, 64000, 48000, 
            44100, 32000, 24000, 22050, 
            16000, 12000, 11025, 8000,  
            7350,  
        };

        AacConfigHelper::AacConfigHelper()
            : data_(new AacConfig)
        {
        }

        AacConfigHelper::AacConfigHelper(
            boost::uint8_t const * buf, 
            boost::uint32_t size)
            : data_(new AacConfig)
        {
            std::vector<boost::uint8_t> vec(buf, buf + size);
            from_data(vec);
        }

        AacConfigHelper::AacConfigHelper(
            std::vector<boost::uint8_t> const & vec)
            : data_(new AacConfig)
        {
            from_data(vec);
        }

        AacConfigHelper::~AacConfigHelper()
        {
            delete data_;
        }

        boost::uint32_t AacConfigHelper::get_frequency() const
        {
            return (data_->samplingFrequencyIndex == 15) 
                ? (boost::uint32_t)data_->samplingFrequency 
                : frequency_table[data_->samplingFrequencyIndex];
        }

        void AacConfigHelper::set_frequency(
            size_t frequency)
        {
            for (size_t i = 0; i < sizeof(frequency_table) / sizeof(frequency_table[0]); ++i) {
                if (frequency == frequency_table[i]) {
                    data_->samplingFrequencyIndex = i;
                    return;
                }
            }
            data_->samplingFrequencyIndex = 15;
            data_->samplingFrequency = frequency;
        }

        boost::uint32_t AacConfigHelper::get_object_type() const
        {
            if (data_->audioObjectType == 5) {
                return (data_->extensionAudioObjectType == 31) 
                    ? (boost::uint32_t)data_->extensionAudioObjectTypeExt + 32 
                    : (boost::uint32_t)data_->extensionAudioObjectType;
            }
            return (data_->audioObjectType == 31) 
                ? (boost::uint32_t)data_->audioObjectTypeExt + 32 
                : (boost::uint32_t)data_->audioObjectType;
        }

        boost::uint32_t AacConfigHelper::get_extension_object_type() const
        {
            if (data_->audioObjectType == 5) {
                return data_->audioObjectType;
            }
            return 0;
        }

        boost::uint32_t AacConfigHelper::get_channel_count() const
        {
            return data_->channelConfiguration;
        }

        void AacConfigHelper::set_object_type(
            size_t object_type)
        {
            if (object_type < 31) {
                data_->audioObjectType = object_type;
            } else {
                data_->audioObjectType = 31;
                data_->audioObjectTypeExt = object_type - 32;
            }
        }

        void AacConfigHelper::from_data(
            std::vector<boost::uint8_t> const & buf)
        {
            util::archive::ArchiveBuffer<boost::uint8_t> abuf((boost::uint8_t *)&buf[0], buf.size(), buf.size());
            ppbox::avformat::BitsIStream<boost::uint8_t> is(abuf);
            is >> *data_;
        }

        void AacConfigHelper::to_data(
            std::vector<boost::uint8_t> & buf) const
        {
            buf.resize(16);
            util::archive::ArchiveBuffer<boost::uint8_t> abuf((boost::uint8_t *)&buf[0], buf.size());
            ppbox::avformat::BitsOStream<boost::uint8_t> os(abuf);
            os << *data_;
            while (!os.byte_aligned())
                os << U<1>(0);
            buf.resize(abuf.size());
        }

        void AacConfigHelper::from_adts_data(
            std::vector<boost::uint8_t> const & buf)
        {
            AacAdts adts;
            util::archive::ArchiveBuffer<boost::uint8_t> abuf((boost::uint8_t *)&buf[0], buf.size());
            ppbox::avformat::BitsOStream<boost::uint8_t> os(abuf);
            os << adts;
            if (!os)
                return;
            data_->audioObjectType = adts.profile + 1;
            data_->audioObjectTypeExt = 0;
            data_->samplingFrequencyIndex = adts.sampling_frequency_index;
            data_->samplingFrequency = 0;
            data_->channelConfiguration = (boost::uint8_t)adts.channel_configuration;
            data_->ga_config.frameLengthFlag = 0;
            data_->ga_config.dependsOnCoreCoder = 0;
            data_->ga_config.extensionFlag = 0;
        }

        void AacConfigHelper::to_adts_data(
            boost::uint32_t frame_size, 
            std::vector<boost::uint8_t> & buf) const
        {
            AacAdts adts;
            adts.syncword = 0xfff;
            adts.version = 0; // MPEG4 part 3
            adts.layer = 0;
            adts.protection_absent = 1;
            adts.profile = data_->audioObjectType - 1;
            adts.sampling_frequency_index = data_->samplingFrequencyIndex;
            adts.private_bit = 0;
            adts.channel_configuration = (boost::uint8_t)data_->channelConfiguration;
            adts.original = 0;
            adts.home = 0;
            adts.copyright_identification_bit = 0;
            adts.copyright_identification_start = 0;
            adts.frame_length = frame_size + 7;
            adts.buffer_fullness = 0x1fff;
            adts.no_raw_data_blocks_in_frame = 0;
        }

    } // namespace avformat
} // namespace ppbox
