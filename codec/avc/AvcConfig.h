// AvcConfig.h

#ifndef _PPBOX_AVFORMAT_CODEC_AVC_AVC_CONFIG_H_
#define _PPBOX_AVFORMAT_CODEC_AVC_AVC_CONFIG_H_

#include "ppbox/avformat/BitsType.h"

namespace ppbox
{
    namespace avformat
    {

        // AVC Decoder Configuration Record

        class AvcConfig
        {
        public:
            U<8> configurationVersion;
            U<8> AVCProfileIndication;
            U<8> profile_compatibility;
            U<8> AVCLevelIndication;
            U<6> reserved;
            U<2> lengthSizeMinusOne;
            U<3> reserved2;
            U<5> numOfSequenceParameterSets;
            // for (i=0; i< numOfSequenceParameterSets; i++)
                std::vector<U<16> > sequenceParameterSetLength;
                std::vector<std::vector<boost::uint8_t> > sequenceParameterSetNALUnit;
            U<5> numOfPictureParameterSets;
                // for (i=0; i< numOfPictureParameterSets; i++)
                std::vector<U<16> > pictureParameterSetLength;
                std::vector<std::vector<boost::uint8_t> > pictureParameterSetNALUnit;

        public:
            AvcConfig();

            AvcConfig(
                boost::uint8_t const * buf, 
                boost::uint32_t size);

            AvcConfig(
                std::vector<boost::uint8_t> const & buf);

        public:
            void from_data(
                std::vector<boost::uint8_t> const & buf);

            void to_data(
                std::vector<boost::uint8_t> & buf) const;

        public:
            void from_es_data(
                std::vector<boost::uint8_t> const & buf);

            void to_es_data(
                std::vector<boost::uint8_t> & buf) const;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_CODEC_AVC_AVC_CONFIG_H_
