// MkvContentEncoding.h

#ifndef _JUST_AVFORMAT_MKV_MKV_CONTENT_ENCODING_H_
#define _JUST_AVFORMAT_MKV_MKV_CONTENT_ENCODING_H_

#include "just/avformat/mkv/ebml/EBML_Vector.h"

namespace just
{
    namespace avformat
    {

        // ContentCompression: child of ContentEncoding

        struct MkvContentCompressionData
        {
            EBML_UInt ContentCompAlgo;
            EBML_Binary ContentCompSettings;

            MkvContentCompressionData()
                : ContentCompAlgo(0x254)
                , ContentCompSettings(0x255)
            {
            }

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & ContentCompAlgo
                    & ContentCompSettings;
            }
        };

        typedef EBML_Element<0x1034, MkvContentCompressionData> MkvContentCompression;

        // ContentEncryption: child of ContentEncoding

        // typedef EBML_Element<0x1035, MkvContentEncryptionData> MkvContentEncryption;

        // ContentEncoding: child of ContentEncodings

        struct MkvContentEncodingData
        {
            EBML_UInt ContentEncodingOrder;
            EBML_UInt ContentEncodingScope;
            EBML_UInt ContentEncodingType;

            MkvContentCompression ContentCompression;
            //MkvContentEncryption ContentEncryption;

            MkvContentEncodingData()
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
                    & ContentCompression;
                    //& ContentEncryption;
            }
        };

        typedef EBML_Element<0x2240, MkvContentEncodingData> MkvContentEncoding;

        // ContentEncodings: child of TrackEntry

        struct MkvContentEncodingsData
        {
            EBML_Vector<MkvContentEncoding> ContentEncodings;

            template <typename Archive>
            void serialize(Archive & ar)
            {
                ar & ContentEncodings;
            }
        };

        typedef EBML_Element<0x2D80, MkvContentEncodingsData> MkvContentEncodings;

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MKV_MKV_CONTENT_ENCODING_H_
