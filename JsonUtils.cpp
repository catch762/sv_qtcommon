#include "JsonUtils.h"

QString getQJsonValueTypeName(const QJsonValue& value)
{
    switch (value.type())
    {
        case QJsonValue::Null:      return "NULL";
        case QJsonValue::Bool:      return "bool";
        case QJsonValue::Double:    return "double";
        case QJsonValue::String:    return "QString";
        case QJsonValue::Array:     return "QJsonArray";
        case QJsonValue::Object:    return "QJsonObject";
        case QJsonValue::Undefined: return "UNDEFINED";
    }
    return "UNKNOWN";
}

template <>
bool jsonValueIsType<bool>(const QJsonValue &val) {
    return val.isBool();
}

template <>
bool jsonValueIsType<double>(const QJsonValue &val) {
    return val.isDouble();
}

template <>
bool jsonValueIsType<int>(const QJsonValue &val) {
    return val.isDouble();
}

template <>
bool jsonValueIsType<QString>(const QJsonValue &val) {
    return val.isString();
}

template <>
bool jsonValueIsType<QJsonArray>(const QJsonValue &val) {
    return val.isArray();
}

template <>
bool jsonValueIsType<QJsonObject>(const QJsonValue &val) {
    return val.isObject();
}

template <>
bool convertJsonValueToType<bool>(const QJsonValue &val) {
    return val.toBool();
}

template <>
double convertJsonValueToType<double>(const QJsonValue &val) {
    return val.toDouble();
}

template <>
int convertJsonValueToType<int>(const QJsonValue &val) {
    return int(val.toDouble());
}

template <>
QString convertJsonValueToType<QString>(const QJsonValue &val) {
    return val.toString();
}

template <>
QJsonArray convertJsonValueToType<QJsonArray>(const QJsonValue &val) {
    return val.toArray();
}

template <>
QJsonObject convertJsonValueToType<QJsonObject>(const QJsonValue &val) {
    return val.toObject();
}