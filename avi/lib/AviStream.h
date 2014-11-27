// AviStream.h

#ifndef _JUST_AVFORMAT_AVI_LIB_AVI_STREAM_H_
#define _JUST_AVFORMAT_AVI_LIB_AVI_STREAM_H_

#include "just/avformat/avi/lib/AviBoxWrapper.h"
#include "just/avformat/avi/box/AviStreamBox.h"

#include <just/avbase/Sample.h>

namespace just
{
    namespace avformat
    {

        class AviIndex;
        class AviIndexStream;

        class AviStream
            : public AviBoxWrapper2<AviStreamBox>
        {
        public:
            AviStream(
                AviBox & box, 
                AviIndex & avi_index, 
                boost::uint32_t index);

            AviStream(
                AviBox & box, 
                AviIndex & avi_index, 
                boost::uint32_t index, 
                boost::uint32_t type); // create new

        public:
            bool put(
                just::avbase::Sample const & sample);

            bool put_eos();

        public:
            bool fixup(
                boost::system::error_code & ec);

        public:
            bool merge(
                AviStream const & track, 
                boost::system::error_code & ec);

            void shift(
                boost::int64_t offset);

        public:
            bool next(
                boost::system::error_code & ec);

            bool seek(
                boost::uint64_t & time, // dts
                boost::system::error_code & ec);

            void rewind();

            bool limit(
                boost::uint64_t offset, 
                boost::uint64_t & time, // dts
                boost::system::error_code & ec) const;

        public:
            void get(
                just::avbase::Sample & sample) const;

        public:
            boost::uint32_t id(
                boost::uint32_t index) const;

            boost::uint32_t type() const
            {
                return strh_->dwType;
            }

            boost::uint32_t handler() const
            {
                return strh_->dwHandler;
            }

            boost::uint32_t sample_duration() const
            {
                return strh_->dwScale;
            }

            boost::uint32_t timescale() const
            {
                return strh_->dwRate;
            }

            boost::uint64_t duration() const;

            void handler(
                boost::uint32_t n);

            void sample_duration(
                boost::uint32_t n);

            void timescale(
                boost::uint32_t n);

            void duration(
                boost::uint64_t n);

        public:
            AviStreamFormatBox::VideoFormat & video()
            {
                return strf_->video;
            }

            AviStreamFormatBox::AudioFormat & audio()
            {
                return strf_->audio;
            }

        private:
            AviStreamHeaderBox * strh_;
            AviStreamFormatBox * strf_;
            AviIndexStream * index_;
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_AVI_LIB_AVI_STREAM_H_
