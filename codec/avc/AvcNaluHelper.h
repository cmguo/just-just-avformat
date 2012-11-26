// AvcNaluHelper.h

#ifndef _PPBOX_AVFORMAT_CODEC_AVC_AVC_NALU_HELPER_H_
#define _PPBOX_AVFORMAT_CODEC_AVC_AVC_NALU_HELPER_H_

#include <util/buffers/BuffersFind.h>

namespace ppbox
{
    namespace avformat
    {

        struct NaluBufferDefine
        {
            typedef std::deque<boost::asio::const_buffer> ConstBuffers;

            typedef util::buffers::BuffersLimit<ConstBuffers::const_iterator> MyBuffersLimit;

            typedef util::buffers::BuffersPosition<
                ConstBuffers::value_type, 
                ConstBuffers::const_iterator
            > MyBuffersPosition;

            typedef util::buffers::BuffersFindIterator2<
                ConstBuffers, 
                boost::asio::const_buffers_1
            > MyFindIterator2;

            typedef MyFindIterator2::ByteIterator MyByteIterator;

            typedef MyFindIterator2::BuffersIterator MyBufferIterator;

            typedef util::buffers::Container<boost::asio::const_buffer, MyBufferIterator> MyBuffers;

            typedef util::buffers::Container<MyBuffersPosition::Byte, MyByteIterator> MyBytes;
        };

        struct NaluBuffer
            : NaluBufferDefine
        {
            NaluBuffer(
                size_t t,
                MyBuffersPosition const & b,
                MyBuffersPosition const & e)
                : size(t)
                , begin(b)
                , end(e)
            {
            }

            MyBufferIterator buffers_begin(
                MyBuffersLimit const & limit)
            {
                return MyBufferIterator(limit, begin, end);
            }

            MyBufferIterator buffers_end()
            {
                return MyBufferIterator();
            }

            MyByteIterator bytes_begin(
                MyBuffersLimit const & limit)
            {
                return MyByteIterator(limit, begin, end);
            }

            MyByteIterator bytes_end()
            {
                return MyByteIterator();
            }

            MyBuffers buffers(
                MyBuffersLimit const & limit)
            {
                return MyBuffers(MyBufferIterator(limit, begin, end));
            }

            MyBytes bytes(
                MyBuffersLimit const & limit)
            {
                return MyBytes(MyByteIterator(limit, begin, end));
            }

            size_t size;
            MyBuffersPosition begin;
            MyBuffersPosition end;
        };

        class AvcNaluHelper
            : public NaluBufferDefine
        {
        public:
            bool from_stream(
                boost::uint32_t size, 
                ConstBuffers const & data, 
                std::vector<NaluBuffer> & nalus);

            bool from_packet(
                boost::uint32_t size, 
                ConstBuffers const & data, 
                std::vector<NaluBuffer> & nalus);

            bool from_stream(
                ConstBuffers const & data, 
                std::vector<NaluBuffer> & nalus);

            bool from_packet(
                ConstBuffers const & data, 
                std::vector<NaluBuffer> & nalus);

        public:
            bool to_stream(
                boost::uint32_t & size, 
                ConstBuffers & data, 
                std::vector<NaluBuffer> const & nalus);

            bool to_packet(
                boost::uint32_t & size, 
                ConstBuffers & data, 
                std::vector<NaluBuffer> const & nalus);

            bool to_stream(
                ConstBuffers & data, 
                std::vector<NaluBuffer> const & nalus);

            bool to_packet(
                ConstBuffers & data, 
                std::vector<NaluBuffer> const & nalus);

        public:
            // return 0 if not found
            boost::uint8_t get_frame_type_from_stream(
                std::vector<boost::uint8_t> const & data, 
                boost::uint32_t * offset);

        private:
            size_t total_size;
            boost::uint8_t nalu_length_;
            std::vector<boost::uint8_t> buffer_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_CODEC_AVC_AVC_NALU_HELPER_H_
