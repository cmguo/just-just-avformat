// AviMerge.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/avi/AviMerge.h"
#include "ppbox/avformat/avi/lib/AviError.h"

#include <framework/logger/Logger.h>
#include <framework/logger/StreamRecord.h>

FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("ppbox.avformat.AviMerge", framework::logger::Debug);

namespace ppbox
{
    namespace avformat
    {

        bool avi_merge_head(
            std::basic_istream<boost::uint8_t> & is,
            std::basic_ostream<boost::uint8_t> & os, 
            std::vector<ppbox::data::SegmentInfo> & segment_infos,
            boost::system::error_code & ec)
        {
            if (segment_infos.empty()) {
                ec = framework::system::logic_error::invalid_argument;
                return false;
            }

            if (segment_infos.size() == 1) {
                ec.clear();
                return true;
            }

            AviFile * file = NULL;
            boost::uint64_t offset = 0;
            for (boost::uint32_t i = 0; i < segment_infos.size(); ++i) {
                is.seekg(offset, std::ios::beg);
                LOG_DEBUG("[avi_merge_head] segment: " << i << ", head_size: " << segment_infos[i].head_size);
                if (!avi_merge(file, is, segment_infos[i], ec))
                    break;
                offset += segment_infos[i].head_size;
            }

            if (!ec) {
                LOG_DEBUG("[avi_merge_head] write");
                avi_write(file, os, ec);
            }

            return !ec;
        }

        bool avi_merge(
            AviFile *& file, 
            std::basic_istream<boost::uint8_t> & is, 
            ppbox::data::SegmentInfo const & segment, 
            boost::system::error_code & ec)
        {
            AviBoxIArchive ia(*is.rdbuf());

            std::auto_ptr<AviFile> file2(new AviFile);
            if (!file2->load(ia, ec))
                return false;

            AviBox * box = file2->boxes().back();
            if (box->type != AviBoxType::mdat 
                || file2->head_size() != segment.head_size
                || box->head_size() + segment.size != box->byte_size() + segment.head_size) {
                    ec = avi_error::invalid_avi_head;
                    return false;
            }

            if (file == NULL) {
                file = file2.release();
                return file->merge_begin(ec);
            } else {
                return file->merge(*file2, ec);
            }
        }

        bool avi_write(
            AviFile *& file, 
            std::basic_ostream<boost::uint8_t> & os,
            boost::system::error_code & ec)
        {
            LOG_DEBUG("[avi_write] merge_end");
            if (!file->merge_end(ec))
                return false;

            LOG_DEBUG("[avi_write] write");
            AviBoxOArchive oa(*os.rdbuf());
            file->save(oa);

            LOG_DEBUG("[avi_write] finish");
            delete file;
            file = NULL;

            ec.clear();
            return true;
        }

    } // namespace avformat
} // namespace ppbox
