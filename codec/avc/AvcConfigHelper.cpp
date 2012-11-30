// AvcConfigHelper.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/codec/avc/AvcConfigHelper.h"
#include "ppbox/avformat/codec/avc/AvcConfig.h"
#include "ppbox/avformat/codec/avc/AvcNaluHelper.h"
#include <ppbox/avformat/codec/avc/AvcNalu.h>
#include "ppbox/avformat/stream/BitsIStream.h"
#include "ppbox/avformat/stream/BitsOStream.h"
#include "ppbox/avformat/stream/FormatBuffer.h"

namespace ppbox
{
    namespace avformat
    {

        AvcConfigHelper::AvcConfigHelper()
            : data_(new AvcConfig)
        {
        }

        AvcConfigHelper::AvcConfigHelper(
            boost::uint8_t const * buf, 
            boost::uint32_t size)
            : data_(new AvcConfig)
        {
            std::vector<boost::uint8_t> vec(buf, buf + size);
            from_data(vec);
        }

        AvcConfigHelper::AvcConfigHelper(
            std::vector<boost::uint8_t> const & vec)
            : data_(new AvcConfig)
        {
            from_data(vec);
        }

        AvcConfigHelper::~AvcConfigHelper()
        {
            delete data_;
        }

        void AvcConfigHelper::from_data(
            std::vector<boost::uint8_t> const & buf)
        {
            FormatBuffer abuf((boost::uint8_t *)&buf[0], buf.size(), buf.size());
            BitsIStream<boost::uint8_t> is(abuf);
            is >> *data_;
        }

        void AvcConfigHelper::to_data(
            std::vector<boost::uint8_t> & buf) const
        {
            buf.resize(256);
            FormatBuffer abuf((boost::uint8_t *)&buf[0], buf.size());
            BitsOStream<boost::uint8_t> os(abuf);
            os << *data_;
            buf.resize(abuf.size());
        }

        void AvcConfigHelper::from_es_data(
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
                    data_->sequenceParameterSetLength.push_back(nalu.size());
                    data_->sequenceParameterSetNALUnit.push_back(nalu);
                } else if (nalu_header.nal_unit_type == avformat::NaluHeader::PPS) {
                    data_->pictureParameterSetLength.push_back(nalu.size());
                    data_->pictureParameterSetNALUnit.push_back(nalu);
                }
            }
            data_->configurationVersion = 1;
            data_->AVCProfileIndication = data_->sequenceParameterSetNALUnit[0][1];
            data_->profile_compatibility = 0;
            data_->AVCLevelIndication = data_->sequenceParameterSetNALUnit[0][3];
            data_->reserved = 0x3f;
            data_->lengthSizeMinusOne = 3;
            data_->reserved2 = 0x7;
            data_->numOfSequenceParameterSets = data_->sequenceParameterSetNALUnit.size();
            data_->numOfPictureParameterSets = data_->pictureParameterSetNALUnit.size();
        }

        void AvcConfigHelper::to_es_data(
            std::vector<boost::uint8_t> & buf) const
        {
            boost::uint8_t vec_0001[] = {0, 0, 0, 1};
            for (size_t i = 0; i < data_->sequenceParameterSetNALUnit.size(); ++i) {
                buf.insert(buf.end(), vec_0001, vec_0001 + 4);
                buf.insert(buf.end(), data_->sequenceParameterSetNALUnit[i].begin(), data_->sequenceParameterSetNALUnit[i].end());
            }
            for (size_t i = 0; i < data_->pictureParameterSetNALUnit.size(); ++i) {
                buf.insert(buf.end(), vec_0001, vec_0001 + 4);
                buf.insert(buf.end(), data_->pictureParameterSetNALUnit[i].begin(), data_->pictureParameterSetNALUnit[i].end());
            }
        }

    } // namespace avformat
} // namespace ppbox
