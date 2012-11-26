// AvcNaluHelper.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/codec/avc/AvcNaluHelper.h"
#include "ppbox/avformat/codec/avc/AvcNalu.h"

#include <util/buffers/BuffersSize.h>
#include <util/buffers/BuffersCopy.h>

#include <framework/system/BytesOrder.h>

namespace ppbox
{
    namespace avformat
    {

        bool AvcNaluHelper::from_stream(
            boost::uint32_t size, 
            ConstBuffers const & data, 
            std::vector<NaluBuffer> & nalus)
        {
            nalus.clear();
            MyFindIterator2 finder(data, boost::asio::buffer("\0\0\0\1", 4));
            MyFindIterator2 end;
            while (finder != end) {
                finder.skip_bytes(4);
                MyBuffersPosition pos = finder.position();
                NaluHeader const nalu_header(finder.position().dereference_byte());
                if (nalu_header.nal_unit_type == NaluHeader::IDR ||
                    nalu_header.nal_unit_type == NaluHeader::UNIDR) {
                        nalus.push_back(NaluBuffer(
                            size - pos.skipped_bytes(),
                            pos,
                            finder.end_position()));
                        break;
                }
                finder++;
                nalus.push_back(NaluBuffer(finder.position().skipped_bytes() - pos.skipped_bytes(), pos, finder.position()));
            }
            return true;
        }

        bool AvcNaluHelper::from_packet(
            boost::uint32_t size, 
            ConstBuffers const & data, 
            std::vector<NaluBuffer> & nalus)
        {
            nalus.clear();
            MyBuffersLimit limit(data.begin(), data.end());
            MyBuffersPosition position(limit);
            while (!position.at_end()) {
                boost::uint32_t len = 0;
                util::buffers::buffers_copy(
                    boost::asio::buffer((char *)&len + 4 - nalu_length_, nalu_length_), 
                    util::buffers::Container<boost::asio::const_buffer, MyBufferIterator>(MyBufferIterator(limit, position)));
                len = framework::system::BytesOrder::net_to_host_long(len);
                position.increment_bytes(limit, nalu_length_);
                MyBuffersPosition pos = position;
                position.increment_bytes(limit, len);
                nalus.push_back(NaluBuffer(len, pos, position));
            }
            return true;
        }

        bool AvcNaluHelper::to_stream(
            boost::uint32_t & size, 
            ConstBuffers & data, 
            std::vector<NaluBuffer> const & nalus)
        {
            util::buffers::BuffersLimit<ConstBuffers::const_iterator> limit(data.begin(), data.end());
            MyBufferIterator end;
            std::deque<boost::asio::const_buffer> datas;
            for (boost::uint32_t i = 0; i < nalus.size(); ++i) {
                NaluHeader nalu_header(nalus[i].begin.dereference_byte());
                MyBufferIterator iter(limit, nalus[i].begin, nalus[i].end);
                //datas.push_back(boost::asio::buffer(nalu_start_code_));
                datas.insert(datas.end(), iter, end);
                size += nalus[i].size + 4;
            }
            data.swap(datas);
            return true;
        }

        bool AvcNaluHelper::to_packet(
            boost::uint32_t & size, 
            ConstBuffers & data, 
            std::vector<NaluBuffer> const & nalus)
        {
            util::buffers::BuffersLimit<ConstBuffers::const_iterator> limit(data.begin(), data.end());
            if (nalus.size() == 0) {
                return true;
            }
            NaluBuffer const * nalu = NULL;
            for (boost::uint32_t i = 0; i < nalus.size(); ++i) {
                NaluHeader const nalu_header(nalus.at(i).begin.dereference_byte());
                if (nalu_header.nal_unit_type == NaluHeader::IDR 
                    || nalu_header.nal_unit_type == NaluHeader::UNIDR) {
                        nalu = &nalus.at(i);
                        break;
                }
            }
            if (nalu == NULL) {
                nalu = &nalus.front();
            }
            std::deque<boost::asio::const_buffer> datas;
            //frame_data_size_ = nalu->size;
            //frame_data_size_ = framework::system::BytesOrder::host_to_big_endian_long(frame_data_size_);
            //datas.push_back(boost::asio::buffer((boost::uint8_t*)&frame_data_size_, 4));
            MyBufferIterator iter(limit, nalu->begin, nalu->end);
            MyBufferIterator end;
            datas.insert(datas.end(), iter, end);
            data.swap(datas);
            return true;
        }

        boost::uint8_t AvcNaluHelper::get_frame_type_from_stream(
            std::vector<boost::uint8_t> const & data, 
            boost::uint32_t * offset)
        {
            ConstBuffers buffers;
            buffers.push_back(boost::asio::buffer(data));
            std::vector<NaluBuffer> nalus;
            from_stream(data.size(), buffers, nalus);
            if (nalus.size() > 0) {
                NaluHeader const nalu_header(nalus.back().begin.dereference_byte());
                if (nalu_header.nal_unit_type == 1 
                    || nalu_header.nal_unit_type == 5) {
                        if (offset) {
                            *offset = nalus.back().begin.skipped_bytes() - 4;
                        }
                        return nalu_header.nal_unit_type;
                }
            }
            return 0;
        }

    } // namespace avformat
} // namespace ppbox
