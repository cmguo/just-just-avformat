// Error.h

#ifndef _PPBOX_AVFORMAT_ERROR_H_
#define _PPBOX_AVFORMAT_ERROR_H_

namespace ppbox
{
    namespace avformat
    {

        namespace error {

            enum errors
            {
                format_not_support = 1, 
                codec_not_support, 
                end_of_stream, 
                bad_media_format,    // �ļ����ݵĸ�ʽ����
                file_stream_error, 
            };

            namespace detail {

                class avformat_category
                    : public boost::system::error_category
                {
                public:
                    const char* name() const
                    {
                        return "avformat";
                    }

                    std::string message(int value) const
                    {
                        switch (value) 
                        {
                            case format_not_support:
                                return "avformat: format not support";
                            case codec_not_support:
                                return "avformat: codec not support";
                            case end_of_stream:
                                return "avformat: end of stream";
                            case bad_media_format:
                                return "avformat: bad media format";
                            case file_stream_error:
                                return "avformat: file stream error";
                            default:
                                return "avformat other error";
                        }
                    }
                };

            } // namespace detail

            inline const boost::system::error_category & get_category()
            {
                static detail::avformat_category instance;
                return instance;
            }

            inline boost::system::error_code make_error_code(
                errors e)
            {
                return boost::system::error_code(
                    static_cast<int>(e), get_category());
            }

        } // namespace live_error

    } // namespace live
} // namespace ppbox

namespace boost
{
    namespace system
    {

        template<>
        struct is_error_code_enum<ppbox::avformat::error::errors>
        {
            BOOST_STATIC_CONSTANT(bool, value = true);
        };

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
        using ppbox::avformat::error::make_error_code;
#endif

    }
}

#endif // _PPBOX_PPAP_ERROR_H_
