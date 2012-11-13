// Ap4Merge.h

#ifndef _PPBOX_AVFORMAT_MP4_AP4_MERGE_H_
#define _PPBOX_AVFORMAT_MP4_AP4_MERGE_H_

#include <ppbox/data/SegmentInfo.h>

class AP4_File;
class AP4_ByteStream;

namespace ppbox
{
    namespace avformat
    {

        bool ap4_merge(
            AP4_File *& file, 
            AP4_ByteStream & stream, 
            ppbox::data::SegmentInfo const & segment, // ���ڼ����ȷ��
            boost::system::error_code & ec);

        bool ap4_write(
            AP4_File *& file, 
            AP4_ByteStream & stream, 
            boost::system::error_code & ec);

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_AP4_MERGE_H_
