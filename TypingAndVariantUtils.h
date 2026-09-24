#pragma once
#include "sv_common.h"
#include <QVariant>

using QtTypeIndex = int;
SV_DECL_OPT(QtTypeIndex);

//returns QMetaType::UnknownType (0) if unregistered
template <typename T>
QtTypeIndex qtTypeId()
{
    return QMetaType::fromType<T>().id();
}

//returns "" if unregistered
template <typename T>
QString qtTypeName()
{
    return QMetaType::fromType<T>().name();
}

inline QString qtTypeName(QtTypeIndex typeIndex)
{
    return QString::fromLatin1(QMetaType(typeIndex).name());
}

//this is basic check, maybe name for this class is still not set even if id is obtainable
template <typename T>
bool qtTypeIsRegistered()
{
    return qtTypeId<T>() != QMetaType::UnknownType;
}

template <typename T>
bool qtTypeIsRegisteredAndNamed()
{
    return qtTypeIsRegistered<T>() && !qtTypeName<T>().isEmpty();
}

template <typename T>
bool canConvert(const QVariant &qVariant)
{
    return qVariant.canConvert(QMetaType::fromType<T>());
}

template<typename T>
bool holdsType(const QVariant& v)
{
    return v.typeId() == qtTypeId<T>();
}

template <typename T>
std::optional<T> getValueOpt(const QVariant &qVariant)
{
    if(canConvert<T>(qVariant)) return qVariant.value<T>();
    else return {};
}

template <typename T>
T getValueOr(const QVariant &qVariant, const T& defaultVal = {})
{
    if(canConvert<T>(qVariant)) return qVariant.value<T>();
    else return defaultVal;
}

template <typename T>
QString qtTypeInfo()
{
    return QString("[type id=%1, name=%2]").arg(qtTypeId<T>()).arg(qtTypeName<T>());
} 

inline QString qtTypeInfo(QtTypeIndex typeIndex)
{
    return QString("[type id=%1, name=%2]").arg(typeIndex).arg(qtTypeName(typeIndex));
}

inline QString qVariantInfo(const QVariant &var)
{
    return QString("QVariant[typeid=%1][typename=%2][tostring=%3]").arg(var.typeId()).arg(var.typeName()).arg(var.toString());
}