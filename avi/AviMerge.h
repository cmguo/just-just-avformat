// AVIMerge.h

#ifndef _PPBOX_MERGE_AVI_AVI_MERGE_H_
#define _PPBOX_MERGE_AVI_AVI_MERGE_H_

#include "ppbox/avformat/avi/lib/AviFile.h"

#include <ppbox/data/segment/SegmentInfo.h>

#include <iostream>

namespace ppbox
{
    namespace avformat
    {

        bool avi_merge_head(
                std::basic_istream<boost::uint8_t> & is,
                std::basic_ostream<boost::uint8_t> & os, 
                std::vector<ppbox::data::SegmentInfo> & segment_infos, 
                boost::system::error_code & ec);

        bool vod_valid_segment_info(
            std::basic_istream<boost::uint8_t> & is,
            std::vector<ppbox::data::SegmentInfo> & segment_infos,
            boost::system::error_code & ec);

        bool avi_merge(
            AviFile *& file, 
            std::basic_istream<boost::uint8_t> & is,
            ppbox::data::SegmentInfo const & segment, // 用于检查正确性
            boost::system::error_code & ec);

        bool avi_write(
            AviFile *& file, 
            std::basic_ostream<boost::uint8_t> & os,
            boost::system::error_code & ec);

    } // namespace merge
} // namespace ppbox

#endif // _PPBOX_MERGE_AVI_AVI_MERGE_H_
