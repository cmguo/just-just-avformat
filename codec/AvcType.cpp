// AvcType.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/codec/AvcType.h"

namespace ppbox
{
    namespace avformat
    {

       char const * const SliceHeader::slice_type_str[] = {
           "P", "B", "I", "SP", "SI", 
           "P2", "B2", "I2", "SP2", "SI2", 
       };

    } // namespace avformat
} // namespace ppbox
