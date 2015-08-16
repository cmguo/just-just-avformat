// Mp4Merge.cpp

#include "just/avformat/Common.h"
#include "just/avformat/mp4/Mp4Merge.h"
#include "just/avformat/mp4/lib/Mp4Error.h"

#include <util/archive/Ctype.h>

#include <framework/logger/Logger.h>
#include <framework/logger/StreamRecord.h>

FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("just.avformat.Mp4Merge", framework::logger::Debug);

namespace just
{
    namespace avformat
    {

        bool mp4_merge_head(
            std::basic_istream<boost::uint8_t> & is,
            std::basic_ostream<boost::uint8_t> & os, 
            std::vector<just::data::SegmentInfo> & segment_infos,
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

            Mp4File * file = NULL;
            boost::uint64_t offset = 0;
            for (boost::uint32_t i = 0; i < segment_infos.size(); ++i) {
                is.seekg(offset, std::ios::beg);
                LOG_DEBUG("[mp4_merge_head] segment: " << i << ", head_size: " << segment_infos[i].head_size);
                if (!mp4_merge(file, is, segment_infos[i], ec))
                    break;
                offset += segment_infos[i].head_size;
            }

            if (!ec) {
                LOG_DEBUG("[mp4_merge_head] write");
                mp4_write(file, os, ec);
            }

            return !ec;
        }

        bool mp4_merge(
            Mp4File *& file, 
            std::basic_istream<boost::uint8_t> & is, 
            just::data::SegmentInfo const & segment, 
            boost::system::error_code & ec)
        {
            Mp4BoxIArchive ia(*is.rdbuf());

            std::auto_ptr<Mp4File> file2(new Mp4File);
            if (!file2->load(ia, ec))
                return false;

            Mp4Box * box = file2->boxes().back();
            if (box->type != Mp4BoxType::mdat 
                || file2->head_size() != segment.head_size
                || box->head_size() + segment.size != box->byte_size() + segment.head_size) {
                    ec = mp4_error::invalid_mp4_head;
                    return false;
            }

            if (file == NULL) {
                file = file2.release();
                return file->merge_begin(ec);
            } else {
                return file->merge(*file2, ec);
            }
        }

        bool mp4_write(
            Mp4File *& file, 
            std::basic_ostream<boost::uint8_t> & os,
            boost::system::error_code & ec)
        {
            LOG_DEBUG("[mp4_write] merge_end");
            if (!file->merge_end(ec))
                return false;

            LOG_DEBUG("[mp4_write] write");
            Mp4BoxOArchive oa(*os.rdbuf());
            file->save(oa);

            LOG_DEBUG("[mp4_write] finish");
            delete file;
            file = NULL;

            ec.clear();
            return true;
        }

    } // namespace avformat
} // namespace just
