// FlvMetadata.cpp

#include "ppbox/avformat/Common.h"
#include "ppbox/avformat/flv/FlvMetaData.h"

namespace ppbox
{
    namespace avformat
    {

        FlvMetaData::FlvMetaData()
            : hasaudio(false)
            , hasvideo(false)
            , filesize(0)
            , datarate(0)
            , datasize(0)
            , duration(0)
            , lasttimestamp(0)
            // video
            , videosize(0)
            , videodatarate(0)
            , width(0)
            , height(0)
            , framerate(0)
            , videocodecid(0)
            , canSeekToEnd(false)
            , lastkeyframetimestamp(0)
            // audio
            , audiosize(0)
            , audiodatarate(0)
            , audiosamplerate(0)
            , audiosamplesize(0)
            , stereo(false)
            , audiocodecid(0)
        {
        }

        void FlvMetaData::from_data(
            FlvDataValue const & data)
        {
            std::vector<FlvDataObjectProperty> const & variables = 
                (data.Type == FlvDataType::MIXEDARRAY) ? data.as<FlvDataECMAArray>().Variables : data.as<FlvDataObject>().ObjectProperties;
            for (size_t i = 0; i < variables.size(); ++i) {
                FlvDataObjectProperty const & property = variables[i];
                if (property.PropertyName.StringData == "datarate") {
                    datarate = (boost::uint32_t)property.PropertyData.Double;
                }
                if (property.PropertyName.StringData == "duration") {
                    duration = (boost::uint32_t)(property.PropertyData.Double * 1000);
                }
                if (property.PropertyName.StringData == "videocodecid") {
                    videocodecid = (boost::uint32_t)property.PropertyData.Double;
                }
                if (property.PropertyName.StringData == "width") {
                    width = (boost::uint32_t)property.PropertyData.Double;
                }
                if (property.PropertyName.StringData == "height") {
                    height = (boost::uint32_t)property.PropertyData.Double;
                }
                if (property.PropertyName.StringData == "framerate") {
                    framerate = (boost::uint32_t)property.PropertyData.Double;
                }
                if (property.PropertyName.StringData == "audiocodecid") {
                    audiocodecid = (boost::uint32_t)property.PropertyData.Double;
                }
                if (property.PropertyName.StringData == "audiosamplerate") {
                    audiosamplerate = (boost::uint32_t)property.PropertyData.Double;
                }
                if (property.PropertyName.StringData == "audiosamplesize") {
                    audiosamplesize = (boost::uint32_t)property.PropertyData.Double;
                }
                if (property.PropertyName.StringData == "keyframes") {
                    FlvDataObject const & object = property.PropertyData.as<FlvDataObject>();
                    FlvDataStrictArray const & filepositions = object.ObjectProperties[0].PropertyData.as<FlvDataStrictArray>();
                    FlvDataStrictArray const & times = object.ObjectProperties[1].PropertyData.as<FlvDataStrictArray>();
                    keyframes.resize(filepositions.StrictArrayLength);
                    for (size_t i = 0; i < keyframes.size(); ++i) {
                        keyframes[i].offset = (boost::uint64_t)filepositions.StrictArrayValue[i].Double;
                        keyframes[i].time = (boost::uint64_t)(times.StrictArrayValue[i].Double * 1000.0);
                    }
                }
            }
        }

        void FlvMetaData::to_data(
            FlvDataValue & data)
        {
            if (data.Type != FlvDataType::MIXEDARRAY) {
                data.reset(FlvDataType::OBJECT);
            }
            std::vector<FlvDataObjectProperty> & variables = 
                (data.Type == FlvDataType::OBJECT) ? data.as<FlvDataObject>().ObjectProperties : data.as<FlvDataECMAArray>().Variables;
            if (hasvideo) {
                variables.push_back(FlvDataObjectProperty("hasVideo", 1));
            }
            if (hasaudio) {
                variables.push_back(FlvDataObjectProperty("hasAudio", 1));
            }
            if (datarate > 0) {
                variables.push_back(FlvDataObjectProperty("datarate", datarate));
            }
            if (duration > 0) {
                variables.push_back(FlvDataObjectProperty("duration", (double)duration / 1000.0));
            }
            if (lasttimestamp > 0) {
                variables.push_back(FlvDataObjectProperty("lasttimestamp", (double)lasttimestamp / 1000.0));
            }
            if (videocodecid > 0) {
                variables.push_back(FlvDataObjectProperty("videocodecid", videocodecid));
            }
            if (width > 0) {
                variables.push_back(FlvDataObjectProperty("width", width));
            }
            if (height > 0) {
                variables.push_back(FlvDataObjectProperty("height", height));
            }
            if (framerate > 0) {
                variables.push_back(FlvDataObjectProperty("framerate", framerate));
            }
            if (audiocodecid > 0) {
                variables.push_back(FlvDataObjectProperty("audiocodecid", audiocodecid));
            }
            if (audiosamplerate > 0) {
                variables.push_back(FlvDataObjectProperty("audiosamplerate", audiosamplerate));
            }
            if (audiosamplesize > 0) {
                variables.push_back(FlvDataObjectProperty("audiosamplesize", audiosamplesize));
            }
            if (!keyframes.empty()) {
                variables.push_back(FlvDataObjectProperty("keyframes", FlvDataValue(FlvDataType::OBJECT)));
                FlvDataObject & object = variables.back().PropertyData.as<FlvDataObject>();
                object.ObjectProperties.push_back(FlvDataObjectProperty("filepositions", FlvDataValue(FlvDataType::ARRAY)));
                object.ObjectProperties.push_back(FlvDataObjectProperty("times", FlvDataValue(FlvDataType::ARRAY)));
                FlvDataStrictArray & filepositions = object.ObjectProperties[0].PropertyData.as<FlvDataStrictArray>();
                filepositions.StrictArrayLength = keyframes.size();
                filepositions.StrictArrayValue.resize(keyframes.size());
                FlvDataStrictArray & times = object.ObjectProperties[1].PropertyData.as<FlvDataStrictArray>();
                times.StrictArrayLength = keyframes.size();
                times.StrictArrayValue.resize(keyframes.size());
                for (size_t i = 0; i < keyframes.size(); ++i) {
                    filepositions.StrictArrayValue[i] = (double)keyframes[i].offset;
                    times.StrictArrayValue[i] = (double)keyframes[i].time / 1000;
                }
            }
        }

    } // namespace avformat
} // namespace ppbox
