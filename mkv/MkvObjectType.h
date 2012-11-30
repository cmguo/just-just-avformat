//MkvObjectType.h

#ifndef _PPBOX_AVFORMAT_MKV_MKV_OBJECT_TYPE_H_
#define _PPBOX_AVFORMAT_MKV_MKV_OBJECT_TYPE_H_

#include "ppbox/avformat/mkv/EBMLType.h"
#include "ppbox/avformat/stream/FormatBuffer.h"

namespace ppbox
{
    namespace avformat
    {

#define VIDEO_TRACKE_TYPE_VALUE 0x01;
#define AUDIO_TRACKE_TYPE_VALUE 0x02;
#define SUBTITLE_TRACKE_TYPE_VALUE 0x11;

        typedef EBMLOArchive MKVOArchive;

        typedef EBMLIArchive MKVIArchive;

        //EBML
        typedef Element_Header<0xA45DFA3> EBML_Header;

        struct EBML_Data
        {
            EBML_UInt EBMLVersion;
            EBML_UInt EBMLReadVersion;
            EBML_UInt EBMLMaxIDLength;
            EBML_UInt EBMLMaxSizeLength;
            EBML_String DocType;
            EBML_UInt DocTypeVersion;
            EBML_UInt DocTypeReadVersion;

            EBML_Data()
                : EBMLVersion(0x286, 1)
                , EBMLReadVersion(0x2F7, 1)
                , EBMLMaxIDLength(0x2F2, 4)
                , EBMLMaxSizeLength(0x2F3, 8)
                , DocType(0x282, "matroska")
                , DocTypeVersion(0x287, 2)
                , DocTypeReadVersion(0x285, 2)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & EBMLVersion
                    & EBMLReadVersion
                    & EBMLMaxIDLength
                    & EBMLMaxSizeLength
                    & DocType
                    & DocTypeVersion
                    & DocTypeReadVersion;
            }

        };

        //EBML_Header具有自己的头部
        struct EBML_Object
            : EBML_Header
            , EBML_Data
        {
            template <typename Archive>
            void serialize(Archive ar)
            {
                EBML_Header::serialize(ar);
                EBML_Data::serialize(ar);
            }

            size_t data_size()
            {
                return
                    EBMLVersion.byte_size() +
                    EBMLReadVersion.byte_size() +
                    EBMLMaxIDLength.byte_size() +
                    EBMLMaxSizeLength.byte_size() +
                    DocType.byte_size() +
                    DocTypeVersion.byte_size() +
                    DocTypeReadVersion.byte_size();
            }
        };


        //Segments

        typedef Element_Header<0x8538067> MKV_Segment_Header;

        //Segment_Info:child of Segments
        typedef Element_Header<0x549A966> MKV_Segment_Info_Header;

        struct MKV_Segment_Info_Data
        {
            EBML_Binary Segment_UID;
            EBML_String Segment_File_Name;
            EBML_Binary Prev_UID;
            EBML_String Prev_File_Name;
            EBML_Binary Next_UID;
            EBML_String Next_File_Name;
            EBML_UInt Time_Code_Scale;
            EBML_Float Duration;
            EBML_String Title;
            EBML_String Muxing_App;
            EBML_String Writing_App;
            EBML_UInt Date_UTC;

            MKV_Segment_Info_Data()
                : Segment_UID(0x33A4)
                , Segment_File_Name(0x3384)
                , Prev_UID(0x1CB923)
                , Prev_File_Name(0x1C83AB)
                , Next_UID(0x1EB923)
                , Next_File_Name(0x1E83BB)
                , Time_Code_Scale(0xAD7B1)
                , Duration(0x489)
                , Title(0x3BA9)
                , Muxing_App(0xD80)
                , Writing_App(0x1741)
                , Date_UTC(0x461)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & Segment_UID
                    & Segment_File_Name
                    & Prev_UID
                    & Prev_File_Name
                    & Next_UID
                    & Next_File_Name
                    & Time_Code_Scale
                    & Duration
                    & Title
                    & Muxing_App
                    & Writing_App
                    & Date_UTC;
            }

        };

        struct MKV_Segment_Info
            : MKV_Segment_Info_Header
            , MKV_Segment_Info_Data
        {
            template <typename Archive>
            void serialize(Archive & ar)
            {
                MKV_Segment_Info_Header::serialize(ar);
                MKV_Segment_Info_Data::serialize(ar);
            }

            size_t data_size()
            {
                return
                    Segment_UID.byte_size() + 
                    Segment_File_Name.byte_size() +
                    Prev_UID.byte_size() + 
                    Prev_File_Name.byte_size() +
                    Next_UID.byte_size() +
                    Next_File_Name.byte_size() +
                    Time_Code_Scale.byte_size() +
                    Duration.byte_size() +
                    Title.byte_size() +
                    Muxing_App.byte_size() +
                    Writing_App.byte_size() +
                    Date_UTC.byte_size();
            }
        };

        //Tracks
        typedef Element_Header<0x654AE6B> MKV_Tracks_Header;

        //MKV_Track_Entry:child of Tracks
        typedef Element_Header<0x2E> MKV_Track_Entry_Header;

        //Video:child of MKV_Track_Entry
        typedef Element_Header<0x60> MKV_Track_Video_Header;

        struct MKV_Track_Video_Data
        {
            EBML_UInt PixelWidth;
            EBML_UInt PixelHeight;
            EBML_UInt PixelCropBottom;
            EBML_UInt PixelCropTop;
            EBML_UInt PixelCropLeft;
            EBML_UInt PixelCropRight;
            EBML_UInt DisplayWidth;
            EBML_UInt DisplayHeight;
            EBML_UInt DisplayUnit;

            MKV_Track_Video_Data()
                : PixelWidth(0x30)
                , PixelHeight(0x3A)
                , PixelCropBottom(0x14AA)
                , PixelCropTop(0x14BB)
                , PixelCropLeft(0x14CC)
                , PixelCropRight(0x14DD)
                , DisplayWidth(0x14B0)
                , DisplayHeight(0x14BA)
                , DisplayUnit(0x14B2)
            {
            }

            template <typename Archive>
            void serialize(Archive ar)
            {
                ar & PixelWidth
                    & PixelHeight
                    & PixelCropBottom
                    & PixelCropTop
                    & PixelCropLeft
                    & PixelCropRight
                    & DisplayWidth
                    & DisplayHeight
                    & DisplayUnit;
            }
        };

        struct MKV_Track_Video
            : MKV_Track_Video_Header
            , MKV_Track_Video_Data
        {
            template <typename Archive>
            void serialize(Archive ar)
            {
                MKV_Track_Video_Header::serialize(ar);
                MKV_Track_Video_Data::serialize(ar);
            }

            size_t data_size() const
            {
                return 
                    PixelWidth.byte_size() + 
                    PixelHeight.byte_size() +
                    PixelCropBottom.byte_size() +
                    PixelCropTop.byte_size() +
                    PixelCropLeft.byte_size() +
                    PixelCropRight.byte_size() +
                    DisplayWidth.byte_size() +
                    DisplayHeight.byte_size() +
                    DisplayUnit.byte_size();
            }
        };

        //Audio:child of MKV_Track_Entry
        typedef Element_Header<0x61> MKV_Track_Audio_Header;

        struct MKV_Track_Audio_Data
        {
            EBML_Float SamplingFrequency;
            EBML_UInt OutputSamplingFrequency;
            EBML_UInt Channels;
            EBML_UInt BitDepth;

            MKV_Track_Audio_Data()
                : SamplingFrequency(0x35)
                , OutputSamplingFrequency(0x38B5)
                , Channels(0x1F)
                , BitDepth(0x2264)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & SamplingFrequency
                    & OutputSamplingFrequency
                    & Channels
                    & BitDepth;
            }

        };

        struct MKV_Track_Audio
            : MKV_Track_Audio_Header
            , MKV_Track_Audio_Data
        {
            template <typename Archive>
            void serialize(Archive ar)
            {
                MKV_Track_Audio_Header::serialize(ar);
                MKV_Track_Audio_Data::serialize(ar);
            }

            size_t data_size() const
            {
                return
                    SamplingFrequency.byte_size() +
                    OutputSamplingFrequency.byte_size() +
                    Channels.byte_size() +
                    BitDepth.byte_size();
            }
        };

        //MKV_Content_Encodings:child of MKV_Track_Entry
        typedef Element_Header<0x2D80> MKV_Content_Encodings_Header;

        //MKV_Content_Encoding:child of MKV_Content_Encodings
        typedef Element_Header<0x2240> MKV_Content_Encoding_Header;

        //MKV_Content_Compression:child of MKV_Content_Encoding
        struct MKV_Content_Compression
            : EBML_Element_Header
        {
            EBML_UInt ContentCompAlgo;
            EBML_Binary ContentCompSettings;

            MKV_Content_Compression()
                : EBML_Element_Header(0x1034)
                , ContentCompAlgo(0x254)
                , ContentCompSettings(0x255)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                EBML_Element_Header::serialize(ar);
                ar & ContentCompAlgo
                    & ContentCompSettings;
            }
        };

        //MKV_Content_Encryption::child of MKV_Content_Encoding
        struct MKV_Content_Encryption
            : EBML_Element_Header
        {
            MKV_Content_Encryption()
                : EBML_Element_Header(0x1035)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                EBML_Element_Header::serialize(ar);
            }
        };

        struct MKV_Content_Encoding_Data
        {
            EBML_UInt ContentEncodingOrder;
            EBML_UInt ContentEncodingScope;
            EBML_UInt ContentEncodingType;

            MKV_Content_Compression ContentCompression;
            MKV_Content_Encryption ContentEncryption;

            MKV_Content_Encoding_Data()
                : ContentEncodingOrder(0x1031)
                , ContentEncodingScope(0x1032)
                , ContentEncodingType(0x1033)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & ContentEncodingOrder
                    & ContentEncodingScope
                    & ContentEncodingType
                    & ContentCompression
                    & ContentEncryption;
            }
        };

        struct MKV_Content_Encoding
            : MKV_Content_Encoding_Header
            , MKV_Content_Encoding_Data
        {
            template <typename Archive>
            void serialize(Archive & ar)
            {
                if(MKV_Content_Encoding_Header::serialize(ar))
                    return;
                MKV_Content_Encoding_Data::serialize(ar);
            }
        };

        struct MKV_Track_Entry_Data
        {
            EBML_UInt TrackNumber;
            EBML_Binary TrackUID;
            EBML_UInt TrackType;
            EBML_Bool FlagEnable;
            EBML_Bool FlagDefault;
            EBML_Bool FlagForced;
            EBML_Bool FlagLacing;
            EBML_UInt MinCache;
            EBML_UInt MaxCache;
            EBML_UInt DefaultDuration;
            EBML_Float TrackTimeCodeScale;
            EBML_String Name;
            EBML_String Language;
            EBML_String CodecID; 
            EBML_Binary CodecPrivate;
            EBML_String CodecName;
            EBML_UInt AttachmentLink;

            MKV_Track_Video Video;
            MKV_Track_Audio Audio;
            MKV_Content_Encoding ContentEncoding;

            MKV_Track_Entry_Data()
                : TrackNumber(0x57)
                , TrackUID(0x33C5)
                , TrackType(0x3)
                , FlagEnable(0x39)
                , FlagDefault(0x8)
                , FlagForced(0x15AA)
                , FlagLacing(0x1C)
                , MinCache(0x2DE7)
                , MaxCache(0x2DF8)
                , DefaultDuration(0x3E383)
                , TrackTimeCodeScale(0x3314F)
                , Name(0x136E)
                , Language(0x2B59C)
                , CodecID(0x6)
                , CodecPrivate(0x23A2)
                , CodecName(0x58688)
                , AttachmentLink(0x3446)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & TrackNumber
                    & TrackUID
                    & TrackType
                    & FlagEnable
                    & FlagDefault
                    & FlagForced
                    & FlagLacing
                    & MinCache
                    & MaxCache
                    & DefaultDuration
                    & TrackTimeCodeScale
                    & Name
                    & Language
                    & CodecID
                    & CodecPrivate
                    & CodecName
                    & AttachmentLink
                    & Video
                    & Audio
                    & ContentEncoding;
            }
        };

        struct MKV_Track_Entry
            : MKV_Track_Entry_Header
            , MKV_Track_Entry_Data
        {
            template <typename Archive>
            void serialize(Archive ar)
            {
                MKV_Track_Entry_Header::serialize(ar);
                MKV_Track_Entry_Data::serialize(ar);
            }

            size_t data_size()
            {
                return
                    TrackNumber.byte_size() +
                    TrackUID.byte_size() +
                    TrackType.byte_size() +
                    FlagEnable.byte_size() +
                    FlagDefault.byte_size() +
                    FlagForced.byte_size() +
                    FlagLacing.byte_size() +
                    MinCache.byte_size() +
                    MaxCache.byte_size() +
                    DefaultDuration.byte_size() +
                    TrackTimeCodeScale.byte_size() +
                    Name.byte_size() +
                    Language.byte_size() +
                    CodecID.byte_size() +
                    CodecPrivate.byte_size() +
                    CodecName.byte_size() +
                    AttachmentLink.byte_size() +
                    Video.byte_size() +
                    Audio.byte_size();
            }
        };

        //MKV_Cluster:child of Segment
        typedef Element_Header<0xF43B675> MKV_Cluster_Header;

        //MKV_Block:element of MKV_Block_Group
        typedef Element_Header<0x20> MKV_Block_Group_Header;

        typedef Element_Header<0x21> MKV_Block_Header;

        struct MKV_Block_Data
        {
            framework::system::VariableNumber<boost::uint32_t> TrackNumber;
            boost::int16_t TimeCode;

            union {
                struct {
#ifdef   BOOST_LITTLE_ENDIAN
                    boost::uint8_t Discardable : 1;
                    boost::uint8_t LaceType : 2;
                    boost::uint8_t Invisible : 1;
                    boost::uint8_t Reserved : 3;
                    boost::uint8_t Keyframe : 1;
#else 
                    boost::uint8_t Keyframe : 1;
                    boost::uint8_t Reserved : 3;
                    boost::uint8_t Invisible : 1;
                    boost::uint8_t LaceType : 2;
                    boost::uint8_t Discardable : 1;
#endif
                };
                boost::uint8_t flag;
            };

            MKV_Block_Data()
                : TimeCode(0)
                , flag(0)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & TrackNumber
                    & TimeCode
                    & flag;
            }

            size_t data_size() const
            {
                return TrackNumber.size() + 3;
            }
        };

        struct MKV_Block
            : MKV_Block_Header
            , MKV_Block_Data
        {
            template <typename Archive>
            void serialize(Archive & ar)
            {
                MKV_Block_Header::serialize(ar);
                MKV_Block_Data::serialize(ar);
            }
        };

        //MKV_Block_Group:child of MKV_Cluster
        struct MKV_Block_Group
            : MKV_Block_Group_Header
        {
            EBML_Int ReferenceBlock;
            EBML_Int BlockDuration;
            //EBML_Binary Block;
            MKV_Block MkvBlock;

            MKV_Block_Group()
                : ReferenceBlock(0x7B)
                , BlockDuration(0x1B)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                if(EBML_Element_Header::serialize(ar)) {
                    return;
                }
                ar & ReferenceBlock
                    & BlockDuration
                    & MkvBlock;
            }
        };

        //MKV_Simple_Block
        typedef Element_Header<0x23> MKV_Simple_Block_Header;

        struct MKV_Simple_Block_Data
            : MKV_Block_Data
        {
            template <typename Archive>
            void serialize(Archive & ar)
            {
                MKV_Block_Data::serialize(ar);
            }
        };

        struct MKV_Simple_Block
            : MKV_Simple_Block_Header
            , MKV_Simple_Block_Data
        {
            template <typename Archive>
            void serialize(Archive & ar)
            {
                if(MKV_Simple_Block_Header::serialize(ar)) {
                    return;
                }
                MKV_Simple_Block_Data::serialize(ar);
            }

            size_t data_size() const
            {
                return TrackNumber.size() + 3;
            }
        };

        //MKV_Cluster_Data
        struct MKV_Cluster_Data
        {
            EBML_UInt TimeCode;
            EBML_UInt Position;
            EBML_UInt PrevSize;
            MKV_Block_Group BlockGroup;

            MKV_Cluster_Data()
                : TimeCode(0x67)
                , Position(0x27)
                , PrevSize(0x2B)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & TimeCode
                    & Position
                    & PrevSize
                    & BlockGroup;
            }
        };

        struct MKV_Cluster
            : MKV_Cluster_Header
            , MKV_Cluster_Data
        {
            template <typename Archive>
            void serialize(Archive & ar)
            {
                MKV_Cluster_Header::serialize(ar);
                MKV_Cluster_Data::serialize(ar);
            }
        };

    } // namespace avformat
} // namespace ppbox

#endif

