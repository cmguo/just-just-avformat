// FlvDataType.h

#ifndef _PPBOX_AVFORMAT_FLV_FLV_DATA_TYPE_H_
#define _PPBOX_AVFORMAT_FLV_FLV_DATA_TYPE_H_

#include "ppbox/avformat/flv/FlvFormat.h"

namespace ppbox
{
    namespace avformat
    {

        struct FlvDataString
        {
            boost::uint16_t StringLength;
            std::string StringData;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & StringLength;
                util::serialization::serialize_collection(ar, StringData, StringLength);
            }
        };

        struct FlvDataDate
        {
            double DateTime;
            boost::int16_t LocalDateTimeOffset;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & DateTime;
                ar & LocalDateTimeOffset;
            }
        };

        struct FlvDataLongString
        {
            boost::uint32_t StringLength;
            std::string StringData;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & StringLength;
                util::serialization::serialize_collection(ar, StringData, StringLength);
            }
        };

        struct FlvDataObjectProperty;

        static inline bool FLV_Property_End(
            FlvDataObjectProperty const & Property);

        struct FlvDataObject
        {
            std::vector<FlvDataObjectProperty> ObjectProperties;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                FlvDataObjectProperty Property;
                ObjectProperties.clear();
                while (ar & Property) {
                    ObjectProperties.push_back(Property);
                    if (FLV_Property_End(Property))
                        break;
                }
            }
        };

        struct FlvDataECMAArray
        {
            boost::uint32_t ECMAArrayLength;
            std::vector<FlvDataObjectProperty> Variables;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & ECMAArrayLength;
                FlvDataObjectProperty Property;
                Variables.clear();
                while (ar & Property) {
                    Variables.push_back(Property);
                    if (FLV_Property_End(Property))
                        break;
                }
            }
        };

        struct FlvDataValue;

        struct FlvDataStrictArray
        {
            boost::uint32_t StrictArrayLength;
            std::vector<FlvDataValue> StrictArrayValue;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & StrictArrayLength;
                util::serialization::serialize_collection(ar, StrictArrayValue, StrictArrayLength);
            }
        };

        struct FlvDataValue
        {
            boost::uint8_t Type;
            union {
                double Double;
                boost::uint64_t _Double;
                boost::uint8_t Bool;
                boost::uint16_t MovieClip;
                boost::uint16_t Null;
                boost::uint16_t Undefined;
                boost::uint16_t Reference;
                boost::uint16_t ObjectEndMarker;
                FlvDataDate Date;
            };
            FlvDataString String;
            FlvDataLongString LongString;
            FlvDataObject Object;
            FlvDataECMAArray ECMAArray;
            FlvDataStrictArray StrictArray;
            template <typename Archive>

            void serialize(
                Archive & ar)
            {
                ar & Type;
                switch (Type) {
                    case AMFDataType::NUMBER:
                        ar & _Double;
                        break;
                    case AMFDataType::BOOL:
                        ar & Bool;
                        break;
                    case AMFDataType::STRING:
                        ar & String;
                        break;
                    case AMFDataType::OBJECT:
                        ar & Object;
                        break;
                    case AMFDataType::MOVIECLIP:
                        ar.fail();
                        break;
                    case AMFDataType::_NULL:
                        break;
                    case AMFDataType::UNDEFINED:
                        break;
                    case AMFDataType::REFERENCE:
                        ar & Reference;
                        break;
                    case AMFDataType::MIXEDARRAY:
                        ar & ECMAArray;
                        break;
                    case AMFDataType::OBJECT_END:
                        break;
                    case AMFDataType::ARRAY:
                        ar & StrictArray;
                        break;
                    case AMFDataType::DATE:
                        ar & Date;
                        break;
                    case AMFDataType::LONG_STRING:
                        ar & LongString;
                        break;
                    default:
                        ar.fail();
                        break;
                }
            }
        };

        struct FlvDataObjectProperty
        {
            FlvDataString PropertyName;
            FlvDataValue PropertyData;

            template <typename Archive>
            void serialize(
                Archive & ar)
            {
                ar & PropertyName
                    & PropertyData;
            }
        };

        static inline bool FLV_Property_End(
            FlvDataObjectProperty const & Property)
        {
            return Property.PropertyData.Type == AMFDataType::OBJECT_END;
        }

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_FLV_FLV_DATA_TYPE_H_
