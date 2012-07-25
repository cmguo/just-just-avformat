// AvcConfig.h

#ifndef      _PPBOX_AVFORMAT_CODEC_AVC_CONFIG_
#define      _PPBOX_AVFORMAT_CODEC_AVC_CONFIG_

namespace ppbox
{
    namespace avformat
    {
        static boost::uint16_t uint16BytesToUInt16BE(boost::uint8_t *src)
        {
            boost::uint8_t buffer[2];
            memcpy(buffer, src, 2);
            return (((boost::uint16_t)buffer[0])<<8) | (((boost::uint16_t)buffer[1]));
        }

        static void UInt16Touint16BytesBE(boost::uint16_t src, boost::uint8_t * obj)
        {
            memcpy(obj, &src, 2);
            boost::uint8_t tmp = *obj;
            *obj = *(obj+ 1);
            *(obj+ 1) = tmp;
        }

        typedef std::vector< boost::uint8_t > Item;
        typedef std::vector< Item > Buffer_Array;
        class AvcConfig
        {
        public:
            AvcConfig()
                : buffer_(NULL)
                , size_(0)
            {
            }

            AvcConfig(boost::uint8_t const * buf, boost::uint32_t size)
                : buffer_((boost::uint8_t *)malloc(size))
                , size_(0)
            {
                memcpy(buffer_, buf, size);
                size_ = size;
            }

            AvcConfig(boost::uint32_t buffer_size)
                : buffer_((boost::uint8_t *)malloc(buffer_size))
                , size_(buffer_size)
            {
            }

            ~AvcConfig()
            {
                if (buffer_) {
                    free(buffer_);
                    buffer_ = NULL;
                }

            }

            void set_buffer(boost::uint8_t const * buffer, boost::uint32_t size)
            {
                buffer_ = (boost::uint8_t *)malloc(size);
                memcpy(buffer_, buffer, size);
                size_ = size;
            }

            bool creat(void)
            {
                bool res = true;
                boost::uint32_t position = 0;
                // avc config 的长度至少为6的字节
                if (size_ > 6) {
                    configuration_version_ = *(buffer_ + position); ++position;
                    profile_               = *(buffer_ + position); ++position;
                    level_                 = *(buffer_ + position); ++position;
                    profile_compatibility_ = *(buffer_ + position); ++position;
                    boost::uint8_t length_size_minus_one;
                    length_size_minus_one  = *(buffer_ + position); ++position;
                    nalu_lengthSize_       = 1 + (length_size_minus_one&3);

                    boost::uint8_t num_seq_params;
                    num_seq_params = *(buffer_ + position); ++position;
                    num_seq_params &= 31;

                    for (unsigned int i=0; i < num_seq_params; i++) {
                        boost::uint16_t param_length;
                        if ((size_ - position) > 2) {
                            param_length = uint16BytesToUInt16BE(buffer_ + position);
                            ++position; ++position;
                            if ((size_ - position) >= param_length) {
                                Item item;
                                boost::uint8_t * begin = buffer_ + position;
                                boost::uint8_t * end   = begin + param_length;
                                item.assign(begin, end);
                                sequence_parameters_.push_back(item);
                                position = position + param_length;
                                begin = NULL;
                                end   = NULL;
                            } else {
                                res = false;
                                break;
                            }
                        } else {
                            res = false;
                            break;
                        }
                    }

                    if ((size_ - position) > 1 && res != false) {
                        boost::uint8_t num_pic_params = *(buffer_ + position); ++position;
                        for (unsigned int i=0; i < num_pic_params; i++) {
                            boost::uint16_t param_length;
                            if ((size_ - position) > 2) {
                                param_length = uint16BytesToUInt16BE(buffer_ + position);
                                ++position; ++position;
                                if ((size_ - position) >= param_length) {
                                    Item item;
                                    boost::uint8_t * begin = buffer_ + position;
                                    boost::uint8_t * end   = begin + param_length;
                                    item.assign(begin, end);
                                    picture_parameters_.push_back(item);
                                    position = position + param_length;
                                    begin = NULL;
                                    end   = NULL;
                                } else {
                                    res = false;
                                    break;
                                }
                            } else {
                                res = false;
                                break;
                            }
                        }
                    }
                } else {
                    res = false;
                }
                return res;
            }

            bool creat(
                boost::uint8_t version,
                boost::uint8_t profile,
                boost::uint8_t level,
                boost::uint8_t profile_compatibility,
                boost::uint8_t nalu_lengthSize,
                Buffer_Array   spss,
                Buffer_Array   ppss)
            {
                boost::uint32_t position = 0;
                *(buffer_ + position) = version;   ++position;
                *(buffer_ + position) = profile;   ++position;
                *(buffer_ + position) = level;     ++position;
                *(buffer_ + position) = profile_compatibility;     ++position;
                nalu_lengthSize--;
                *(buffer_ + position) = (nalu_lengthSize | 0xFC);  ++position;
                boost::uint8_t sps_size = (boost::uint8_t)spss.size();
                *(buffer_ + position) = (0xE0 | sps_size);     ++position;
                for(boost::uint32_t i = 0; i < spss.size(); i++) {
                    boost::uint16_t param_size = (boost::uint16_t)spss[i].size();
                    boost::uint8_t param_size_be[2];
                    UInt16Touint16BytesBE(param_size, param_size_be);
                    memcpy(buffer_ + position, param_size_be, 2);
                    ++position; ++position;
                    memcpy(buffer_ + position, &spss[i].at(0), spss[i].size());
                    position = position + spss[i].size();
                }

                boost::uint8_t pps_size = (boost::uint8_t)ppss.size();
                *(buffer_ + position) = pps_size;     ++position;
                for(boost::uint32_t i = 0; i < ppss.size(); i++) {
                    boost::uint16_t param_size = (boost::uint16_t)ppss[i].size();
                    boost::uint8_t param_size_be[2];
                    UInt16Touint16BytesBE(param_size, param_size_be);
                    memcpy(buffer_ + position, param_size_be, 2);
                    ++position; ++position;
                    memcpy(buffer_ + position, &ppss[i].at(0), ppss[i].size());
                    position = position + ppss[i].size();
                }
                size_ = position;
                return true;
            }

            void set(
                boost::uint8_t version,
                boost::uint8_t profile,
                boost::uint8_t level,
                boost::uint8_t profile_compatibility,
                boost::uint8_t nalu_lengthSize,
                Buffer_Array   spss,
                Buffer_Array   ppss)
            {
                configuration_version_ = version;
                profile_               = profile;
                level_                 = level;
                profile_compatibility_ = profile_compatibility;
                nalu_lengthSize_       = nalu_lengthSize;
                sequence_parameters_   = spss;
                picture_parameters_    = ppss;
            }

            boost::uint8_t configuration_version(void) const
            {
                return configuration_version_;
            }

            boost::uint8_t profile(void) const
            {
                return profile_;
            }

            boost::uint8_t level(void) const
            {
                return level_;
            }

            boost::uint8_t profile_compatibility(void) const
            {
                return profile_compatibility_;
            }

            boost::uint8_t nalu_lengthSize(void) const
            {
                return nalu_lengthSize_;
            }

            Buffer_Array const & sequence_parameters(void) const
            {
                return sequence_parameters_;
            }

            Buffer_Array const & picture_parameters(void) const
            {
                return picture_parameters_;
            }

            boost::uint8_t * data(void)
            {
                return buffer_;
            }

            boost::uint32_t data_size(void)
            {
                return size_;
            }

        private:
            boost::uint8_t      * buffer_;
            boost::uint32_t     size_;
            boost::uint8_t      configuration_version_;
            boost::uint8_t      profile_;
            boost::uint8_t      level_;
            boost::uint8_t      profile_compatibility_;
            boost::uint8_t      nalu_lengthSize_;
            Buffer_Array        sequence_parameters_;
            Buffer_Array        picture_parameters_;
        };
    }
}

#endif // _PPBOX_AVFORMAT_CODEC_AVC_CONFIG_
