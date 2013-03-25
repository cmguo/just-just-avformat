// Sample.h

#ifndef _PPBOX_AVFORMAT_SAMPLE_H_
#define _PPBOX_AVFORMAT_SAMPLE_H_

#include "ppbox/avformat/StreamInfo.h"

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

        struct Sample
        {
            Sample()
                : itrack(0)
                , flags(0)
                , time(0)
                , ustime(0)
                , dts(0)
                , us_delta(0)
                , cts_delta(0)
                , duration(0)
                , size(0)
                , stream_info(NULL)
                , memory(NULL)
                , context(NULL)
            {
            }

            enum FlagEnum
            {
                 sync = 1, 
                 discontinuity = 2,
                 config = 4, 
            };

            Sample & operator=(
                Sample & s);

            void append(
                Sample & s);

            void append(
                MemoryLock * mem);

            boost::uint32_t itrack;
            boost::uint32_t flags;
            boost::uint64_t time;
            boost::uint64_t ustime;
            boost::uint64_t dts;
            boost::uint32_t us_delta; //pts-dts
            boost::uint32_t cts_delta;
            boost::uint32_t duration;
            boost::uint32_t size;
            StreamInfo const * stream_info;
            MemoryLock * memory; // 内存锁
            void * context; // 处理上下文
            std::deque<boost::asio::const_buffer> data;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_SAMPLE_H_
