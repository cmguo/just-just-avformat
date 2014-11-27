// Mp4BoxBase.cpp

#include "just/avformat/Common.h"
#include "just/avformat/mp4/box/Mp4SampleDescriptionBox.h"
#include "just/avformat/mp4/box/Mp4BoxEnum.h"

namespace just
{
    namespace avformat
    {

        Mp4Box::id_type const Mp4VisualSampleEntryBox::codings[] = {
            Mp4CodecType::avc1, 
            Mp4CodecType::avc2, 
            Mp4CodecType::avc3, 
            Mp4CodecType::avc4, 
            Mp4CodecType::hvc1, 
            Mp4CodecType::hev1, 
            0, 
        };

        Mp4Box::id_type const Mp4AudioSampleEntryBox::codings[] = {
            Mp4CodecType::mp4a, 
            Mp4CodecType::ac_3, 
            Mp4CodecType::ec_3, 
            0, 
        };

    } // namespace avformat
} // namespace just
