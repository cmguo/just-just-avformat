// Mp4SampleDescription.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/lib/Mp4SampleDescription.h"
#include "ppbox/avformat/mp4/box/Mp4Box.hpp"
#include "ppbox/avformat/mp4/box/Mp4BoxVector.hpp"

namespace ppbox
{
    namespace avformat
    {

        // Mp4EsDescription

        Mp4EsDescription::Mp4EsDescription(
            Mp4EsDescriptionBox const * box)
            : box_(box)
            , decoder_config_(NULL)
            , decoder_info_(NULL)
        {
            box_->descriptor.is<Mp4EsDescriptor>();
            Mp4EsDescriptor const & es_desc(box_->descriptor.as<Mp4EsDescriptor>());
            for (size_t i = 0; i < es_desc.sub_descriptors.size(); ++i) {
                if (es_desc.sub_descriptors[i].is<Mp4DecoderConfigDescriptor>()) {
                    decoder_config_ = &es_desc.sub_descriptors[i].as<Mp4DecoderConfigDescriptor>();
                    for (size_t i = 0; i < decoder_config_->sub_descriptors.size(); ++i) {
                        if (decoder_config_->sub_descriptors[i].is<Mp4DecoderSpecificInfoDescriptor>()) {
                            decoder_info_ = &decoder_config_->sub_descriptors[i].as<Mp4DecoderSpecificInfoDescriptor>();
                        }
                    }
                }
            }
        }

        // Mp4SampleEntry

        Mp4SampleEntry::Mp4SampleEntry(
            Mp4SampleEntryBoxBase * box)
            : Mp4BoxVectorWrapper<Mp4SampleEntryBoxBase>(box)
            , es_description_(NULL)
        {
            Mp4EsDescriptionBox const * es_desc = find_item_as<Mp4EsDescriptionBox>("/esds");
            if (es_desc)
                es_description_ = new Mp4EsDescription(es_desc);
        }

        Mp4SampleEntry::~Mp4SampleEntry()
        {
            if (es_description_)
                delete es_description_;
        }

        // Mp4VisualSampleEntry

        Mp4VisualSampleEntry::Mp4VisualSampleEntry(
            Mp4Box & box)
            : Mp4BoxWrapper<Mp4VisualSampleEntryBox>(box)
            , Mp4SampleEntry(&data())
        {
        }


        // Mp4AudioSampleEntry

        Mp4AudioSampleEntry::Mp4AudioSampleEntry(
            Mp4Box & box)
            : Mp4BoxWrapper<Mp4AudioSampleEntryBox>(box)
            , Mp4SampleEntry(&data())
        {
        }

        // Mp4SampleDescriptionTable

        Mp4SampleDescriptionTable::Mp4SampleDescriptionTable(
            Mp4Box * box)
            : Mp4BoxWrapper<Mp4SampleDescriptionBox>(*box)
        {
            Mp4SampleDescriptionBox & stbl(data());
            for (size_t i = 0; i <  stbl.size(); ++i) {
                Mp4Box & child = stbl.at(i);
                if (is_entry<Mp4VisualSampleEntryBox>(child.type)) {
                    entries_.push_back(new Mp4VisualSampleEntry(child));
                } else if (is_entry<Mp4AudioSampleEntryBox>(child.type)) {
                    entries_.push_back(new Mp4AudioSampleEntry(child));
                }
            }
        }

        static void delete_entry(
            Mp4SampleEntry * entry)
        {
            delete entry;
        }

        Mp4SampleDescriptionTable::~Mp4SampleDescriptionTable()
        {
            std::for_each(entries_.begin(), entries_.end(), delete_entry);
            entries_.clear();
        }

    } // namespace avformat
} // namespace ppbox
