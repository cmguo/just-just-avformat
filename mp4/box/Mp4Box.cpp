// Mp4Box.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/mp4/box/Mp4Box.h"
#include "ppbox/avformat/mp4/box/Mp4BoxContext.h"
#include "ppbox/avformat/mp4/box/Mp4Archive.h"
#include "ppbox/avformat/mp4/box/Mp4BoxEnum.h"

#include <ppbox/avbase/FourCC.h>

#include <framework/logger/StreamRecord.h>
#include <framework/logger/Logger.h>

namespace ppbox
{
    namespace avformat
    {

        FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("ppbox.avformat.Mp4Box", framework::logger::Debug);

        std::map<Mp4Box::id_type, Mp4BoxDefine const *> & Mp4Box::msg_defs()
        {
            static std::map<typename Mp4Box::id_type, Mp4BoxDefine const *> smap;
            return smap;
        }

        Mp4Box::Mp4Box()
        {
        }

        Mp4Box::Mp4Box(
            Mp4Box const & r)
            : Mp4BoxBase(r)
            , Mp4BoxHeader(r)
        {
            // default copy constructor will copy member "data_", which is not expected to 
        }

        Mp4Box::Mp4Box(
            id_type t)
        {
            reset(t);
        }

        void Mp4Box::reset(
            id_type t)
        {
            Mp4BoxBase::reset(find_msg(t));
        }

        void Mp4Box::reset()
        {
            (Mp4BoxHeader &)(*this) = Mp4BoxHeader();
            Mp4BoxBase::reset();
        }

        void Mp4Box::serialize(
            Mp4IArchive & ia)
        {
            Mp4BoxContext * ctx = (Mp4BoxContext *)ia.context();
            ia >> (Mp4BoxHeader &)(*this);
            if (!ia)
                return;
            std::streampos data_beg = ia.tellg();
            std::streampos data_end = 
                data_beg + (std::streamoff)Mp4BoxHeader::data_size();
            if (type != Mp4BoxType::mdat) {
                ia.seekg(data_end, std::ios::beg);
                if (!ia)
                    return;
                ia.seekg(data_beg, std::ios::beg);
            }
            ctx->path += "/"; ctx->path += ppbox::avbase::FourCC::to_string(Mp4BoxHeader::type);
            ctx->stack.push_back(this);
            ctx->data_ends.push_back(data_end);
            Mp4BoxDefine const * def = find_msg(Mp4BoxHeader::type);
            if (def) {
                Mp4BoxBase::reset(def);
                Mp4BoxBase::from_data(&ia);
                assert(!ia || ia.tellg() == data_end);
            } else {
                if (data_size() <= MAX_SIZE) {
                    ia >> make_array(data_, MAX_SIZE);
                }
                LOG_DEBUG("[load] unknown box type: " << ctx->path);
            }
            ctx->data_ends.pop_back();
            ctx->stack.pop_back();
            ctx->path.erase(ctx->path.size() - 5);
            if (type != Mp4BoxType::mdat)
                ia.seekg(data_end, std::ios::beg);
        }

        void Mp4Box::serialize(
            Mp4OArchive & oa) const
        {
            std::streampos beg = oa.tellp();
            oa << (Mp4BoxHeader const &)(*this);
            std::streampos data_beg = oa.tellp();
            Mp4BoxBase::to_data(&oa);
            std::streampos data_end = oa.tellp();
            const_cast<Mp4Box &>(*this).data_size(data_end - data_beg);
            oa.seekp(beg, std::ios::beg);
            oa << (Mp4BoxHeader const &)(*this);
            oa.seekp(data_end, std::ios::beg);
        }

    } // namespace avformat
} // namespace ppbox
