// Mp4BoxBase.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/box/Mp4SampleDescriptionBox.h"

namespace ppbox
{
    namespace avformat
    {

        Mp4Box::id_type const Mp4VisualSampleEntryBox::codings[] = {
            MAKE_FOURC_TYPE('a', 'v', 'c', '1'), 
            0, 
        };

        Mp4Box::id_type const Mp4AudioSampleEntryBox::codings[] = {
            MAKE_FOURC_TYPE('m', 'p', '4', 'a'), 
            0, 
        };

    } // namespace avformat
} // namespace ppbox
