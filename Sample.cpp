// Sample.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/Sample.h"

#include <ppbox/data/base/MemoryLock.h>

namespace ppbox
{
    namespace avformat
    {

        Sample::Sample()
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

        Sample::Sample(
            Sample const & s)
            : itrack(s.itrack)
            , flags(s.flags)
            , time(s.time)
            , ustime(s.ustime)
            , dts(s.dts)
            , us_delta(s.us_delta)
            , cts_delta(s.cts_delta)
            , duration(s.duration)
            , size(s.size)
            , stream_info(s.stream_info)
            , memory(s.memory)
            , context(s.context)
        {
            s.memory = NULL;
        }

        Sample & Sample::operator=(
            Sample const & s)
        {
            assert(memory == NULL);
            assert(data.empty());

            itrack = s.itrack;
            flags = s.flags;
            time = s.time;
            ustime = s.ustime;
            dts = s.dts;
            us_delta = s.us_delta;
            cts_delta = s.cts_delta;
            duration = s.duration;
            size = s.size;
            stream_info = s.stream_info;
            memory = s.memory;
            context = s.context;
            data = s.data;
            s.memory = NULL;

            return *this;
        }

        void Sample::append(
            Sample const & s)
        {
            append(s.memory);
            s.memory = NULL;
        }

        void Sample::append(
            MemoryLock * mem)
        {
            if (memory == NULL) {
                memory = mem;
            } else if (mem) {
                mem->unlink();
                memory->insert(memory, mem); // static push_back
            }
        }

    } // namespace avformat
} // namespace ppbox
