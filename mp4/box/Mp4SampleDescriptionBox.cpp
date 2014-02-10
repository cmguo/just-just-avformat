// Mp4BoxBase.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/box/Mp4SampleDescriptionBox.h"
#include "ppbox/avformat/mp4/box/Mp4BoxEnum.h"

namespace ppbox
{
    namespace avformat
    {

        Mp4Box::id_type const Mp4VisualSampleEntryBox::codings[] = {
            Mp4CodecType::avc1, 
            0, 
        };

        Mp4Box::id_type const Mp4AudioSampleEntryBox::codings[] = {
            Mp4CodecType::mp4a, 
            Mp4CodecType::ac_3, 
            Mp4CodecType::ec_3, 
            0, 
        };

    } // namespace avformat
} // namespace ppbox
