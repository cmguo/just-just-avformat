// Mp4Error.h

#ifndef _JUST_AVFORMAT_MP4_LIB_MP4_ERROR_H_
#define _JUST_AVFORMAT_MP4_LIB_MP4_ERROR_H_

namespace just
{
    namespace avformat
    {

        namespace mp4_error {

            enum errors
            {
                invalid_mp4_head = 1, 
                invalid_mp4_truck,
            };

            namespace detail {

                class mp4_category
                    : public boost::system::error_category
                {
                public:
                    const char* name() const BOOST_SYSTEM_NOEXCEPT
                    {
                        return "mp4";
                    }

                    std::string message(int value) const
                    {
                        switch (value) 
                        {
                            case invalid_mp4_head:
                                return "invalid mp4 head";
                            case invalid_mp4_truck:
                                return "invalid mp4 truck";
                            default:
                                return "mp4 other error";
                        }
                    }
                };

            } // namespace detail

            inline const boost::system::error_category & get_category()
            {
                static detail::mp4_category instance;
                return instance;
            }

            inline boost::system::error_code make_error_code(
                errors e)
            {
                return boost::system::error_code(
                    static_cast<int>(e), get_category());
            }

        } // namespace mp4_error

    } // namespace live
} // namespace just

namespace boost
{
    namespace system
    {

        template<>
        struct is_error_code_enum<just::avformat::mp4_error::errors>
        {
            BOOST_STATIC_CONSTANT(bool, value = true);
        };
    }
}

#endif // _JUST_AVFORMAT_MP4_LIB_MP4_ERROR_H_
