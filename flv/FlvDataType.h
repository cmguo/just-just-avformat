// FlvDataType.h

#ifndef _PPBOX_AVFORMAT_FLV_FLV_DATA_TYPE_H_
#define _PPBOX_AVFORMAT_FLV_FLV_DATA_TYPE_H_

#include "ppbox/avformat/flv/FlvArchive.h"

#include <util/protocol/rtmp/RmtpAmfType.h>

namespace ppbox
{
    namespace avformat
    {

        typedef util::protocol::RtmpAmfType FlvDataType;

        typedef util::protocol::RtmpAmfString FlvDataString;
        typedef util::protocol::RtmpAmfDate FlvDataDate;
        typedef util::protocol::RtmpAmfLongString FlvDataLongString;
        typedef util::protocol::RtmpAmfObjectProperty FlvDataObjectProperty;
        typedef util::protocol::RtmpAmfObject FlvDataObject;
        typedef util::protocol::RtmpAmfECMAArray FlvDataECMAArray;
        typedef util::protocol::RtmpAmfStrictArray FlvDataStrictArray;

        typedef util::protocol::RtmpAmfValue FlvDataValue;

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_FLV_FLV_DATA_TYPE_H_
