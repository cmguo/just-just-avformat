// AvcNaluBuffer.h

#ifndef _PPBOX_AVFORMAT_CODEC_AVC_AVC_NALU_BUFFER_H_
#define _PPBOX_AVFORMAT_CODEC_AVC_AVC_NALU_BUFFER_H_

#include "ppbox/avformat/stream/SampleBuffers.h"

namespace ppbox
{
    namespace avformat
    {

        struct NaluBuffer
            : SampleBuffers
        {
            NaluBuffer(
                size_t t,
                BuffersPosition const & b,
                BuffersPosition const & e)
                : size(t)
                , begin(b)
                , end(e)
            {
            }

            BufferIterator buffers_begin() const
            {
                return BufferIterator(begin, end);
            }

            BufferIterator buffers_end() const
            {
                return BufferIterator();
            }

            ByteIterator bytes_begin() const
            {
                return ByteIterator(begin, end);
            }

            ByteIterator bytes_end() const
            {
                return ByteIterator();
            }

            RangeBuffers buffers() const
            {
                return SampleBuffers::range_buffers(begin, end);
            }

            RangeBytes bytes() const
            {
                return SampleBuffers::range_bytes(begin, end);
            }

            boost::uint32_t size;
            BuffersPosition begin;
            BuffersPosition end;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_CODEC_AVC_AVC_NALU_BUFFER_H_
