// EBML_File.h

#ifndef _JUST_AVFORMAT_MKV_EBML_EBML_FILE_H_
#define _JUST_AVFORMAT_MKV_EBML_EBML_FILE_H_

#include "just/avformat/mkv/ebml/EBML_Element.h"

namespace just
{
    namespace avformat
    {

        struct EBML_FileData
        {
            EBML_UInt EBMLVersion;
            EBML_UInt EBMLReadVersion;
            EBML_UInt EBMLMaxIDLength;
            EBML_UInt EBMLMaxSizeLength;
            EBML_String DocType;
            EBML_UInt DocTypeVersion;
            EBML_UInt DocTypeReadVersion;

            EBML_FileData()
                : EBMLVersion(0x286, 1)
                , EBMLReadVersion(0x2F7, 1)
                , EBMLMaxIDLength(0x2F2, 4)
                , EBMLMaxSizeLength(0x2F3, 4)
                , DocType(0x282)
                , DocTypeVersion(0x287)
                , DocTypeReadVersion(0x285)
            {
            }

            template <typename Archive>
            void serialize(
                Archive & ar)
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

        typedef EBML_Element<0xA45DFA3, EBML_FileData> EBML_File;

    } // namespace avformat
} // namespace just

#endif // _JUST_AVFORMAT_MKV_EBML_EBML_FILE_H_
