#pragma once
#include "sv_qtcommon.h"

QString getQJsonValueTypeName(const QJsonValue& value);

template <typename T>
bool jsonValueIsType(const QJsonValue &val);

template<> bool jsonValueIsType<bool>(const QJsonValue &val);
template<> bool jsonValueIsType<double>(const QJsonValue &val);
template<> bool jsonValueIsType<QString>(const QJsonValue &val);
template<> bool jsonValueIsType<QJsonArray>(const QJsonValue &val);
template<> bool jsonValueIsType<QJsonObject>(const QJsonValue &val);

template <typename T>
T convertJsonValueToType(const QJsonValue &val);

template <> bool convertJsonValueToType<bool>(const QJsonValue &val);
template <> double convertJsonValueToType<double>(const QJsonValue &val);
template <> QString convertJsonValueToType<QString>(const QJsonValue &val);
template <> QJsonArray convertJsonValueToType<QJsonArray>(const QJsonValue &val);
template <> QJsonObject convertJsonValueToType<QJsonObject>(const QJsonValue &val);

template <typename T>
std::optional<T> getFromJson(const QJsonObject& json, const QString& key)
{
    auto value = json[key];
    if (jsonValueIsType<T>(value)) return convertJsonValueToType<T>(value);
    return {};
}

template <typename T>
std::optional<T> getFromJson(const QJsonObjectOpt& json, const QString& key)
{
    return json ? getFromJson<T>(*json, key) : std::optional<T>{};
}


template <typename T>
std::optional<T> getFromJsonAndLogError(const QJsonObject& json, const QString& key, const QString& logErrorTextBegin = "JSON error")
{
    auto value = json[key];
    if (value.isUndefined())
    {
        SV_ERROR(QString("%1: QJsonObject doesnt have key \"%2\"")
            .arg(logErrorTextBegin)
            .arg(key).toStdString());
        return {};
    }

    if (!jsonValueIsType<T>(value))
    {
        SV_ERROR(QString("%1: QJsonObject has key \"%2\" but its of unexpected type [%3]")
            .arg(logErrorTextBegin)
            .arg(key)
            .arg(getQJsonValueTypeName(value)).toStdString());
        return {};    
    }

    return convertJsonValueToType<T>(value);
}

template <typename T>
std::optional<T> convertJson(const QJsonValue &value)
{
    return jsonValueIsType<T>(value) ? convertJsonValueToType<T>(value) : std::optional<T>{};
}

template <typename T>
std::optional<T> convertJsonAndLogError(const QJsonValue &value, const QString& logErrorTextBegin = "JSON error")
{
    if (!jsonValueIsType<T>(value))
    {
        SV_ERROR(QString("%1: QJsonValue is of unexpected type [%2]")
            .arg(logErrorTextBegin)
            .arg(getQJsonValueTypeName(value)).toStdString());
        return {};    
    }

    return convertJsonValueToType<T>(value);
}

//we have to wrap it in array or object, this wraps in array
inline QString jsonValueToString(const QJsonValue &value)
{
    QJsonArray myArray;
    myArray.append(value);

    QJsonDocument doc;
    doc.setArray(myArray);

    return doc.toJson(QJsonDocument::Indented).trimmed();
}

inline QJsonArrayOpt jsonStringToArray(const QString& jsonString)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (doc.isArray()) return doc.array();
    else return {};
}

inline QJsonObjectOpt jsonStringToObject(const QString& jsonString)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (doc.isObject()) return doc.object();
    else return {};
}

inline QJsonValueOpt loadJsonFromFile(const QString& jsonFilePath)
{
    QFile file(jsonFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        SV_ERROR(std::format("Cant load JSON from [{}] because cant open file", jsonFilePath));
        return {};
    }

    const QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError)
    {
        SV_ERROR(std::format("Cant load JSON from [{}] because parse error occured:", jsonFilePath, error.errorString()));
        return {};
    }

    return doc.isObject() ? QJsonValue(doc.object())
                          : QJsonValue(doc.array());
}