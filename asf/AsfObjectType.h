// AsfObjectType.h

#ifndef _JUST_AVFORMAT_ASF_ASF_OBJECT_TYPE_H_
#define _JUST_AVFORMAT_ASF_ASF_OBJECT_TYPE_H_

#include "just/avformat/asf/AsfGuid.h"
#include "just/avformat/FormatBuffer.h"

#include <util/archive/LittleEndianBinaryIArchive.h>
#include <util/archive/LittleEndianBinaryOArchive.h>
#include <util/serialization/stl/vector.h>
#include <util/serialization/SplitMember.h>

#include <framework/string/Uuid.h>

namespace framework
{
    namespace string
    {

        template <typename Archive>
        void serialize(Archive & ar, Uuid & t)
        {
            util::serialization::split_free(ar, t);
        }

        template <typename Archive>
        void load(Archive & ar, Uuid & t)
        {
            UUID guid;
            ar & guid.Data1
                & guid.Data2
                & guid.Data3
                & framework::container::make_array(guid.Data4, sizeof(guid.Data4));
            if (ar)
                t.assign(guid);
        };

        template <typename Archive>
        void save(Archive & ar, Uuid const & t)
        {
            UUID const & guid = t.data();
            ar & guid.Data1
                & guid.Data2
                & guid.Data3
                & framework::container::make_array(guid.Data4, sizeof(guid.Data4));
        }
    }
}

namespace just
{
    namespace avformat
    {

        typedef util::archive::LittleEndianBinaryOArchive<boost::uint8_t> AsfOArchive;
        typedef util::archive::LittleEndianBinaryIArchive<boost::uint8_t> AsfIArchive;

        typedef framework::string::Uuid AsfUuid;
        typedef framework::string::UUID ASFUUID;

        template <typename Archive>
        void serialize_length(
            Archive & ar, 
            boost::uint32_t & t, 
            size_t m)
        {
            switch (m) {
                case 0:
                    t = 0;
                    break;
                case 1:
                    {
                        boost::uint8_t tt = (boost::uint8_t)t;
                        ar & tt;
                        t = tt;
                    }
                    break;
                case 2:
                    {
                        boost::uint16_t tt = (boost::uint16_t)t;
                        ar & tt;
                        t = tt;
                    }
                    break;
                case 3:
                    ar & t;
                    break;
                default:
                    assert(0);
                    break;
            }
        }

        template <typename Archive>
        struct save_archive_tell
        {
            static inline std::streampos invoke(
                Archive & ar)
            {
                return ar.tellp();
            }
        };

        template <typename Archive>
        struct load_archive_tell
        {
            static inline std::streampos invoke(
                Archive & ar)
            {
                return ar.tellg();
            }
        };

        template <typename Archive>
        static inline std::streampos archive_tell(
            Archive & ar)
        {
            typedef BOOST_DEDUCED_TYPENAME boost::mpl::if_<
                BOOST_DEDUCED_TYPENAME Archive::is_saving, 
                save_archive_tell<Archive>, 
                load_archive_tell<Archive>
            >::type typex;
            return typex::invoke(ar);
        }

        template <typename Archive>
        struct save_archive_seek
        {
            static inline Archive & invoke(
                Archive & ar, 
                std::streamoff off, 
                std::ios::seek_dir dir)
            {
                //ar.seekp(off, dir);
                return ar;
            }
        };

        template <typename Archive>
        struct load_archive_seek
        {
            static inline Archive & invoke(
                Archive & ar, 
                typename Archive::off_type off, 
                std::ios_base::seekdir dir)
            {
                ar.seekg(off, dir);
                return ar;
            }
        };

        template <typename Archive>
        static inline Archive & archive_seek(
            Archive & ar, 
            typename Archive::off_type off, 
            std::ios_base::seekdir dir)
        {
            typedef BOOST_DEDUCED_TYPENAME boost::mpl::if_<
                BOOST_DEDUCED_TYPENAME Archive::is_saving, 
                save_archive_seek<Archive>, 
                load_archive_seek<Archive>
            >::type typex;
            return typex::invoke(ar, off, dir);
        }

        struct AsfObjectHeader
        {
            AsfUuid ObjectId;
            boost::uint64_t ObjLength;

            AsfObjectHeader()
            {

            }
            AsfObjectHeader(
                AsfUuid const &ObjectId)
                : ObjectId(ObjectId)
                , ObjLength(0)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & ObjectId
                    & ObjLength;
            }
        };

        struct AsfHeaderObject
            : AsfObjectHeader//page 6 ,3.1
        {
            boost::uint32_t NumOfHeaderObject;
            boost::uint8_t Reserved1;
            boost::uint8_t Reserver2;

            AsfHeaderObject()
                : AsfObjectHeader(ASF_HEADER_OBJECT)
                , NumOfHeaderObject(0)
                , Reserved1(1)
                , Reserver2(2)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                AsfObjectHeader::serialize(ar);
                ar & NumOfHeaderObject
                    & Reserved1
                    & Reserver2;
            }
        };

       

        struct PropertiesFlag
        {
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint32_t Reserved : 30;
                    boost::uint32_t SeekableFlag : 1;
                    boost::uint32_t BroadcastFlag : 1;
#else 
                    boost::uint32_t BroadcastFlag : 1;
                    boost::uint32_t SeekableFlag : 1;
                    boost::uint32_t Reserved : 30;
#endif
                };
                boost::uint32_t flag;
            };

            PropertiesFlag()
                :flag(0)
            {

            }
            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & flag;
            }
        };

        struct AsfFilePropertiesObjectData
        {
            AsfUuid FileId;//调用generate()生成
            boost::uint64_t FileSize;
            boost::uint64_t CreationDate;
            boost::uint64_t DataPacketsCount;
            boost::uint64_t PlayDuration;
            boost::uint64_t SendDuration;
            boost::uint64_t Preroll;
            struct PropertiesFlag Flag;
            boost::uint32_t MinimumDataPacketSize;
            boost::uint32_t MaximumDataPacketSize;
            boost::uint32_t MaximumBitrate;

            AsfFilePropertiesObjectData()
                : FileSize(0)
                , CreationDate(0)
                , DataPacketsCount(0xFFFFFFFF)
                , PlayDuration(0)
                , SendDuration(0)
                , Preroll(0)
                , MinimumDataPacketSize(0)
                , MaximumDataPacketSize(0)
                , MaximumBitrate(0)
            {

            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & FileId
                    & FileSize
                    & CreationDate
                    & DataPacketsCount
                    & PlayDuration
                    & SendDuration
                    & Preroll
                    & Flag
                    & MinimumDataPacketSize
                    & MaximumDataPacketSize
                    & MaximumBitrate;
            }
        };


        struct AsfFilePropertiesObject
            : AsfObjectHeader
            , AsfFilePropertiesObjectData
        {
            AsfFilePropertiesObject()
                :AsfObjectHeader(ASF_FILE_PROPERTIES_OBJECT)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                AsfObjectHeader::serialize(ar);
                AsfFilePropertiesObjectData::serialize(ar);
            }
        };

        struct AsfHeaderExtensionObject
        {
            AsfUuid ObjectId;
            boost::uint64_t ObjectSize;
            AsfUuid Reserved1;
            boost::uint16_t Reserved2;
            boost::uint32_t HeaderExtensionDataSize;
            std::vector<boost::uint8_t> HeaderExtensionData;

            AsfHeaderExtensionObject()
                : ObjectId(ASF_HEADER_EXTENSION_OBJECT)
                , ObjectSize(0)
                , Reserved1(ASF_Reserved_1)
                , Reserved2(6)
                , HeaderExtensionDataSize(0)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & ObjectId
                    & ObjectSize
                    & Reserved1
                    & Reserved2
                    & HeaderExtensionDataSize;
                if (0 !=HeaderExtensionDataSize)
                {
                    util::serialization::serialize_collection(
                        ar
                        , HeaderExtensionData
                        , HeaderExtensionDataSize);
                }
            }
        };

        struct AsfStreamProperFlag
        {
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint16_t EncryptedContentFlag : 1;
                    boost::uint16_t Reserved : 8;
                    boost::uint16_t StreamNumber : 7;
#else 
                    boost::uint16_t StreamNumber : 7;
                    boost::uint16_t Reserved : 8;
                    boost::uint16_t EncryptedContentFlag : 1;
#endif
                };
                boost::uint16_t flag;
            };

            AsfStreamProperFlag()
                :flag(0)
            {

            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & flag;
            }
        };

        struct AsfAudioMediaType//page 80
        {
            boost::uint16_t CodecId;
            boost::uint16_t NumberOfChannels;
            boost::uint32_t SamplesPerSecond;
            boost::uint32_t AverageNumberOfBytesPerSecond;
            boost::uint16_t BlockAlignment;
            boost::uint16_t BitsPerSample;
            boost::uint16_t CodecSpecificDataSize;
            std::vector<boost::uint8_t> CodecSpecificData;

            AsfAudioMediaType()
                : CodecId(0)
                , NumberOfChannels(0)
                , SamplesPerSecond(0)
                , AverageNumberOfBytesPerSecond(0)
                , BlockAlignment(0)
                , BitsPerSample(0)
                , CodecSpecificDataSize(0)
            {

            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & CodecId
                    & NumberOfChannels
                    & SamplesPerSecond
                    & AverageNumberOfBytesPerSecond
                    & BlockAlignment
                    & BitsPerSample
                    & CodecSpecificDataSize;
                util::serialization::serialize_collection(ar, CodecSpecificData, CodecSpecificDataSize);
            }
        };

        struct AsfVideoMediaType//page 82
        {
            boost::uint32_t EncodeImageWidth;
            boost::uint32_t EncodeImageHeight;
            boost::uint8_t ReservedFlags;
            boost::uint16_t FormatDataSize;

            struct FormatData {
                boost::uint32_t FormatDataSize;
                boost::uint32_t ImageWidth;
                boost::uint32_t ImageHeight;
                boost::uint16_t Reserved;
                boost::uint16_t BitsPerPixelCount;
                boost::uint32_t CompressionID;
                boost::uint32_t ImageSize;
                boost::uint32_t HorizontalPixelsPerMeter;
                boost::uint32_t VerticalPixelsPerMeter;
                boost::uint32_t ColorsUsedCount;
                boost::uint32_t ImportantColorsCount;
                std::vector<boost::uint8_t> CodecSpecificData;

                FormatData()
                    : FormatDataSize(0)
                    , ImageWidth(0)
                    , ImageHeight(0)
                    , Reserved(1)
                    , BitsPerPixelCount(0)
                    , CompressionID(0)
                    , ImageSize(0)
                    , HorizontalPixelsPerMeter(0)
                    , VerticalPixelsPerMeter(0)
                    , ColorsUsedCount(0)
                    , ImportantColorsCount(0)
                {

                }

                template <typename Archive>
                void serialize(Archive & ar)
                {
                    ar & FormatDataSize
                        & ImageWidth
                        & ImageHeight
                        & Reserved
                        & BitsPerPixelCount
                        & framework::container::make_array((boost::uint8_t *)&CompressionID, sizeof(CompressionID))
                        & ImageSize
                        & HorizontalPixelsPerMeter
                        & VerticalPixelsPerMeter
                        & ColorsUsedCount
                        & ImportantColorsCount;
                    util::serialization::serialize_collection(ar, CodecSpecificData, FormatDataSize - 40);
                }
            } FormatData;

            AsfVideoMediaType()
                : EncodeImageWidth(0)
                , EncodeImageHeight(0)
                , ReservedFlags(2)
                , FormatDataSize(0)
            {

            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & EncodeImageWidth
                    & EncodeImageHeight
                    & ReservedFlags
                    & FormatDataSize
                    & FormatData;
            }
        };

        struct AsfStreamPropertiesObjectData//page 9
        {
            AsfUuid StreamType;
            AsfUuid ErrorCorrectionType;
            boost::uint64_t TimeOffset;
            boost::uint32_t TypeSpecificDataLength;
            boost::uint32_t ErrorCorrectionDataLength;
            struct AsfStreamProperFlag Flag;
            boost::uint32_t Reserved;
            AsfVideoMediaType Video_Media_Type;
            AsfAudioMediaType Audio_Media_Type;
            std::vector<boost::uint8_t> TypeSpecificData;
            std::vector<boost::uint8_t> Error_Correction_Data;

            AsfStreamPropertiesObjectData()
                : ErrorCorrectionType(ASF_ERROR_CORRECTION_OBJECT)
                , TimeOffset(0)
                , TypeSpecificDataLength(0)
                , ErrorCorrectionDataLength(0)
                , Reserved(0)
            {

            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ASFUUID ASF_Audio_Media = {
                    0xF8699E40,0x5B4D,0x11CF,{0xA8,0xFD,0x00,0x80,0x5F,0x5C,0x44,0x2B}};
                ASFUUID ASF_Video_Media = {
                    0xBC19EFC0,0x5B4D,0x11CF,{0xA8,0xFD,0x00,0x80,0x5F,0x5C,0x44,0x2B}};

                ar & StreamType
                    & ErrorCorrectionType
                    & TimeOffset
                    & TypeSpecificDataLength
                    & ErrorCorrectionDataLength
                    & Flag
                    & Reserved;

                boost::uint32_t offset1 = archive_tell(ar);
                if (ASF_Video_Media == StreamType) {
                    ar & Video_Media_Type;
                } else if (ASF_Audio_Media == StreamType) {
                    ar & Audio_Media_Type;
                } else {
                    util::serialization::serialize_collection( ar
                        , TypeSpecificData
                        , TypeSpecificDataLength);
                }
                boost::uint32_t offset2 = archive_tell(ar);
                (void)offset1;
                (void)offset2;
//               assert(!ar || offset2 == offset1 + TypeSpecificDataLength );
                
                if (ErrorCorrectionDataLength > 0) {
                    util::serialization::serialize_collection(ar, Error_Correction_Data, ErrorCorrectionDataLength);
                }
            }
        };
        
        struct AsfStreamPropertiesObject
            : AsfObjectHeader
            , AsfStreamPropertiesObjectData
        {
            AsfStreamPropertiesObject()
                :AsfObjectHeader(ASF_STREAM_PROPERTIES_OBJECT)
            {
            }
            
            template <typename Archive>
            void serialize(Archive & ar)
            {
                AsfObjectHeader::serialize(ar);
                AsfStreamPropertiesObjectData::serialize(ar);
            }
        };

        struct ErrorCorrectionData//page 46 
        {
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint8_t ErrorCorrectionPresent : 1;
                    boost::uint8_t ErrorCorrectionLengthType : 2;
                    boost::uint8_t OpaqueDataPresent : 1;
                    boost::uint8_t ErrorCorrectionDataLength : 4;
#else 
                    boost::uint8_t ErrorCorrectionDataLength : 4;
                    boost::uint8_t OpaqueDataPresent : 1;
                    boost::uint8_t ErrorCorrectionLengthType : 2;
                    boost::uint8_t ErrorCorrectionPresent : 1;
#endif
                };
                boost::uint8_t flag;
            };


            ErrorCorrectionData()
                :flag(0)
            {
                ErrorCorrectionPresent = 1;
            }

            std::vector<boost::uint8_t> Data;

            SERIALIZATION_SPLIT_MEMBER()

            template <typename Archive>
            void load(Archive & ar)
            {
                ar & flag;
                assert(!ar || ErrorCorrectionPresent == 0 || ErrorCorrectionLengthType == 0);
                if (ar && ErrorCorrectionPresent != 0 && ErrorCorrectionLengthType != 0) {
                    ar.fail();
                }
                if (ErrorCorrectionPresent == 0) {
                    ar.seekg(-1, std::ios::cur);
                } else {
                    util::serialization::serialize_collection(ar, Data, ErrorCorrectionDataLength);
                }
            }

            template <typename Archive>
            void save(Archive & ar) const
            {
                if (!ErrorCorrectionPresent)
                    return;
                ar & flag;
                util::serialization::serialize_collection(ar, Data, ErrorCorrectionDataLength);
            }
        };

        struct AsfPayLoadParsingInformation//page 47
        {
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint8_t ErrorCorrectionPresent : 1;
                    boost::uint8_t PacketLengthType : 2;
                    boost::uint8_t PaddingLengthType : 2;
                    boost::uint8_t SequenceType : 2;
                    boost::uint8_t MultiplePayloadsPresent : 1;
#else 
                    boost::uint8_t MultiplePayloadsPresent : 1;
                    boost::uint8_t SequenceType : 2;
                    boost::uint8_t PaddingLengthType : 2;
                    boost::uint8_t PacketLengthType : 2;
                    boost::uint8_t ErrorCorrectionPresent : 1;
#endif
                };
                boost::uint8_t flag1;
            };

            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint8_t StreamNumLengthType : 2;
                    boost::uint8_t MediaObjNumType : 2;
                    boost::uint8_t OffsetIntoMOLType : 2;
                    boost::uint8_t ReplicatedDataLengthType : 2;
#else 
                    boost::uint8_t ReplicatedDataLengthType : 2;
                    boost::uint8_t OffsetIntoMOLType : 2;
                    boost::uint8_t MediaObjNumType : 2;
                    boost::uint8_t StreamNumLengthType : 2;
#endif
                };
                boost::uint8_t flag2;
            };
            boost::uint32_t PacketLength;
            boost::uint32_t Sequence;
            boost::uint32_t PaddingLength;
            boost::uint32_t SendTime;
            boost::uint16_t Duration;

            AsfPayLoadParsingInformation()
                : flag1(0)
                , flag2(0)
                , PacketLength(0)
                , Sequence(0)
                , PaddingLength(0)
                , SendTime(0)
                , Duration(0)
            {

            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & flag1;
                ar & flag2;
                serialize_length(ar, PacketLength, PacketLengthType);
                serialize_length(ar, Sequence, SequenceType);
                serialize_length(ar, PaddingLength, PaddingLengthType);
                ar  & SendTime & Duration;
            }
        };

        struct AsfDataObject//page 44
        {
            AsfUuid ObjectId;
            boost::uint64_t ObjLength;
            AsfUuid FileId;
            boost::uint64_t TotalDataPackets;
            boost::uint16_t Reserved;
            
            AsfDataObject()
                : ObjectId(ASF_DATA_OBJECT)
                , ObjLength(0)
                , TotalDataPackets(0)
                , Reserved(0x0101)
            {

            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & ObjectId
                    & ObjLength
                    & FileId
                    & TotalDataPackets
                    & Reserved;
            }
        };

        struct AsfPacket;

        struct AsfParseContext
        {
            AsfParseContext()
                : max_packet_size(0)
                , packet(NULL)
                , packet_payload_end(0)
                , payload_data_offset(0)
            {
            }

            boost::uint32_t max_packet_size;
            AsfPacket const * packet;
            boost::uint64_t packet_payload_end;
            boost::uint64_t payload_data_offset;
        };

        struct AsfPacket//page 45
        {
            //boost::uint32_t MaximumDataPacketSize;
            ErrorCorrectionData ErrorCorrectionInfo;
            AsfPayLoadParsingInformation PayLoadParseInfo;
            // Only for Multiple payloads
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint8_t PayloadLengthType : 2;
                    boost::uint8_t PayloadNum : 6;
#else 
                    boost::uint8_t PayloadNum : 6;
                    boost::uint8_t PayloadLengthType : 2;
#endif
                };
                boost::uint8_t flag;
            };


            AsfPacket()
                : flag(0)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                boost::uint32_t start_offset = archive_tell(ar);
                AsfParseContext * ctx = (AsfParseContext *)ar.context();

                ar & ErrorCorrectionInfo
                    & PayLoadParseInfo;

                if (PayLoadParseInfo.PacketLengthType == 0) {
                    PayLoadParseInfo.PacketLength = ctx->max_packet_size;
                } else {
                    assert(!ar || PayLoadParseInfo.PacketLength <= ctx->max_packet_size);
                    if (ar && PayLoadParseInfo.PacketLength > ctx->max_packet_size) {
                        ar.fail();
                    }
                }

                ctx->packet_payload_end = start_offset + PayLoadParseInfo.PacketLength - PayLoadParseInfo.PaddingLength;

                if (PayLoadParseInfo.MultiplePayloadsPresent) {//Multiple Payloads
                    ar & flag;
                } else {//Single Payload
                    PayloadNum = 1;
                    PayloadLengthType = 0;
                }

                boost::uint32_t end_offset = archive_tell(ar);
                assert(!ar || start_offset + PayLoadParseInfo.PacketLength > end_offset + PayLoadParseInfo.PaddingLength);
                if (ar && start_offset + PayLoadParseInfo.PacketLength <= end_offset + PayLoadParseInfo.PaddingLength) {
                    ar.fail();
                }
            }
        };

        struct AsfPayloadHeader
        {
            union {
                struct {
#ifdef   BOOST_BIG_ENDIAN
                    boost::uint8_t KeyFrameBit : 1;
                    boost::uint8_t StreamNum : 7;
#else
                    boost::uint8_t StreamNum : 7;
                    boost::uint8_t KeyFrameBit : 1;
                };
                boost::uint8_t flags;
            };
#endif
            boost::uint32_t MediaObjNum;
            boost::uint32_t OffsetIntoMediaObj;
            boost::uint32_t ReplicatedDataLen;
            boost::uint32_t MediaObjectSize; // In ReplicateData
            boost::uint32_t PresTime; // In ReplicateData
            std::vector<boost::uint8_t> ReplicateData;
            // Only for Multiple payloads
            boost::uint32_t PayloadLength;

            AsfPayloadHeader()
                : flags(0)
                , MediaObjNum(0)
                , OffsetIntoMediaObj(0)
                , ReplicatedDataLen(0)
                , MediaObjectSize(0)
                , PresTime(0)
                , PayloadLength(0)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                AsfParseContext * ctx = (AsfParseContext *)ar.context();

                ar & flags;
                //serialize_length():指定序列化位数
                serialize_length(ar, MediaObjNum, ctx->packet->PayLoadParseInfo.MediaObjNumType);
                serialize_length(ar, OffsetIntoMediaObj, ctx->packet->PayLoadParseInfo.OffsetIntoMOLType);
                serialize_length(ar, ReplicatedDataLen, ctx->packet->PayLoadParseInfo.ReplicatedDataLengthType);
                // 不考虑压缩的情形
                assert(!ar || ReplicatedDataLen >= 8);
                boost::uint32_t start_offset = archive_tell(ar);
                if (ar && (ReplicatedDataLen < 8 || start_offset + ReplicatedDataLen >= ctx->packet_payload_end)) {
                    ar.fail();
                }
                ar & MediaObjectSize;
                ar & PresTime;
                // already read 8 bytes
                if (ReplicatedDataLen > 8)
                    //serialize_collection():集合类型的序列化，同时指定集合长度
                    util::serialization::serialize_collection(ar, ReplicateData, ReplicatedDataLen - 8);
                if (ctx->packet->PayLoadParseInfo.MultiplePayloadsPresent) {
                    serialize_length(ar, PayloadLength, ctx->packet->PayloadLengthType);
                    ctx->payload_data_offset = archive_tell(ar);
                    if (ar && PayloadLength + ctx->payload_data_offset > ctx->packet_payload_end) {
                        ar.fail();
                    }
                } else {
                    ctx->payload_data_offset = archive_tell(ar);
                    PayloadLength = (boost::uint32_t)(ctx->packet_payload_end - ctx->payload_data_offset);
                }
            }
        };

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_ASF_ASF_OBJECT_TYPE_H_
 
