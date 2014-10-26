// AviError.h

#ifndef _PPBOX_AVFORMAT_AVI_LIB_AVI_ERROR_H_
#define _PPBOX_AVFORMAT_AVI_LIB_AVI_ERROR_H_

namespace ppbox
{
    namespace avformat
    {

        namespace avi_error {

            enum errors
            {
                invalid_avi_head = 1, 
                invalid_avi_truck,
            };

            namespace detail {

                class avi_category
                    : public boost::system::error_category
                {
                public:
                    const char* name() const
                    {
                        return "avi";
                    }

                    std::string message(int value) const
                    {
                        switch (value) 
                        {
                            case invalid_avi_head:
                                return "invalid avi head";
                            case invalid_avi_truck:
                                return "invalid avi truck";
                            default:
                                return "avi other error";
                        }
                    }
                };

            } // namespace detail

            inline const boost::system::error_category & get_category()
            {
                static detail::avi_category instance;
                return instance;
            }

            inline boost::system::error_code make_error_code(
                errors e)
            {
                return boost::system::error_code(
                    static_cast<int>(e), get_category());
            }

        } // namespace avi_error

    } // namespace live
} // namespace ppbox

namespace boost
{
    namespace system
    {

        template<>
        struct is_error_code_enum<ppbox::avformat::avi_error::errors>
        {
            BOOST_STATIC_CONSTANT(bool, value = true);
        };
    }
}

#endif // _PPBOX_AVFORMAT_AVI_LIB_AVI_ERROR_H_
