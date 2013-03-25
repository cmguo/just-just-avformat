// Sample.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/Sample.h"

#include <ppbox/data/base/MemoryLock.h>

namespace ppbox
{
    namespace avformat
    {

        Sample & Sample::operator=(
            Sample & s)
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
            data.swap(s.data);
            s.memory = NULL;

            return *this;
        }

        void Sample::append(
            Sample & s)
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
