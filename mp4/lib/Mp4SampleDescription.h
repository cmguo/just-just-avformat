// Mp4SampleDescription.h

#ifndef _PPBOX_AVFORMAT_MP4_LIB_MP4_SAMPLE_DESCRIPTION_H_
#define _PPBOX_AVFORMAT_MP4_LIB_MP4_SAMPLE_DESCRIPTION_H_

#include "ppbox/avformat/mp4/lib/Mp4BoxWrapper.h"
#include "ppbox/avformat/mp4/box/Mp4SampleBox.h"
#include "ppbox/avformat/mp4/box/Mp4SampleDescriptionBox.h"

namespace ppbox
{
    namespace avformat
    {

        class Mp4EsDescription
        {
        public:
            Mp4EsDescription(
                Mp4EsDescriptionBox const * box);

        public:
            Mp4DecoderConfigDescriptor const * decoder_config() const
            {
                return decoder_config_;
            }

            Mp4DecoderSpecificInfoDescriptor const * decoder_info() const
            {
                return decoder_info_;
            }

        private:
            Mp4EsDescriptionBox const * box_;
            Mp4DecoderConfigDescriptor const * decoder_config_;
            Mp4DecoderSpecificInfoDescriptor const * decoder_info_;
        };

        class Mp4SampleEntry
            : public Mp4BoxVectorWrapper<Mp4SampleEntryBoxBase>
        {
        public:
            Mp4SampleEntry(
                Mp4SampleEntryBoxBase * box);

            ~Mp4SampleEntry();

        public:
            Mp4EsDescription const * es_description() const
            {
                return es_description_;
            }

        protected:
            Mp4EsDescription const * es_description_;
        };

        class Mp4VisualSampleEntry
            : public Mp4BoxWrapper<Mp4VisualSampleEntryBox>
            , public Mp4SampleEntry
        {
        public:
            Mp4VisualSampleEntry(
                Mp4Box & box);

            boost::uint16_t width() const
            {
                return data().width;
            }

            boost::uint16_t height() const
            {
                return data().height;
            }
        };

        class Mp4AudioSampleEntry
            : public Mp4BoxWrapper<Mp4AudioSampleEntryBox>
            , public Mp4SampleEntry
        {
        public:
            Mp4AudioSampleEntry(
                Mp4Box & box);

        public:
            boost::uint32_t sample_rate() const
            {
                return data().samplerate;
            }

            boost::uint16_t sample_size() const
            {
                return data().samplesize;
            }

            boost::uint16_t channel_count() const
            {
                return data().channelcount;
            }
        };

        class Mp4SampleDescriptionTable
            : public Mp4BoxWrapper<Mp4SampleDescriptionBox>
        {
        public:
            Mp4SampleDescriptionTable(
                Mp4Box * box);

        public:
            Mp4SampleEntry const & description(
                boost::uint32_t index) const
            {
                return *entries_.at(index);
            }

        private:
            template <typename T>
            static bool is_entry(
                Mp4Box::id_type type)
            {
                Mp4Box::id_type const * p = T::codings;
                while (*p && *p != type) ++p;
                return *p == type;
            }

        private:
            std::vector<Mp4SampleEntry *> entries_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_LIB_MP4_SAMPLE_DESCRIPTION_H_
