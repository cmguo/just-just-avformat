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
            Mp4Box * box)
            : Mp4BoxWrapper<Mp4EsDescriptionBox>(box)
            , decoder_config_(NULL)
            , decoder_info_(NULL)
        {
            data_->descriptor.is<Mp4EsDescriptor>();
            Mp4EsDescriptor & es_desc(data_->descriptor.as<Mp4EsDescriptor>());
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

        Mp4EsDescription::Mp4EsDescription(
            Mp4Box * box, 
            create_new_t)
            : Mp4BoxWrapper<Mp4EsDescriptionBox>(box)
            , decoder_config_(NULL)
            , decoder_info_(NULL)
        {
            data_->descriptor.reset(Mp4EsDescriptor());
            Mp4EsDescriptor & es_desc(data_->descriptor.as<Mp4EsDescriptor>());
            es_desc.sub_descriptors.push_back(Mp4DecoderConfigDescriptor());
            es_desc.sub_descriptors.push_back(Mp4SLConfigDescriptor());
            decoder_config_ = &es_desc.sub_descriptors[0].as<Mp4DecoderConfigDescriptor>();
            decoder_config_->sub_descriptors.push_back(Mp4DecoderSpecificInfoDescriptor());
            decoder_info_ = &decoder_config_->sub_descriptors[0].as<Mp4DecoderSpecificInfoDescriptor>();
        }

        // Mp4SampleEntry

        Mp4SampleEntry::Mp4SampleEntry(
            Mp4SampleEntryBoxBase * box)
            : Mp4BoxVectorWrapper(box)
            , es_description_(NULL)
        {
            Mp4Box * es_desc = find_item("/esds");
            if (es_desc)
                es_description_ = new Mp4EsDescription(es_desc);
        }

        Mp4SampleEntry::~Mp4SampleEntry()
        {
            if (es_description_)
                delete es_description_;
        }

        Mp4EsDescription * Mp4SampleEntry::create_es_description()
        {
            assert(es_description_ == NULL);
            es_description_ = new Mp4EsDescription(create_item("/esds"), create_new_t());
            return es_description_;
        }

        // Mp4VisualSampleEntry

        Mp4VisualSampleEntry::Mp4VisualSampleEntry(
            Mp4Box & box)
            : Mp4BoxWrapper<Mp4VisualSampleEntryBox>(box)
            , Mp4SampleEntry(&data())
        {
        }

        void Mp4VisualSampleEntry::width(
            boost::uint16_t n)
        {
            data_->width = n;
        }

        void Mp4VisualSampleEntry::height(
            boost::uint16_t n)
        {
            data_->height = n;
        }

        // Mp4AudioSampleEntry

        Mp4AudioSampleEntry::Mp4AudioSampleEntry(
            Mp4Box & box)
            : Mp4BoxWrapper<Mp4AudioSampleEntryBox>(box)
            , Mp4SampleEntry(&data())
        {
        }

        void Mp4AudioSampleEntry::channel_count(
            boost::uint16_t n)
        {
            data_->channelcount = n;
        }

        void Mp4AudioSampleEntry::sample_size(
            boost::uint16_t n)
        {
            data_->samplesize = n;
        }

        void Mp4AudioSampleEntry::sample_rate(
            boost::uint32_t n)
        {
            data_->samplerate = n << 16;
        }

        // Mp4SampleDescriptionTable

        Mp4SampleDescriptionTable::Mp4SampleDescriptionTable(
            Mp4Box * box)
            : Mp4BoxWrapper2<Mp4SampleDescriptionBox>(*box)
        {
            Mp4SampleDescriptionBox & stbl(data());
            for (size_t i = 0; i <  stbl.size(); ++i) {
                Mp4Box & child = *stbl.at(i);
                if (is_entry<Mp4VisualSampleEntryBox>(child.type)) {
                    entries_.push_back(new Mp4VisualSampleEntry(child));
                } else if (is_entry<Mp4AudioSampleEntryBox>(child.type)) {
                    entries_.push_back(new Mp4AudioSampleEntry(child));
                }
            }
        }

        Mp4SampleEntry * Mp4SampleDescriptionTable::create_description(
            boost::uint32_t codec)
        {
            Mp4Box * entry = create_item(codec);
            ++data_->entry_count;
            if (is_entry<Mp4VisualSampleEntryBox>(codec)) {
                entries_.push_back(new Mp4VisualSampleEntry(*entry));
            } else if (is_entry<Mp4AudioSampleEntryBox>(codec)) {
                entries_.push_back(new Mp4AudioSampleEntry(*entry));
            }
            return entries_.back();
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
