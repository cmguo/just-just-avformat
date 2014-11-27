// Mp4SampleDescription.h

#ifndef _JUST_AVFORMAT_MP4_LIB_MP4_SAMPLE_DESCRIPTION_H_
#define _JUST_AVFORMAT_MP4_LIB_MP4_SAMPLE_DESCRIPTION_H_

#include "just/avformat/mp4/lib/Mp4BoxWrapper.h"
#include "just/avformat/mp4/box/Mp4SampleBox.h"
#include "just/avformat/mp4/box/Mp4SampleDescriptionBox.h"
#include "just/avformat/mp4/box/Mp4Descriptor.h"

namespace just
{
    namespace avformat
    {

        class Mp4EsDescription
            : public Mp4BoxWrapper<Mp4EsDescriptionBox>
        {
        public:
            Mp4EsDescription(
                Mp4Box * box);

            Mp4EsDescription(
                Mp4Box * box, 
                create_new_t);

        public:
            Mp4DecoderConfigDescriptor * decoder_config()
            {
                return decoder_config_;
            }

            Mp4DecoderSpecificInfoDescriptor * decoder_info()
            {
                return decoder_info_;
            }

            Mp4DecoderConfigDescriptor const * decoder_config() const
            {
                return decoder_config_;
            }

            Mp4DecoderSpecificInfoDescriptor const * decoder_info() const
            {
                return decoder_info_;
            }

        private:
            Mp4DecoderConfigDescriptor * decoder_config_;
            Mp4DecoderSpecificInfoDescriptor * decoder_info_;
        };

        class Mp4SampleEntry
            : public Mp4BoxVectorWrapper
        {
        public:
            Mp4SampleEntry(
                Mp4SampleEntryBoxBase * box);

            virtual ~Mp4SampleEntry();

        public:
            Mp4EsDescription const * es_description() const
            {
                return es_description_;
            }

            Mp4EsDescription * create_es_description();

        protected:
            Mp4EsDescription * es_description_;
        };

        class Mp4VisualSampleEntry
            : public Mp4BoxWrapper<Mp4VisualSampleEntryBox>
            , public Mp4SampleEntry
        {
        public:
            Mp4VisualSampleEntry(
                Mp4Box & box);

        public:
            void width(
                boost::uint16_t n);

            void height(
                boost::uint16_t n);

        public:
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
            void channel_count(
                boost::uint16_t n);

            void sample_size(
                boost::uint16_t n);

            void sample_rate(
                boost::uint32_t n);

        public:
            boost::uint16_t channel_count() const
            {
                return data().channelcount;
            }

            boost::uint16_t sample_size() const
            {
                return data().samplesize;
            }

            boost::uint32_t sample_rate() const
            {
                return data().samplerate >> 16;
            }
        };

        class Mp4SampleDescriptionTable
            : public Mp4BoxWrapper2<Mp4SampleDescriptionBox>
        {
        public:
            Mp4SampleDescriptionTable(
                Mp4Box * box);

            ~Mp4SampleDescriptionTable();

        public:
            Mp4SampleEntry const * description(
                boost::uint32_t index) const
            {
                return entries_.at(index);
            }

            Mp4SampleEntry * create_description(
                boost::uint32_t codec);

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
} // namespace just

#endif // _JUST_AVFORMAT_MP4_LIB_MP4_SAMPLE_DESCRIPTION_H_
