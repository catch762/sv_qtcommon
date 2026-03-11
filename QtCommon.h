#pragma once
#include "sv_common.h"

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QFrame>

SV_DECL_OPT(QString)
SV_DECL_OPT(QJsonArray)
SV_DECL_OPT(QJsonObject)

inline QStringOpt jsonGetStringOpt(const QJsonObject& json, const QString& key)
{
    auto value = json[key];
    if (value.isString()) return value.toString();
    else return {};
};

inline QStringOpt jsonGetStringOptAndLogError(const QJsonObject& json, const QString& key, const QString& logErrorTextBegin)
{
    auto valueOpt = jsonGetStringOpt(json, key);
    if (!valueOpt)
    {
        SV_ERROR(QString("%1: no string under key \"%2\"").arg(logErrorTextBegin).arg(key).toStdString());
    }
    return valueOpt;
};

inline doubleOpt jsonGetDoubleOpt(const QJsonObject& json, const QString& key)
{
    auto value = json[key];
    if (value.isDouble()) return value.toDouble();
    else return {};
};

inline doubleOpt jsonGetDoubleOptAndLogError(const QJsonObject& json, const QString& key, const QString& logErrorTextBegin)
{
    auto valueOpt = jsonGetDoubleOpt(json, key);
    if (!valueOpt)
    {
        SV_ERROR(QString("%1: no double under key \"%2\"").arg(logErrorTextBegin).arg(key).toStdString());
    }
    return valueOpt;
};

inline QJsonObjectOpt jsonGetObjectOptAndLogError(const QJsonValue& json, const QString& logErrorTextBegin)
{
    if (json.isObject())
    {
        return json.toObject();
    }
    else
    {
        SV_ERROR(QString("%1: value is not an object").arg(logErrorTextBegin).toStdString());
        return {};
    }
}

inline QJsonArrayOpt jsonGetArrayOpt(const QJsonObject& json, const QString& key)
{
    auto value = json[key];
    if (value.isArray()) return value.toArray();
    else return {};
};

using QtTypeIndex = int;

//returns QMetaType::UnknownType (0) if unregistered
template <typename T>
inline QtTypeIndex qtTypeId()
{
    return QMetaType::fromType<T>().id();
}

//returns "" if unregistered
template <typename T>
inline QString qtTypeName()
{
    return QMetaType::fromType<T>().name();
}

//this is basic check, maybe name for this class is still not set even if id is obtainable
template <typename T>
inline bool qtTypeIsRegistered()
{
    return qtTypeId<T>() != QMetaType::UnknownType;
}

template <typename T>
inline bool qtTypeIsRegisteredAndNamed()
{
    return qtTypeIsRegistered<T>() && !qtTypeName<T>().isEmpty();
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

inline QString qVariantInfo(const QVariant &var)
{
    return QString("QVariant[typeid=%1][typename=%2][tostring=%3]").arg(var.typeId()).arg(var.typeName()).arg(var.toString());
}