// AviStream.h

#ifndef _PPBOX_AVFORMAT_AVI_LIB_AVI_STREAM_H_
#define _PPBOX_AVFORMAT_AVI_LIB_AVI_STREAM_H_

#include "ppbox/avformat/avi/lib/AviBoxWrapper.h"
#include "ppbox/avformat/avi/box/AviStreamBox.h"

#include <ppbox/avbase/Sample.h>

namespace ppbox
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
                AviBox & box);

            AviStream(
                AviBox & box, 
                AviIndex & avi_index, 
                boost::uint32_t index);

            AviStream(
                AviBox & box, 
                boost::uint32_t id, 
                boost::uint32_t type); // create new

        public:
            bool put(
                ppbox::avbase::Sample const & sample);

            bool put_eos();

        public:
            bool fixup(
                boost::uint32_t timescale, 
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
                ppbox::avbase::Sample & sample) const;

        public:
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

            void sample_duration(
                boost::uint32_t n);

            void timescale(
                boost::uint32_t n);

            void duration(
                boost::uint64_t n);

            AviStreamFormatBox::VideoFormat const & video() const
            {
                return strf_->video;
            }

            AviStreamFormatBox::AudioFormat const & audio() const
            {
                return strf_->audio;
            }

        private:
            AviStreamHeaderBox * strh_;
            AviStreamFormatBox * strf_;
            AviIndexStream * index_;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_AVI_LIB_AVI_STREAM_H_
