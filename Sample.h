// Sample.h

#ifndef _PPBOX_AVFORMAT_SAMPLE_H_
#define _PPBOX_AVFORMAT_SAMPLE_H_

#include <boost/asio/buffer.hpp>

namespace ppbox
{
    namespace data
    {
        struct MemoryLock;
    }

    namespace avformat
    {

        using ppbox::data::MemoryLock;

        struct StreamInfo;

        struct Sample
        {
            enum FlagEnum
            {
                 f_sync = 1, 
                 f_discontinuity = 2,
                 f_config = 4, 
            };

            Sample();

            Sample(
                Sample const & s);

            Sample & operator=(
                Sample const & s);

            void slice(
                Sample const & s);

            void append(
                Sample const & s);

            void append(
                MemoryLock * mem);

            boost::uint32_t itrack;
            boost::uint32_t flags;
            boost::uint64_t time;
            boost::uint64_t dts;
            boost::uint32_t cts_delta;
            boost::uint32_t duration;
            boost::uint32_t size;
            StreamInfo const * stream_info;
            mutable MemoryLock * memory; // 内存锁
            void const * context; // 处理上下文
            std::deque<boost::asio::const_buffer> data;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_SAMPLE_H_
