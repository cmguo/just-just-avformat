// Mp4SampleDescriptionBox.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_SAMPLE_DESCRIPTION_BOX_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_SAMPLE_DESCRIPTION_BOX_H_

#include "ppbox/avformat/mp4/box/Mp4BoxEnum.h"
#include "ppbox/avformat/mp4/box/Mp4FullBox.h"
#include "ppbox/avformat/mp4/box/Mp4Descriptor.h"

namespace ppbox
{
    namespace avformat
    {

        struct Mp4SampleEntryBoxBase
            : ppbox::avbase::ObjectDataBase
            , Mp4BoxVector
        {
            boost::uint8_t reserved[6]; // = 0;
            boost::uint16_t data_reference_index;

            Mp4SampleEntryBoxBase()
                : data_reference_index(0)
            {
                for (int i =0 ; i < 6; ++i) {
                    reserved[i] = 0;
                }
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & make_array(reserved)
                    & data_reference_index;
            }
        };

        template <typename T>
        struct Mp4SampleEntryBoxConfig
            : Mp4BoxDefine
        {
            Mp4SampleEntryBoxConfig()
            {
                Mp4Box::id_type const * p = T::codings;
                while (p && *p) {
                    Mp4Box::reg_obj<T>(*p, *this);
                    Mp4BoxVector::reg_box(*p, get_vec);
                    ++p;
                }
            }

        private:
            static Mp4BoxVector * get_vec(
                Mp4Box & box)
            {
                return static_cast<Mp4BoxVector *>(&box.as<T>());
            }
        };

        template <typename T>
        struct Mp4SampleEntryBox
            : Mp4SampleEntryBoxBase
        {
            static Mp4SampleEntryBoxConfig<T> obj_def;
        };

        template <typename T>
        Mp4SampleEntryBoxConfig<T> Mp4SampleEntryBox<T>::obj_def;

        struct Mp4PixelAspectRatioBox
            : Mp4BoxData<Mp4PixelAspectRatioBox, Mp4BoxType::pasp>
        {
            boost::uint32_t hSpacing;
            boost::uint32_t vSpacing;

            Mp4PixelAspectRatioBox()
                : hSpacing(0)
                , vSpacing(0)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & hSpacing
                    & vSpacing;
            }
        };

        struct Mp4CleanApertureBox
            : Mp4BoxData<Mp4CleanApertureBox, Mp4BoxType::clap>
        {
            boost::uint32_t cleanApertureWidthN;
            boost::uint32_t cleanApertureWidthD;
            boost::uint32_t cleanApertureHeightN;
            boost::uint32_t cleanApertureHeightD;
            boost::uint32_t horizOffN;
            boost::uint32_t horizOffD;
            boost::uint32_t vertOffN;
            boost::uint32_t vertOffD;

            Mp4CleanApertureBox()
                : cleanApertureWidthN(0)
                , cleanApertureWidthD(0)
                , cleanApertureHeightN(0)
                , cleanApertureHeightD(0)
                , horizOffN(0)
                , horizOffD(0)
                , vertOffN(0)
                , vertOffD(0)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & cleanApertureWidthN
                    & cleanApertureWidthD
                    & cleanApertureHeightN
                    & cleanApertureHeightD
                    & horizOffN
                    & horizOffD
                    & vertOffN
                    & vertOffD;
            }
        };

        struct Mp4VisualSampleEntryBox
            : Mp4SampleEntryBox<Mp4VisualSampleEntryBox>
        {
            boost::uint16_t pre_defined; // = 0;
            boost::uint16_t reserved; // = 0;
            boost::uint32_t pre_defined2[3]; // = 0;
            boost::uint16_t width;
            boost::uint16_t height;
            boost::uint32_t horizresolution; // = 0x00480000; // 72 dpi
            boost::uint32_t vertresolution; // = 0x00480000; // 72 dpi
            boost::uint32_t reserved2; // = 0;
            boost::uint16_t frame_count; // = 1;
            char compressorname[32];
            boost::uint16_t depth; // = 0x0018;
            boost::int16_t pre_defined3; // = -1;
            //Mp4CleanApertureBox clap; // optional
            //Mp4PixelAspectRatioBox pasp; // optional

            Mp4VisualSampleEntryBox()
                : pre_defined(0)
                , reserved(0)
                , width(0)
                , height(0)
                , horizresolution(0)
                , vertresolution(0)
                , reserved2(0)
                , frame_count(0)
                , depth(0)
                , pre_defined3(0)
            {
                pre_defined2[0] = pre_defined2[1] = pre_defined2[2] = 0;
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4SampleEntryBoxBase::serialize(ar);
                ar & pre_defined
                    & reserved
                    & make_array(pre_defined2)
                    & width
                    & height
                    & horizresolution
                    & vertresolution
                    & reserved2
                    & frame_count;
                ar & make_array(compressorname);
                ar & depth
                    & pre_defined3;
                Mp4BoxVector::serialize(ar);
            }

            static Mp4Box::id_type const codings[];
        };

        struct Mp4AudioSampleEntryBox
            : Mp4SampleEntryBox<Mp4AudioSampleEntryBox>
        {
            boost::uint32_t reserved[2]; // = 0;
            boost::uint16_t channelcount; // = 2;
            boost::uint16_t samplesize; // = 16;
            boost::uint16_t pre_defined; // = 0;
            boost::uint16_t reserved2; // = 0 ;
            boost::uint32_t samplerate; // = { default samplerate of media}<<16;

            Mp4AudioSampleEntryBox()
                : channelcount(0)
                , samplesize(0)
                , pre_defined(0)
                , reserved2(0)
                , samplerate(0)
            {
                reserved[0] = reserved[1] = 0;
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4SampleEntryBoxBase::serialize(ar);
                ar & make_array(reserved)
                    & channelcount
                    & samplesize
                    & pre_defined
                    & reserved2
                    & samplerate;
                Mp4BoxVector::serialize(ar);
            }

            static Mp4Box::id_type const codings[];
        };

        struct Mp4SampleDescriptionBox
            : Mp4FullBoxData<Mp4SampleDescriptionBox, Mp4BoxType::stsd>
            , Mp4BoxContainer<Mp4SampleDescriptionBox, Mp4BoxType::stsd>
        {
            boost::uint32_t entry_count;
            //for (i = 1 ; i <= entry_count ; i++){
            //    switch (handler_type){
            //        case ¡®soun¡¯: // for audio tracks
            //            Mp4AudioSampleEntryBox ;
            //            break;
            //        case ¡®vide¡¯: // for video tracks
            //            VisualSampleEntryBox();
            //            break;
            //        case ¡®hint¡¯: // Hint track
            //            HintSampleEntryBox();
            //            break;
            //        case ¡®meta¡¯: // Metadata track
            //            MetadataSampleEntryBox();
            //            break;
            //    }
            //}

            Mp4SampleDescriptionBox()
                : entry_count(0)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4FullBoxHeader::serialize(ar);
                ar & entry_count;
                Mp4BoxVector::serialize(ar);
            }
        };

        struct Mp4EsDescriptionBox
            : Mp4FullBoxData<Mp4EsDescriptionBox, Mp4BoxType::esds>
        {
            Mp4Descriptor descriptor;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                Mp4FullBoxHeader::serialize(ar);
                ar & descriptor;
            }
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_SAMPLE_DESCRIPTION_BOX_H_
