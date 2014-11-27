// Mp2CrcBuffer.h

#ifndef _JUST_AVFORMAT_MP2_MP2_CRC_BUFFER_H_
#define _JUST_AVFORMAT_MP2_MP2_CRC_BUFFER_H_

#include <streambuf>

namespace just
{
    namespace avformat
    {

        class Mp2Crc
        {
        public:
            Mp2Crc();

        public:
            boost::uint32_t value() const
            {
                return crc_;
            }

        public:
            void put(
                boost::uint8_t c);

            void put(
                boost::uint8_t * buf, 
                size_t size);

        private:
            static boost::uint32_t const crc_table[256];

        private:
            boost::uint32_t crc_;
        };

        class Mp2CrcBuffer
            : public std::basic_streambuf<boost::uint8_t>
        {
        public:
            Mp2CrcBuffer(
                std::basic_streambuf<boost::uint8_t> & next_layer);

        public:
            std::basic_streambuf<boost::uint8_t> & next_layer()
            {
                return next_layer_;
            }

            boost::uint32_t crc_in() const
            {
                return crc_in_.value();
            }

            boost::uint32_t crc_out() const
            {
                return crc_out_.value();
            }

        private:
            virtual int_type underflow();

            virtual int_type overflow(
                int_type c);

            virtual pos_type seekoff(
                off_type off, 
                std::ios_base::seekdir dir,
                std::ios_base::openmode mode)
            {
                return next_layer_.pubseekoff(off, dir, mode);
            }

            virtual pos_type seekpos(
                pos_type position, 
                std::ios_base::openmode mode)
            {
                return next_layer_.pubseekpos(position, mode);
            }

        private:
            std::basic_streambuf<boost::uint8_t> & next_layer_;
            char_type byte_in_;
            char_type byte_out_;
            Mp2Crc crc_in_;
            Mp2Crc crc_out_;
        };

        template <typename Archive>
        struct Mp2AutoCrc
        {
            Mp2AutoCrc(
                Archive & ar, 
                boost::uint32_t & crc)
                : ar_(ar)
                , crc_(crc)
                , buf_(*ar.rdbuf())
            {
                ar_.rdbuf(&buf_);
            }

            ~Mp2AutoCrc()
            {
                ar_.rdbuf(&buf_.next_layer());
                check((typename Archive::is_saving *)0);
            }

        private:
            // save
            void check(
                boost::mpl::true_ *)
            {
                crc_ = buf_.crc_out();
                ar_ << crc_;
            }

            // load
            void check(
                boost::mpl::false_ *)
            {
                ar_ >> crc_;
                if (crc_ != buf_.crc_in()) {
                    ar_.fail();
                }
            }

        private:
            Archive & ar_;
            boost::uint32_t & crc_;
            Mp2CrcBuffer buf_;
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MP2_MP2_CRC_BUFFER_H_
