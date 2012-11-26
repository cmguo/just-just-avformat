// AvcConfig.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/codec/avc/AvcConfig.h"
#include "ppbox/avformat/codec/avc/AvcNaluHelper.h"
#include <ppbox/avformat/codec/avc/AvcNalu.h>
#include "ppbox/avformat/BitsIStream.h"
#include "ppbox/avformat/BitsOStream.h"

#include <util/archive/ArchiveBuffer.h>

namespace ppbox
{
    namespace avformat
    {

        template <
            typename Archive
        >
        void serialize(
            Archive & ar, 
            AvcConfig & c)
        {
            ar & c.configurationVersion;
            ar & c.AVCProfileIndication;
            ar & c.profile_compatibility;
            ar & c.AVCLevelIndication;
            ar & c.reserved;
            ar & c.lengthSizeMinusOne;
            ar & c.reserved2;
            ar & c.numOfSequenceParameterSets;
            c.sequenceParameterSetLength.resize(c.numOfSequenceParameterSets);
            c.sequenceParameterSetNALUnit.resize(c.numOfSequenceParameterSets);
            for (size_t i = 0; i < c.sequenceParameterSetLength.size(); ++i) {
                ar & c.sequenceParameterSetLength[i];
                c.sequenceParameterSetNALUnit[i].resize(c.sequenceParameterSetLength[i]);
                util::serialization::serialize_collection(ar, c.sequenceParameterSetNALUnit[i], c.sequenceParameterSetNALUnit[i].size());
            }
            ar & c.numOfPictureParameterSets;
            c.pictureParameterSetLength.resize(c.numOfPictureParameterSets);
            c.pictureParameterSetNALUnit.resize(c.numOfPictureParameterSets);
            for (size_t i = 0; i < c.pictureParameterSetLength.size(); ++i) {
                ar & c.pictureParameterSetLength[i];
                c.pictureParameterSetNALUnit[i].resize(c.pictureParameterSetLength[i]);
                util::serialization::serialize_collection(ar, c.pictureParameterSetNALUnit[i], c.pictureParameterSetNALUnit[i].size());
            }
        }

        AvcConfig::AvcConfig()
        {
        }

        AvcConfig::AvcConfig(
            boost::uint8_t const * buf, 
            boost::uint32_t size)
        {
            std::vector<boost::uint8_t> vec(buf, buf + size);
            from_data(vec);
        }

        AvcConfig::AvcConfig(
            std::vector<boost::uint8_t> const & vec)
        {
            from_data(vec);
        }

        void AvcConfig::from_data(
            std::vector<boost::uint8_t> const & buf)
        {
            util::archive::ArchiveBuffer<boost::uint8_t> abuf((boost::uint8_t *)&buf[0], buf.size(), buf.size());
            ppbox::avformat::BitsIStream<boost::uint8_t> is(abuf);
            is >> *this;
        }

        void AvcConfig::to_data(
            std::vector<boost::uint8_t> & buf) const
        {
            buf.resize(16);
            util::archive::ArchiveBuffer<boost::uint8_t> abuf((boost::uint8_t *)&buf[0], buf.size());
            ppbox::avformat::BitsOStream<boost::uint8_t> os(abuf);
            os << *this;
            buf.resize(abuf.size());
        }

        void AvcConfig::from_es_data(
            std::vector<boost::uint8_t> const & buf)
        {
            std::vector<NaluBuffer> nalus;
            AvcNaluHelper::ConstBuffers buffers;
            buffers.push_back(boost::asio::buffer(buf));
            AvcNaluHelper list;
            list.from_stream(buf.size(), buffers, nalus);
            AvcNaluHelper::MyBuffersLimit limit(buffers.begin(), buffers.end());
            for (size_t i = 0; i < nalus.size(); ++i) {
                NaluHeader const nalu_header(nalus.at(i).begin.dereference_byte());
                std::vector<boost::uint8_t> nalu(nalus[i].bytes_begin(limit), nalus[i].bytes_end());
                if (nalu_header.nal_unit_type == avformat::NaluHeader::SPS) {
                    sequenceParameterSetLength.push_back(nalu.size());
                    sequenceParameterSetNALUnit.push_back(nalu);
                } else if (nalu_header.nal_unit_type == avformat::NaluHeader::PPS) {
                    pictureParameterSetLength.push_back(nalu.size());
                    pictureParameterSetNALUnit.push_back(nalu);
                }
            }
        }

        void AvcConfig::to_es_data(
            std::vector<boost::uint8_t> & buf) const
        {
            boost::uint8_t vec_0001[] = {0, 0, 0, 1};
            for (size_t i = 0; i < sequenceParameterSetNALUnit.size(); ++i) {
                buf.insert(buf.end(), vec_0001, vec_0001 + 4);
                buf.insert(buf.end(), sequenceParameterSetNALUnit[i].begin(), sequenceParameterSetNALUnit[i].end());
            }
            for (size_t i = 0; i < pictureParameterSetNALUnit.size(); ++i) {
                buf.insert(buf.end(), vec_0001, vec_0001 + 4);
                buf.insert(buf.end(), pictureParameterSetNALUnit[i].begin(), pictureParameterSetNALUnit[i].end());
            }
        }

    } // namespace avformat
} // namespace ppbox
